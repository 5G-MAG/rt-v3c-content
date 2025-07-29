#!/bin/bash

# set test configuration
SCRIPT_DIR="/C/pathto/rt-v3c-content/tools/vpcc-generation-tools/createVpccChuncks"
INPUT_BIN_DIR="/C/pathto/rt-v3c-content/tools/vpcc-generation-tools/test/output_bin"
OUTPUT_DIR="/C/pathto/rt-v3c-content/tools/vpcc-generation-tools/test/output_chunk"
TMC2_DIR="/C/pathto/rt-v3c-decoder-plugin/External/mpeg-pcc-tmc2/25.0/"
FPS="25"

# build the application
cd $SCRIPT_DIR/source
python3.exe build_win64.py --tmc2 "$TMC2_DIR"

# launch the chunk creation
cd $SCRIPT_DIR
python3.exe create.py --inputDir "$INPUT_BIN_DIR" --outputDir "$OUTPUT_DIR" --fps "$FPS"