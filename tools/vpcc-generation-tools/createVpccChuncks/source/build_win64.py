#!/usr/bin/python3

"""\
* Copyright (c) 2025 InterDigital CE Patent Holdings SASU
* Licensed under the License terms of 5GMAG software (the "License").
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at https://www.5g-mag.com/license .
* Unless required by applicable law or agreed to in writing, software distributed under the License is
* distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and limitations under the License.
"""

"""\
This script clone all external dependancies needed for vpcc tools content generation
"""
import os, sys, argparse, glob, platform
import shutil, vswhere, subprocess, cmake
from pathlib import Path

def str2bool(v):
    if isinstance(v, bool):           return v
    if v.lower() in ('true', '1'):    return True
    elif v.lower() in ('false', '0'): return False
    else:                             raise argparse.ArgumentTypeError('Boolean value expected.')
    
parser = argparse.ArgumentParser(description='dependencies script building - Use : python.exe -m pip install gitpython vswhere cmake')
parser.add_argument('-m', '--mode',    help="Define compilation mode : Release or Debug (default=release)", default="release", type=str)
parser.add_argument(      '--tmc2Dir', help="Path to TMC2 reference software", nargs="?", type=str, required=True)
parser.add_argument('-c', '--clean',   help="Force cleaning (optional, default=False)", nargs="?", default=False, const=True, type=str2bool)

args = parser.parse_args()

GREEN =  '\033[92m' # Green Text
BLUE = '\033[94m'
RED = '\033[91m'
ENDC = '\033[m' # reset to the defaults

##################
def main():

    try:
        print ("-------------------------------------------")
        print (BLUE + "Input Arguments :" , ENDC)
        print (BLUE + "\tmode  =", args.mode , ENDC)
        print (BLUE + "\ttmc2  =", args.tmc2Dir , ENDC)
        print (BLUE + "\tclean =", args.clean , ENDC)
        print ("-------------------------------------------", flush=True)
        
        compilationMode = args.mode.lower()        
        sourceChunckCreation = str(Path(__file__).resolve(strict=True).parent)
        tmc2Dir   = str(Path(args.tmc2Dir).resolve())

        MsGenerator=" ".join([str(vswhere.find(legacy=True, latest=True, prop='ProductName')[0]), 
                              str(vswhere.get_latest_major_version()), 
                              str(vswhere.find(legacy=True, latest=True, prop='productLineVersion')[0])])
        print (GREEN + "Microsoft Generator = ", MsGenerator, ENDC)
           
        # clean mode
        if args.clean:
            build_dir = os.path.join(sourceChunckCreation, "..", "build")
            bin_dir = os.path.join(sourceChunckCreation, "..", "bin")
            for directory in [build_dir, bin_dir]:
                if os.path.exists(directory):
                    shutil.rmtree(directory)
                    print(RED + "Deleted: ", directory, ENDC)
                else:
                    print(RED + "Directory not found (skipped): ", directory, ENDC)
            print(RED + "Cleaning sourceChunckCreation done", ENDC)
        
        currentDir = os.getcwd()
        
        # build project sourceChunckCreation
        path = os.path.join(sourceChunckCreation, "..", "build", "msvc", compilationMode)
        
        if not  os.path.exists(path):
            os.makedirs(path)
        os.chdir(path)

        cmd="".join(["cmake -G\"",MsGenerator,"\"",
             " -DTMC2_DIR=", tmc2Dir,  
             " -DBUILD_MODE=", compilationMode, 
             " ", sourceChunckCreation])

        subprocess.check_call(cmd, shell=True)
        cmd="".join(["cmake --build . --config ", compilationMode, " --parallel --target install --clean-first"])
        subprocess.check_call(cmd, shell=True)
        print(GREEN + "compile sourceChunckCreation done", ENDC)
        os.chdir(currentDir)     

        # copy shared librairies to exe directory
        tmc2DllDir=os.path.join(tmc2Dir, "bin", compilationMode, "*.dll")        
        dst=os.path.join(sourceChunckCreation, "..", "bin", platform.system())
        listDir=[tmc2DllDir]
                
        for dir in listDir:
            for filename in glob.glob(dir):
                shutil.copy(filename, dst)
        print(GREEN + "copy shared libraries done", ENDC)
        
    except Exception as e:
        print (RED + "Exception:", e, ENDC)
    

if __name__ == "__main__":
    main()

