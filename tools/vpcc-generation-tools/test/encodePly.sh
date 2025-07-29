#!/bin/bash

# set test configuration
SCRIPT_DIR="/C/pathto/rt-v3c-content/tools/vpcc-generation-tools/encodePly"
INPUT_PLY_DIR="/C/pathto/PCC/contents/football-production"
TMC2_DIR="/C/pathto/rt-v3C-decoder-plugin/External/mpeg-pcc-tmc2/25.0"
SEQ_ID="80"
SEQ_CFG="footprod.cfg"
OUTPUT_DIR="/C/pathto/rt-v3c-content/tools/vpcc-generation-tools/test/output_bin"
NB_FRAME="4"
RATE="1"

# launch the encoding
cd $SCRIPT_DIR
python3.exe encode.py -i "$INPUT_PLY_DIR" -o "$OUTPUT_DIR" --tmc2Dir "$TMC2_DIR" --seqNb "$SEQ_ID" --seqCfg "$SEQ_CFG" -n "$NB_FRAME" -r "$RATE"

# for test purpuse, copy all .bin in subdirectories
find "$OUTPUT_DIR" -mindepth 2 -type f -name "*.bin" -exec cp {} "$OUTPUT_DIR" \;
