#!/bin/bash                                                                                                                                                             
#YARR_DIR="/home/yamagaya/Desktop/Yarr-sw/LocalDB0704/src"                                                                                                              
#YARR_DIR="/home/yamagaya/Desktop/Yarr-sw/LatestYarr2/src"                                                                                        

module_name="OU078B"                      
#module_name="OU076A"                      
YARR_DIR="/home/yamagaya/Desktop/Yarr-sw/YARR_20190801"
TMP_DIR=`pwd`
SerialConsole="/home/yamagaya/Desktop/SerialConsole"
Operator_DIR="/home/yamagaya/Desktop/ScanOperator"
json_file=$YARR_DIR"/configs/rd53a/${module_name}/chipId0.json"
scan_json=$YARR_DIR"/configs/scans/rd53a/diff_thresholdscan.json"

output=$1

if [ -z $output ];then
    echo "Please specialize output directory name"
    exit
fi

output_dir=$TMP_DIR"/root/${output}"
mkdir -p $output_dir
################################                                                                                                                                        
data_dir=$YARR_DIR"/data/last_scan/"

i_candidate=(1)

i=0
#       cd $Operator_DIR                                                                                                             
target_threshold_start=2000
DiffPrecomp=400
threshold_max=400
middle_max=200
target_threshold_middle=800;

#middle_candidate=(1000 1400 1800)
middle_candidate=(800 1000 1200 1400 1600 1800)

#for i in `seq 0 100` ; do
for i in ${i_candidate[@]} ; do
    diff_min=2000
    cd $YARR_DIR
    ./configs/create_defaultconfig.sh -a rd53a -m ${module_name} -c 1
    
    now_Precomp=`cat $json_file | jq '.RD53A.GlobalConfig.DiffPrecomp'`
    sed -e "/DiffPrecomp/s/${now_Precomp}/${DiffPrecomp}/g" -i $json_file
    
    cd $TMP_DIR
    target_threshold=$((target_threshold_start - i*20))
    #target_threshold=$((target_threshold_start - i*500))
#    target_threshold=$target_threshold_start
    echo $target_threshold

    if [ $target_threshold -ge 1500 ];then
	threshold_max=400
	middle_max=400
    elif [ $target_threshold -ge 1000 ];then
	threshold_max=300
	middle_max=300
    elif [ $target_threshold -ge 800 ];then
	threshold_max=200
	middle_max=200
    elif [ $target_threshold -ge 500 ];then
	threshold_max=150
	middle_max=150
    elif [ $target_threshold -ge 250 ];then
	threshold_max=100
	middle_max=150
    else
	threshold_max=60
	middle_max=150
    fi
    ./edit_thresholdscan.sh 0 $threshold_max 5 $scan_json
    #echo "-------------------target threshold middle------------------------------"
    #echo $target_threshold_middle

    cd $YARR_DIR
    ./scripts/doScan.sh -a rd53a -m ${module_name} -s diff_thresholdscan -t $target_threshold >> ${output_dir}/gayaLog${i}.log 2>&1
    root -l -q read_dat.cpp+'("'$data_dir'","'$output_dir"_via800"'",'$i','$target_threshold')'
    via_path=`readlink -f data/last_scan`

    ./scripts/doScan.sh -a rd53a -m ${module_name} -s diff_tune_globalthreshold_selected -t $target_threshold >> ${output_dir}/gayaLog${i}.log 2>&1
    ./scripts/doScan.sh -a rd53a -m ${module_name} -s diff_tune_pixelthreshold_selected -t $target_threshold >> ${output_dir}/gayaLog${i}.log 2>&1

    ./scripts/doScan.sh -a rd53a -m ${module_name} -s diff_analogscan -t $target_threshold >> ${output_dir}/gayaLog${i}.log 2>&1
    ./scripts/doScan.sh -a rd53a -m ${module_name} -s diff_thresholdscan -t $target_threshold >> ${output_dir}/gayaLog${i}.log 2>&1
    
    cd $TMP_DIR
    root -l -q read_dat_via.cpp+'("'$data_dir'","'$via_path'","'$output_dir'",'$i','$target_threshold')'

<<EOF


    cd $TMP_DIR
    ./edit_thresholdscan.sh 0 $threshold_max 5 $scan_json
    
    cd $YARR_DIR
    ./scripts/doScan.sh -a rd53a -m ${module_name} -s diff_retune_globalthreshold -t $target_threshold >> ${output}/gayaLog${i}.log 2>&1
    ./scripts/doScan.sh -a rd53a -m ${module_name} -s diff_retune_pixelthreshold -t $target_threshold >> ${output}/gayaLog${i}.log 2>&1


EOF
done
