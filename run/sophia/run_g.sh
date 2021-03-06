#!/bin/bash

SCRIPT_PATH=${0%/*}
BIN_PATH=$SCRIPT_PATH/../../bin
RUN_PATH=$SCRIPT_PATH/../../run
SOPHIA_PY=$SCRIPT_PATH/../../../Sophia/src/sophia.py

ZIP_ROOT=~/MATLAB/Data
ZIP_DATA=$ZIP_ROOT/$1.zip

TEMP_ROOT=/tmp/Sibyl
TEMP_DATA_PATH=$TEMP_ROOT/Data/$1

DEV="device=cuda0"
FLAGS="floatX=float32,"$DEV",gpuarray.preallocate=1,base_compiledir=theano,print_active_device=False"

TCP_ADDRESS=127.0.0.1
TCP_PORT=50505

if [ -f $ZIP_DATA ] ; then
    rm -rf $TEMP_ROOT
    
    mkdir -p $TEMP_DATA_PATH
    unzip -qq -d $TEMP_DATA_PATH $ZIP_DATA
    
    #printf '%s\t' $1
    
    THEANO_FLAGS=$FLAGS python -u $SOPHIA_PY &
    
    $BIN_PATH/simserv $RUN_PATH/KOSPI.config $TEMP_DATA_PATH $TCP_PORT &
    
    sleep 1
    
    $BIN_PATH/sophia $RUN_PATH/reward.config $RUN_PATH/reshaper_0.config $SCRIPT_PATH/workspace.list $TCP_ADDRESS $TCP_PORT 
    
    rm -rf $TEMP_ROOT
fi
