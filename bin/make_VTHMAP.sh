#!/bin/bash

tmp=$(cd $(dirname $0); cd .. ;  pwd)
cd $tmp

module_name="OU078B"

YARR_DIR=$tmp"/YARR/"
TMP_DIR=$tmp
json_file=$YARR_DIR"/configs/rd53a/${module_name}/chipId0.json"
scan_json=$YARR_DIR"/configs/scans/rd53a/diff_thresholdscan.json"
#######
VTH1_change=false
VTH2_change=false
DiffPrecomp_change=false
TDAC_change=false
RUN_SCAN=false
#######

while getopts o:fsctr OPT
do
    case $OPT in
	o) output=${OPTARG} ;;
	f) VTH1_change=true ; VTH1_finish=false ;;
	s) VTH2_change=true ; VTH2_finish=false ;;
	c) DiffPrecomp_change=true ; DiffPrecomp_finish=false ;;
	t) TDAC_change=true ; TDAC_finish=false ;;
	r) RUN_SCAN=true ;;
    esac
done

function Usage(){
    cat <<EOF
$0 <o:output_dirctory_name> <f: VTH1 change> <s: VTH2 change> <c: DiffPrecomp change> <t TDAC change>
EOF
    exit
}

## default #########
VTH1_start=500
VTH2_start=500
DiffPrecomp_start=550
TDAC_start=-15;
i=0

VTH1=$VTH1_start
VTH2=$VTH2_start
DiffPrecomp=$DiffPrecomp_start
TDAC=$TDAC_start

VTH1_goal=0
VTH2_goal=0
DiffPrecomp_goal=50
TDAC_goal=0
####################

#### now runnumber #############
run_start=20267
################################

#### output ####################
#output=$1
if [ -z $output ];then
    Usage
fi

output_dir=$TMP_DIR"/data/${output}"
mkdir -p $output_dir
################################
data_dir=$YARR_DIR"/data/last_scan/"

######default setting###################
now_VTH1=`cat $json_file | jq '.RD53A.GlobalConfig.DiffVth1'`
sed -e "/DiffVth1/s/${now_VTH1}/${VTH1}/g" -i $json_file
now_VTH2=`cat $json_file | jq '.RD53A.GlobalConfig.DiffVth2'`
sed -e "/DiffVth2/s/${now_VTH2}/${VTH2}/g" -i $json_file
now_Precomp=`cat $json_file | jq '.RD53A.GlobalConfig.DiffPrecomp'`
sed -e "/DiffPrecomp/s/${now_Precomp}/${DiffPrecomp}/g" -i $json_file    

if $RUN_SCAN ; then
    echo "reseting TDAC ......"
#    ./modifyConfig.sh $json_file TDAC $TDAC_start diff >> /dev/null
    ./modifyConfig.sh $json_file TDAC $TDAC_start diff >> /dev/null
    echo "finish reseting TDAC !"
fi
########################################

if $RUN_SCAN;then
    cd $Operator_DIR
#    ./script/call_turn_on.sh -L
    cd $TMP_DIR
    cd $YARR_DIR
    #./doScan.sh -a rd53a -m OU076A -s diff_tune_pixelthreshold -j
    cd $TMP_DIR
fi

#for i in `seq 0 40`
while $VTH1_change || $VTH2_change || $DiffPrecomp_change || $TDAC_change
do
    echo $i
########### For files already existed #####################
    runId=`expr ${run_start} + $i`
#    data_dir=$YARR_DIR"/data/0"$runId"_diff_thresholdscan/"
###########################################################
    if $DiffPrecomp_change && ! $VTH1_change ;then
#    if $DiffPrecomp_change && ! $TDAC_change ;then
	echo $DiffPrecomp_change
	now_Precomp=`cat $json_file | jq '.RD53A.GlobalConfig.DiffPrecomp'`
	sed -e "/DiffPrecomp/s/${now_Precomp}/${DiffPrecomp}/g" -i $json_file    
	echo "DiffPrecomp = "$now_Precomp" -> "$DiffPrecomp
	if [ $DiffPrecomp -le $DiffPrecomp_goal ]; then
	    DiffPrecomp_change=false
	else
#	    TDAC=$TDAC_start
#	    TDAC_change=true
	    VTH1=$VTH1_start
	   # VTH2=$VTH2_start
	    VTH1_change=true
	   # VTH2_change=true
	    DiffPrecomp=`expr $DiffPrecomp - 150`
	fi
    fi


#    if $VTH1_change ;then
    if $VTH1_change && ! $VTH2_change ;then
	now_VTH1=`cat $json_file | jq '.RD53A.GlobalConfig.DiffVth1'`
	sed -e "/DiffVth1/s/${now_VTH1}/${VTH1}/g" -i $json_file
	echo "VTH1 = "$now_VTH1" -> "$VTH1
	if [ $VTH1 -le $VTH1_goal ]; then
	    VTH1_change=false
	else
	    VTH2_change=true
	    VTH2=$VTH2_start
	    VTH1=`expr $VTH1 - 25`
	fi
    fi
    if $VTH2_change ; then
	now_VTH2=`cat $json_file | jq '.RD53A.GlobalConfig.DiffVth2'`
	sed -e "/DiffVth2/s/${now_VTH2}/${VTH2}/g" -i $json_file
	echo "VTH2 = "$now_VTH2" -> "$VTH2
	if [ $VTH2 -le $VTH2_goal ]; then
	    VTH2_change=false
	else
	    VTH2=`expr $VTH2 - 25`	    
	fi
    fi
    if $TDAC_change ;then
	if $RUN_SCAN;then
	./modifyConfig.sh $YARR_DIR/configs/rd53a/OU076A/chipId0.json TDAC $TDAC diff >> /dev/null
	fi
	echo "TDAC -> "$TDAC
	if [ $TDAC -le $TDAC_goal ]; then
	    TDAC_change=false
	else
	    TDAC=`expr $TDAC - 1`	    
	fi

    fi
    if $RUN_SCAN ; then
	cd $YARR_DIR
#	cd $Operator_DIR
	pwd
	./scripts/doScan.sh -a rd53a -m OU076A -s diff_thresholdscan
#	./doScan.sh -a rd53a -m OU076A -s diff_thresholdscan_high
#	./Single_Operator.sh -m OU076A -s diff_thresholdscan
	cd $TMP_DIR
	root -l -q read_dat.cpp+'("'$data_dir'","'$output_dir'",'$i')'
	if [ $? -eq 1 ];then
	    continue
	fi
    fi

#    pwd
    i=$((i+1))
done
if $RUN_SCAN ; then
    cd $Operator_DIR
    pwd
#    ./script/call_turn_off.sh -L
fi
