#!/bin/bash

function diff_echo(){
    before=$1
    after=$2

    echo ""
    echo "${before}"
    echo "--------------------------------------------"
    echo "${after}"
}

function edit_thresholdscan(){
    if [ $# -lt 4 ];then
	echo "input should be 4!"
	echo $@
	exit
    fi
    
    Rd53aParameterLoop=false
    ParameterConfig=false
    local min=$1
    local max=$2
    local step=$3

    local json_file=$4

    trap 'echo ""; echo "Stopped edit_thresholdscan by SIGINT!!"; echo "Going back to the original file."; cp ${json_file}_bak ${json_file}; exit 1' 2
    
    local indent=""
    local i=0
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
}
function edit_triggerLoop(){
    if [ $# -lt 2 ];then
	echo "input should be 2!"
	echo $@
	exit
    fi

    Rd53aTriggerLoop=false
    TriggerConfig=false
    local count=$1

    local json_file=$2
    echo $json_file
    trap 'echo ""; echo "Stopped edit_triggerLoop by SIGINT!!"; echo "Going back to the original file."; cp ${json_file}_bak ${json_file}; exit 1' 2
    
    local indent=""
    local i=0
    for i in `seq 0 9`
    do
	indent+=" "
    done

    mv ${json_file} ${json_file}_bak

    cat ${json_file}_bak | while IFS= read line || [ -n "$line" ]
    do
	if echo "$line" | grep "Rd53aCoreColLoop" >> /dev/null;then
	    Rd53aTriggerLoop=true
	fi
	if $Rd53aTriggerLoop && echo "$line" | grep "config" >> /dev/null ;then
	    TriggerConfig=true
	fi 
	if $TriggerConfig && echo "$line" | grep "count" >>/dev/null;then
	    echo "$indent\"count\": ${count}," >> $json_file
	    diff_echo "$line" "$indent\"count\": ${count},"
	    Rd53aTriggerLoop=false
	    TriggerConfig=false
	else
	    echo "$line" >> $json_file
	fi
	
    done
}
