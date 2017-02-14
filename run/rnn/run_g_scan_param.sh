#!/bin/bash

SCRIPT_PATH=${0%/*}
CONFIG_FILE=$SCRIPT_PATH/../reshaper_0.config
DATE_LIST=$SCRIPT_PATH/comb.txt
LOG_FILE=$SCRIPT_PATH/scan_param.log

rm -f $LOG_FILE

for b_th in     `$SCRIPT_PATH/gen_range.sh 0.9  0.1  1.2 `; do
    for s_th in `$SCRIPT_PATH/gen_range.sh 0.5  0.1  0.7 `; do
        #if ([ `echo "$b_th != 1.2" | bc` == 1 ] && [ `echo "$b_th != 1.3" | bc` == 1 ]) || ([ `echo "$s_th != 0.6" | bc` == 1 ] && [ `echo "$s_th != 0.7" | bc` == 1 ]); then
        rm -f $CONFIG_FILE
        touch $CONFIG_FILE
        
        echo B_TH=$b_th >> $CONFIG_FILE
        echo S_TH=$s_th >> $CONFIG_FILE
        
        echo ----------------                 | tee -a $LOG_FILE
        date                                  | tee -a $LOG_FILE
        echo B_TH=$b_th                       | tee -a $LOG_FILE
        echo S_TH=$s_th                       | tee -a $LOG_FILE
        $SCRIPT_PATH/run_g_list.sh $DATE_LIST | tee -a $LOG_FILE
        #fi
    done
done