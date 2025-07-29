<h1 align="center">V3C Immersive Platform - V-PCC generation tools</h1>
<p align="center">
  <img src="https://img.shields.io/badge/Status-Under_Development-yellow" alt="Under Development">
  <img src="https://img.shields.io/badge/License-5G--MAG%20Public%20License%20(v1.0)-blue" alt="License">
</p>

## 📖 Overview

This directory provides a set of tools for preparing and processing Dynamic Point Cloud data into V-PCC bitstream for compatibility with the **V3C Immersive Platform**.
It includes modules for encoding PLY files, generating data chunks, and running test scripts.

## 🛠️ V-PCC generation tools 

Requirements:
- to run scripts install python3 on your Windows machine
- open a bash shell (e.g. Git bash) and install missing packages (e.g. python.exe -m pip install argparse, glob, subprocess)

### `encodePly`
Encodes PLY files into a V3C bitstream with TMC2 V-PCC reference software.

Example command:  
`python3.exe encode.py -i "$INPUT_PLY_DIR" -o "$OUTPUT_DIR" --tmc2Dir "$TMC2_DIR" --seqNb "$SEQ_ID" --seqCfg "$SEQ_CFG" -n "$NB_FRAME" -r "$RATE"`

> **Note:** For TMC2, use the version compiled with the [decoder plugin](https://github.com/5G-MAG/rt-v3c-decoder-plugin) to have TMC2 options properly set.

### `createVpccChunks`
Generates data chunks from encoded V-PCC bin files for ingestion by the V3C Immersive Platform - local interface.

> **Note:** A dedicated software tool is provided for chunk creation. This tool must be compiled before use.

Example build command:  
`python3.exe build_win64.py --tmc2 "$TMC2_DIR"`

Example command:  
`python3.exe create.py --inputDir "$INPUT_BIN_DIR" --outputDir "$OUTPUT_DIR" --fps "$FPS"`

### `test`
Contains test scripts and usage examples for the above modules.


## 📦 Dependencies

- `tmc2`, version 25.0  
  Repository: [github.com/MPEGGroup/mpeg-pcc-tmc2](https://github.com/MPEGGroup/mpeg-pcc-tmc2.git)
  
## ✅ Supported Features and Limitations

The **V3C Immersive Platform** currently supports a limited subset of features, as outlined below:

### Supported Profile
- **Codec**: HEVC V-PCC Basic Profile Rec 0
- **Constraints**:
  - `videoEncoderInternalBitdepth` set to **8**
  - `mapCountMinus1` set to **0**

### Atlas Constraints
- Only **constant atlas size** is supported for decoding.

> **Note:** To generate encoded bitstream with constant atlas size, two-pass encoding may be required. A first encoding pass with a default size. If the atlas size is too small and is changed during the encoding sequence, a second encoding pass with a bigger size is needed (set '--width' and '--height' input parameters) to get an encoded bitstream with constant atlas size.

### Bitstream Requirements
- Bitstreams must contain **at least 4 frames**.

## 📄 License

This project is provided under the 5G-MAG Public License (v1.0).  
For the full license terms, please refer to the `LICENSE` file in the repository or access it [here](https://drive.google.com/file/d/1cinCiA778IErENZ3JN52VFW-1ffHpx7Z/view).
