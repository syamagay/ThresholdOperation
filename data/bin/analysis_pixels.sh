#!/bin/bash

directory=$1
if [ ! -e $directory ];then
    echo "$directory is not exist!"
    exit 1
fi

#directory="../"$directory
./analysis.sh $directory

root src/analysis_pixels.cpp+'("'$directory'")'
