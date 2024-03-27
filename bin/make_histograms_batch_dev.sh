#!/bin/bash

shopt -s nullglob

# create histograms
function unpack_data(){
    # printf "Processing run %d\n" $1
    file_pattern="$ANALYSIS_DIR/analysis${1}_[0-9][0-9][0-9].root"
    analysis_tree_array=($file_pattern)

    # hist_file_array=($hist_file_pattern)
    # get first subrun
    first_subrun=${analysis_tree_array[0]}
    first_fullname=${first_subrun##*/}
    first_filename=${first_fullname%%.*}
    first_subrun=${first_filename##*_}
    # get last subrun
    last_subrun=${analysis_tree_array[-1]}
    last_fullname=${last_subrun##*/}
    last_filename=${last_fullname%%.*}
    last_subrun=${last_filename##*_}
    # build final filename
    # histogram_filename="run${1}_${first_subrun}-${last_subrun}.root"
    # printf "%s\n" "${analysis_tree_array[@]}"
    for value in "${analysis_tree_array[@]}"
    do
        echo $value
        histogram_filename="run${1}_${first_subrun}.root"
    done

    # if [[ -f ${HIST_DIR}/${histogram_filename} ]]; then 
    #     printf "Histograms already exist for run: %d\n" $1
    # elif [[ ! -f ${HIST_DIR}/${histogram_filename} ]]; then
    #     run_command="grsiframe ${analysis_tree_array[@]} $HELPER --max-workers=${MAX_WORKERS}"
    #     # echo $run_command
    #     cd $SORT_DIR
    #     grsiframe ${analysis_tree_array[@]} $HELPER --max-workers=${MAX_WORKERS}
    #     # echo mv $histogram_filename ${HIST_DIR}/${histogram_filename}
    #     mv $histogram_filename ${HIST_DIR}/${histogram_filename}
    # fi
}

#--- Main Function ---------------------------------------
TOP=$(pwd)
HIST_DIR="$TOP/data/histograms/subruns"
ANALYSIS_DIR="$TOP/data/analysis-trees"
SORT_DIR="$TOP/current-sort"
HELPER="$TOP/analysis/helpers/2019/SummingCorrectionHelper.cxx"
# HELPER="$TOP/analysis/helpers/2019/LimitCalculationHelper.cxx"
# HELPER="$TOP/analysis/helpers/2019/TimingGateOptimizationHelper.cxx"
# HELPER="$TOP/analysis/helpers/2019/FWHMMappingHelper.cxx"
# HELPER="$TOP/analysis/helpers/AngularCorrelationHelper.cxx"
MAX_WORKERS=8


SECONDS=0

if [ $# -eq 0 ]; then
    echo "Pass one argument to sort a single run or two for a range"
elif [ $# -eq 1 ]; then
    unpack_data $1
elif [ $# -eq 2 ]; then
    for ((i=$1;i<=$2;i++)); do
        unpack_data $i
    done
fi

secs=$SECONDS
printf '\nTime Elapsed: %02dh:%02dm:%02ds\n' $((secs/3600)) $((secs%3600/60)) $((secs%60))
