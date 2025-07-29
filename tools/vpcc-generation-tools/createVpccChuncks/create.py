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

import os, platform, pathlib, sys, argparse, glob, subprocess, posixpath, ntpath

def str2bool(v):
    if isinstance(v, bool):           return v
    if v.lower() in ('true', '1'):    return True
    elif v.lower() in ('false', '0'): return False
    else:                             raise argparse.ArgumentTypeError('Boolean value expected.')

def printCmd(command):
    repl_map = {"C:": "/C", "c:": "/C", "\\": "/"}
    for key, value in repl_map.items():
        command = command.replace(key, value)
    print ("cmd=",command, flush=True)
                    
parser = argparse.ArgumentParser(description='Create local chunks from multiple V-PCC bin files suitable for the V3C Immersive Platform')
parser.add_argument('-i',   '--inputDir',  help="Input BIN directory")
parser.add_argument('-o',   '--outputDir', help="Output cunk BIN directory")
parser.add_argument('-fps', '--fps',     help="sequence FPS (optional, default=30)", nargs="?", default="30", type=int, choices=[25,30,50,60,120])

if len(sys.argv)==1:
    parser.print_help(sys.stderr)
    sys.exit(1)
args = parser.parse_args()

GREEN =  '\033[92m' # Green Text
BLUE = '\033[94m'
RED = '\033[91m'
ENDC = '\033[m' # reset to the defaults

##################
def main():

    try:
        plt = platform.system()
        if plt == "Windows":
            print(BLUE + "Your system is Windows", ENDC)
        elif plt == "Linux":
            print(BLUE + "Your system is Linux, unsupported system", ENDC)
        elif plt == "Darwin":
            raise ValueError("Your system is MacOS, unsupported system")
        else:
            raise ValueError("Unidentified system")

        print ("-------------------------------------------")
        print (BLUE + "Input Arguments :" , ENDC)
        print (BLUE + "\tinputDir     =", args.inputDir , ENDC)
        print (BLUE + "\toutputDir    =", args.outputDir , ENDC)
        print (BLUE + "\tfps          =", args.fps , ENDC)
        print ("-------------------------------------------", flush=True)
            
        scriptDir = os.path.dirname(os.path.abspath(__file__)).replace("\\","/")
        inputDir = os.path.abspath(args.inputDir)
        outputDir = os.path.abspath(args.outputDir)
        vpccChunckCreatorExe = os.path.join(scriptDir, "bin", "Windows", "VpccChunkCreator.exe").replace("\\","/")
       
        if not os.path.isfile(vpccChunckCreatorExe):
            raise ValueError("Exe not found : ", vpccChunckCreatorExe)
        
        logFile = "".join([os.path.os.path.splitext(outputDir)[0], "/", pathlib.PurePath(inputDir).name, "_chunkCreation.log"])
        os.makedirs(outputDir, exist_ok=True)
        
        for elem in os.listdir(inputDir):
            if elem.endswith(".bin"):
                print (GREEN + "Create chunks for ", elem, ENDC)
                fps=args.fps
                if "footlive" in elem or "footprod" in elem :
                    fps=25
                config = "".join( [ " -i ", inputDir, "/", elem, 
                                    " -fps ", str(fps),
                                    " -o ", outputDir,
                                    " > ",  logFile])
                cmd = " ".join([vpccChunckCreatorExe, config]) 
                printCmd(cmd)
                subprocess.check_call(cmd, shell=True)
               
    except Exception as e:
        print (RED + "Exception:", e, ENDC)
        sys.exit()
    except subprocess.CalledProcessError as e:
        print(RED + "subprocess Exception:", e.returncode, ENDC)
        print(RED + e.cmd, ENDC)
        print(RED + e.output, ENDC)        
        sys.exit();        
        
if __name__ == "__main__":
    main()
