#!/bin/bash

# Copyright (c) 2024 InterDigital R&D France
# Licensed under the License terms of 5GMAG software (the "License").
# You may not use this file except in compliance with the License.
# You may obtain a copy of the License at https://www.5g-mag.com/license .
# Unless required by applicable law or agreed to in writing, software distributed under the License is
# distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and limitations under the License.


#=============================================================================================
#
# Bash script to segment:
#   - MIV V23.0 bitstreams (.bit)
#   - VPCC r25 bitstreams (.bin)
#
#=============================================================================================
# shellcheck disable=SC2086 disable=SC2068 disable=SC2012 disable=SC2046
true
set -e
IFS=''

deps_dir=undefined
root_dir=undefined

function usage {
    echo "Usage:"
    echo "    -i | --inDir \"Input directory with V-PCC encoded bitstream\""
    echo "    -f | --filelist \"File that contains a list of MIV encoded bitstreams with their absolute path\""
    echo "                    \"Note: Use absolute path for the file list.\""
    echo "    -o | --outDir \"Output directory to dump the generated segments\""
    echo "    --fps_occ \"V3C bitstream FPS for occupancy, typically 25 or 30\""
    echo "    --fps_geo \"V3C bitstream FPS for geometry, typically 25 or 30\""
    echo "    --fps_atl \"V3C bitstream FPS for atlas, typically 25 or 30\""
    echo "    --fps_att \"V3C bitstream FPS for attribute, typically 25 or 30\""
    echo "    --fps \"V3C bitstream FPS for occupancy, geometry, atlas and attribute, typically 25 or 30\""
    echo "    -c | --clear \"clear old output files\""
    echo "    -m | --merge-inDir \"build a filelist from inDir files and process extrat on built filelist\""
    echo "    -h | --help"
    echo ""
    echo "Note:"
    echo "  1) Search for .bit or .bin file recursively in the input directory"
    echo "  2) Directory paths may be absolute or relative"
    exit 0
}

function load_params {
  out_root_dir=$root_dir/_out
  fps_occ=30
  fps_geo=30
  fps_atl=30
  fps_attr=30
  clear_output=no
  in_file_list=undefined
  in_dir=undefined
  in_dir_merge=no

  [ $# -eq 0 ] && usage

  while [ $# -gt 0 ]
  do
    case "$1" in
      -i|--inDir)
        in_dir="$2"; shift;;
      -f|--filelist)
        in_file_list="$2"; shift;;
      -o|--outDir)
        out_root_dir="$2"; shift;;
      --fps_occ)
        fps_occ="$2"; shift;;
      --fps_geo)
        fps_geo="$2"; shift;;
      --fps_atl)
        fps_atl="$2"; shift;;
      --fps_att)
        fps_attr="$2"; shift;;
      --fps)
        fps_occ="$2"; fps_geo="$2"; fps_atl="$2"; fps_attr="$2"; shift;;
      -c|--clear)
        clear_output=yes;;
      -m|--merge-inDir)
        in_dir_merge=yes;;
      -h|--help|*)
        usage;;
    esac
    shift
  done

  mkdir -p $out_root_dir

  in_dir=$(readlink -f $in_dir)
  out_root_dir=$(readlink -f $out_root_dir)
  log_file="$out_root_dir/v3cpackager.log"
  [ -f $log_file ] && rm $log_file

  echo "out_root_dir=$out_root_dir"
  echo "fps_occ=$fps_occ"
  echo "fps_geo=$fps_geo"
  echo "fps_atl=$fps_atl"
  echo "fps_attr=$fps_attr"
  echo "clear_output=$clear_output"
  echo "in_file_list=$in_file_list"
  echo "in_dir=$in_dir"
  echo "in_dir_merge=$in_dir_merge"
}

function log {
    echo "$*"
    echo "$*" >> $log_file
}

function error {
    echo "[Error]" $@
    exit 1
}


function find_packager {
  v3cpackager=$(find . -name v3cpackager.exe)
  [ -f "$v3cpackager" ] || error "Can't find v3cpackager.exe."
  v3cpackager=$(readlink -f $v3cpackager)
}

function run_packager {
  if [ -d $out_dir ]
  then
    if [ $clear_output == "yes" ]
    then
      rm -rf $clear_output
    else
      echo "Error: ${out_dir} already exists, remove it first."
      return
    fi
  fi

  mkdir -p $out_dir

  log ""
  log "$out_dir"
  log "$ $v3cpackager --verbose --vod --occupancy-fps $fps_occ --attribute-fps $fps_attr --geometry-fps $fps_geo --atlas-fps $fps_atl $*"

  if "$v3cpackager" --verbose --vod --occupancy-fps "$fps_occ" --attribute-fps "$fps_attr" --geometry-fps "$fps_geo" --atlas-fps "$fps_atl" $@ >> $log_file 2>&1
  then
    echo "success"
  else
    cat $log_file
    error "$v3cpackager failed"
  fi

  [ ! -d "v3c" ] && error "v3c bitstream not segmented, see log file."

  cp -r v3c/output/* ${out_dir}/.
  rm -rf v3c
}

function set_out_dir {
  reference_file=$1
  reference_file_basename=$(basename "${reference_file}")
  out_dir="$out_root_dir/${reference_file_basename%.*}"
}

function run_packager_from_filelist {
  set_out_dir $in_file_list
  run_packager --track-list ${in_file_list}
}

function run_packager_from_dir {
  cd $in_dir
  mapfile -t files < <(find . -type f \( -name "*.bit" -o -name "*.bin" \) -print | sort)
  if [ $in_dir_merge == yes ]
  then
    tmp_file_list=_tmp_file.txt
    printf "%s\n" "${files[@]}" > $tmp_file_list
    set_out_dir $in_dir
    run_packager --track-list ${tmp_file_list}
  else
    for _file in ${files[@]}
    do
      set_out_dir $_file
      run_packager --track $_file
    done
  fi
}

function run_packager_from_options {
  if [ -f $in_file_list ]
  then
    run_packager_from_filelist
  elif [ -d $in_dir ]
  then
    run_packager_from_dir
  else
    error "set input"
  fi
}

function clean_log_file {
  # remove ^M from the log file
  if [[ -f  $log_file ]]
  then
    sed -e "s/^M//" $log_file > _tmpfile
    mv _tmpfile $log_file
  fi
}

function main {
    load_params $@
    find_packager
    run_packager_from_options
    clean_log_file
}

main $@
