#!/bin/bash

maxevents=${1}
startevent=${2:-0}
input=${3:-'ntuple_ttH.root'}
output=${4:-'mem_output.root'}
config=${5:-'ttHTauTau_MEM_Interface/MEM_Interface/mem_cfg.py'}

runMEM $maxevents $startevent $input $output $config 
