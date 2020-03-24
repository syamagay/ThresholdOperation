#!/bin/bash

source func.sh


if [ $# -lt 2 ];then
    echo "input should be 2!"
    echo $@
    exit
fi

Rd53aTriggerLoop=false
TriggerConfig=false
count=$1

json_file=$2
echo $json_file
trap 'echo ""; echo "Stopped edit_triggerLoop by SIGINT!!"; echo "Going back to the original file."; cp ${json_file}_bak ${json_file}; exit 1' 2

indent=""
i=0
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

    
