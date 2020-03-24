#!/bin/bash
set -e

function usage(){
    cat <<EOF
$0 <configfile_name> <target_parName> <value> <analogFE_type:syn/diff/lin/all>
EOF
exit 1
}

if [ $# -lt 4 ];then
    usage
fi

origfilename="${1}"
backupfilename="${1}_bak"
cp -p ${origfilename} ${backupfilename}
rm -f ${origfilename}
trap 'echo ""; echo "Stopped by SIGINT!!"; echo "Going back to the original file."; cp ${backupfilename} ${origfilename}; exit 1' 2

#sleep 1000
#ls exit

target=${2}
value=${3}
afterTarget="no"
if [ -z ${4} ]; then
    afetype="all"
else
    afetype=${4}
fi

#Note: FEI4 pixel config has list of values of each row.
#      RD53A pixel config has list of values of each column.
if head -n 2 ${backupfilename} | grep "FE-I4B" > /dev/null; then
    nelement=80
    asictype="FE-I4B"
    cfglisttype="Row"
    #For special mask (only valid for other than TDAC/FDAC)
    minrow=1
    maxrow=336
    mincol=1
    maxcol=80
elif head -n 2 ${backupfilename} | grep "RD53A" > /dev/null; then
    nelement=192
    asictype="RD53A"
    cfglisttype="Col"
    #For special mask (only valid for other than TDAC/FDAC)
    minrow=1 #1
    maxrow=192 #192
    if [ ${afetype} == "sync" ]; then
        mincol=1
        maxcol=128
    elif [ ${afetype} == "lin" ]; then
        mincol=129
        maxcol=264
    elif [ ${afetype} == "diff" ]; then
        mincol=265
        maxcol=400
    else
        mincol=1 #SYNC:1-128, LIN:129-264, DIFF:265-400
        maxcol=400
    fi
fi

#Forming a line for a new config
if [ ${asictype} == "FE-I4B" ]; then
    min=${mincol}
    max=${maxcol}
elif [ ${asictype} == "RD53A" ]; then
    min=${minrow}
    max=${maxrow}
fi
newconfig=""
for i in `seq 0 11`
do
newconfig+=" "
done
newconfig+="\"${target}\": ["
for icnt in `seq 0 $((${nelement}-1))`; do
    if [ ${target} != "TDAC" -a  ${target} != "FDAC" ]; then
        if [ $((${icnt}+1)) -ge `expr ${min}` -a $((${icnt}+1)) -le `expr ${max}` ]; then
            newconfig+="$value"
        else
            if [ ${value} == "1" ]; then
                newconfig+="0"
            else
                newconfig+="1"
            fi
        fi
    else
        newconfig+=${value}
    fi
    if [ ${icnt} == $((${nelement}-1)) ]; then
        newconfig+="]"
	if [ ${target} != "TDAC" ];then
	    newconfig+=","
	fi

    else
        newconfig+=","

    fi
done
echo ${newconfig}

#Loop over lines in file
if [ ${asictype} == "FE-I4B" ]; then
    min=${minrow}
    max=${maxrow}
elif [ ${asictype} == "RD53A" ]; then
    min=${mincol}
    max=${maxcol}
fi
ntarget=0
cat ${backupfilename} | while IFS= read line || [ -n "${line}" ]
do
#    echo "${line}" #Do not use echo ${line}!! It will ignore white-spaces in front of line.
    if [ ${afterTarget} == "no" ]; then
        if echo ${line} | grep "\"${target}\"" > /dev/null; then
	    ntarget=$((ntarget + 1))
            if [ ${ntarget} -ge `expr ${min}` -a ${ntarget} -le `expr ${max}` ]; then
                afterTarget="yes"
                echo "${cfglisttype}: ${ntarget}"
		echo "Substituting"
		echo ${line}
		echo "for"
		echo ${newconfig}
		echo ""
		afterTarget="no"
		#Output to a new config file
		echo "${newconfig}" >> ${origfilename}
	    else
		#Output to a new config file
		echo "${line}" >> ${origfilename} 
	    fi
        else
	    #Output to a new config file
	    echo "${line}" >> ${origfilename} 
        fi
    fi
done

echo "Target pixel config: ${target}"
echo "Set values to ${value}"
echo "${origfilename} was modified."
echo "${backupfilename} was created."
