#!/bin/bash

tmp=$(cd $(dirname $0); cd .. ; pwd)

DIR=$1
if [ ! -e $DIR ];then
    echo "selected directory = ${DIR} is not exist!!"
    exit 1
fi

file_list=(`ls ${DIR}/gaya*.root`)
file_num=${#file_list[@]}


cd $DIR
filename=gaya0.root
i=0
while [ -e $filename ]
do
   i=$((i+1))
   filename="gaya"$i".root"
done
cd ..
#echo $i

root -q -l tools/analysis.cpp+'("'$DIR'",'$file_num')'
#root -q -l src/analysis.cpp+'("'$DIR'",'$i')'
