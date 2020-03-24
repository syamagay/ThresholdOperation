#!/bin/bash

source func.sh

if [ $# -lt 4 ];then
    echo "input should be 4!"
    echo $@
    exit
fi

Rd53aParameterLoop=false
ParameterConfig=false
min=$1
max=$2
step=$3

json_file=$4

trap 'echo ""; echo "Stopped edit_thresholdscan by SIGINT!!"; echo "Going back to the original file."; cp ${json_file}_bak ${json_file}; exit 1' 2

indent=""
for i in `seq 0 9`
do
    indent+=" "
done

mv ${json_file} ${json_file}_bak

cat ${json_file}_bak | while IFS= read line || [ -n "$line" ]
do
    if echo "$line" | grep "Rd53aMaskLoop" >> /dev/null;then
        Rd53aParameterLoop=true
    fi
    if $Rd53aParameterLoop && echo "$line" | grep "config" >> /dev/null ;then
        ParameterConfig=true
    fi
    if $ParameterConfig && echo "$line" | grep "min" >>/dev/null;then
        echo "$indent\"min\": ${min}," >> $json_file
        diff_echo "$line" "$indent\"min\": ${min},"
    elif $ParameterConfig && echo "$line" | grep "max" >>/dev/null ;then
        echo "$indent\"max\": ${max}," >> $json_file
        diff_echo "$line" "$indent\"max\": ${max},"
    elif $ParameterConfig && echo "$line" | grep "step" >>/dev/null ;then
        echo "$indent\"step\": ${step}," >> $json_file
        diff_echo "$line" "$indent\"step\": ${step},"
        Rd53aParameterLoop=false
        ParameterConfig=false
    else
        echo "$line" >> $json_file
    fi

done
