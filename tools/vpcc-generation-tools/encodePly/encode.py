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
import os, platform, sys, argparse, glob
from pathlib import Path

import subprocess

def str2bool(v):
    if isinstance(v, bool):           return v
    if v.lower() in ('true', '1'):    return True
    elif v.lower() in ('false', '0'): return False
    else:                             raise argparse.ArgumentTypeError('Boolean value expected.')

parser = argparse.ArgumentParser(description='encode point clouds with TMC2 reference software (V-PCC) for the V3C Immersive Platform')
parser.add_argument('-i', '--inputDir',    help="Input PLY directory")
parser.add_argument('-o', '--outputDir',   help="Output BIN directory")
parser.add_argument(      '--tmc2Dir',     help="Path to TMC2 (optional, default=vpcc/thirdparty/mpeg-pcc-tmc2)", nargs="?", default="vpcc/thirdparty/mpeg-pcc-tmc2", type=str)
parser.add_argument(      '--seqCfg',      help="Sequence Config file (optional, default=./seq_vox10.cfg)", nargs="?", default="./seq_vox10.cfg", type=str)
parser.add_argument(      '--seqNb',       help="Output sequence Number (optional, default=80)", nargs="?", default="80", type=int)
parser.add_argument('-n', '--frameNumber', help="Number of frames to process (optional, default=4, 1000 corresponds to all frames in inputDir)", nargs="?", default="4", type=int, choices=[4,8,32,1000])
parser.add_argument('-r', '--rate',        help="Compression rate with 1 the lowest quality and 5 the highest quality (optional, default=5)", nargs="?", default="5", type=int, choices=[1,2,3,4,5])
parser.add_argument(      '--width',       help="Width of coded video (optional, default=1472)", nargs="?", default="1472", type=int)
parser.add_argument(      '--height',      help="Height of coded video (optional, default=1600)", nargs="?", default="1600", type=int)
parser.add_argument('-e', '--forceEncode', help="Force the encoding (optional, default=False)", nargs="?", default=False, const=True, type=str2bool)
parser.add_argument('-d', '--forceDecode', help="Force the decoding (optional, default=False)", nargs="?", default=False, const=True, type=str2bool)

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
               
        print ("-------------------------------------------")
        print (BLUE + "Input Arguments :" , ENDC)
        print (BLUE + "\tinputDir     =", args.inputDir , ENDC)
        print (BLUE + "\toutputDir    =", args.outputDir , ENDC)
        print (BLUE + "\ttmc2         =", args.tmc2Dir , ENDC)
        print (BLUE + "\tseqCfg       =", args.seqCfg , ENDC)
        print (BLUE + "\tseqNb        =", args.seqNb , ENDC)
        print (BLUE + "\tframeNumber  =", args.frameNumber , ENDC)
        print (BLUE + "\trate         =", args.rate , ENDC)
        print (BLUE + "\twidth        =", args.width , ENDC)
        print (BLUE + "\theight       =", args.height , ENDC)
        print (BLUE + "\tforceEncode  =", args.forceEncode , ENDC)
        print (BLUE + "\tforceDecode  =", args.forceDecode , ENDC)
        print ("-------------------------------------------", flush=True)
      
        if not os.path.exists(args.inputDir) :
            raise ValueError("Check dir", args.inputDir)
        if not os.path.exists(args.tmc2Dir):
            raise ValueError("Check dir", args.tmc2Dir)

        scriptDir = Path(__file__).resolve(strict=True)
        inputDir  = Path(args.inputDir).resolve(strict=True)    
        outputDir = Path(args.outputDir).resolve()
        tmc2Dir   = Path(args.tmc2Dir).resolve()
        seqCfg    = Path(args.seqCfg).resolve()

        if not os.path.exists(seqCfg):
            raise ValueError("Check dir", seqCfg)
        
        # check options        
        frameNumber = args.frameNumber
        if (args.frameNumber == 1000):
            frameNumber = len(glob.glob1(inputDir,"*.ply"))
            
        plt = platform.system()
        if plt == "Windows":
            print(BLUE + "Your system is Windows", ENDC)
            encoder=Path(tmc2Dir).joinpath("bin", "Release", "PccAppEncoder.exe")
            decoder=Path(tmc2Dir).joinpath("bin", "Release", "PccAppDecoder.exe")
        elif plt == "Linux":
            print(BLUE + "Your system is Linux", ENDC)
            encoder=Path(tmc2Dir).joinpath("bin", "PccAppEncoder")
            decoder=Path(tmc2Dir).joinpath("bin", "PccAppDecoder")
        else:
            raise ValueError("Your system is not supported")

        if not encoder.exists():
            raise ValueError("Exe not found : ", encoder)
        if not decoder.exists():
            raise ValueError("Exe not found : ", decoder)       

        compressedPath  = Path(outputDir).joinpath("".join(["F", str(frameNumber), "_map1_1472x1600_08bits"]))
        outputPrefix ="".join(["S",str(args.seqNb),"C2RAR%02d" % args.rate]);
        cmdFile      = Path(compressedPath).joinpath("".join([outputPrefix, "_command.log"]))

        #create outputDir if does not exist
        if not compressedPath.exists():
            print("create dir:", compressedPath, flush=True);
            os.makedirs(compressedPath, exist_ok=True)

        compressBinFile = Path(compressedPath).joinpath("".join([outputPrefix, "_enc.bin"]))
        encoderFile     = Path(compressedPath).joinpath("".join([outputPrefix, "_encoder.log"]))
        decoderFile     = Path(compressedPath).joinpath("".join([outputPrefix, "_decoder.log"]))
        plyDecPath      = Path(compressedPath).joinpath("".join([outputPrefix, "_dec_%04d.ply"])) 

        #search info in Sequence cfg file
        with open(seqCfg) as f:
            for line in f:
                if "startFrameNumber" in line:
                    startFrameNb = int(line.split(":")[1])
                if "geometry3dCoordinatesBitdepth" in line:
                    resolution = 1023 if int(line.split(":")[1]) == 10 else 2047
               
               
        # ENCODER
        if not os.path.exists(compressBinFile)  or args.forceEncode:
            print (GREEN  + "Encode: ", compressBinFile,  ENDC, flush=True)
            config = "".join([
                        " --config=", str(Path(tmc2Dir).joinpath("cfg", "common", "ctc-common.cfg")),
                        " --config=", str(Path(tmc2Dir).joinpath("cfg", "condition", "ctc-random-access.cfg")), 
                        " --config=", str(seqCfg),
                        " --config=", str(Path(tmc2Dir).joinpath("cfg", "rate", "".join(["ctc-r", str(args.rate), ".cfg"]))),
                        " --configurationFolder=", str(Path(tmc2Dir).joinpath("cfg")), os.sep,
                        " --uncompressedDataFolder=", str(inputDir), os.sep,
                        " --compressedStreamPath=", str(compressBinFile),
                        " --nbThread=1",
                        " --frameCount=", str(frameNumber),                                        
                        " --resolution=", str(resolution),
                        " --mapCountMinus1=0",                                
                        " --minimumImageHeight=", str(args.height),
                        " --minimumImageWidth=", str(args.width),
                        " --videoEncoderInternalBitdepth=8 > ", str(encoderFile), " 2>&1"
                        ])
                                              
            
            cmd = " ".join([str(encoder), config]) 
            f = open(cmdFile,'w')
            print(cmd, file=f) 
            print("CMD=", cmd)
            f.close()        
            subprocess.check_call(cmd, shell=True)
        
        else:
            print (GREEN  + "Already encoded: ",compressBinFile,  ENDC, flush=True)
        
        # DECODER
        if args.forceDecode:      
            print (GREEN  + "Decode", compressBinFile, ENDC, flush=True)
        
            config = "".join(
                        [" --startFrameNumber=", str(startFrameNb),
                        " --compressedStreamPath=", str(compressBinFile),
                        " --reconstructedDataPath=", str(plyDecPath),
                        " --inverseColorSpaceConversionConfig=", str(Path(tmc2Dir).joinpath("cfg", "hdrconvert", "yuv420toyuv444_16bit.cfg")),
                        " --nbThread=1 > ", str(decoderFile)])
            cmd = " ".join([str(decoder), config]) 
            f = open(cmdFile,'a')
            print(cmd, file=f) 
            f.close()
            subprocess.check_call(cmd, shell=True)
            isDecodedProcessDone = True
        else:
            print (GREEN  + "Already decoded: ",compressBinFile,  ENDC, flush=True)
                    
    except Exception as e:
        print (RED + "Exception:", e, ENDC)
        sys.exit();
    except subprocess.CalledProcessError as e:
        print(RED + "subprocess Exception:", e.returncode, ENDC)
        print(RED + e.cmd, ENDC)
        print(RED + e.output, ENDC)
        print("standard output:", e.stdout)
        print("standard error :", e.stderr)
        sys.exit(e.returncode);
                
if __name__ == "__main__":
    main()