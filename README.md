# ttHTauTau_MEM_Interface

A supporting package for running Matrix Element Method computation on ntuples produced by ttHTauTauAnalysis package

## Installation

Setup CMSSW environment:
	  
	cmsrel CMSSW_9_4_4
	cd CMSSW_9_4_4/src/
	cmsenv

Get MEM package from LLR for ttH, H->tautau analysis:
	
	git clone https://github.com/tstreble/ttH_Htautau_MEM_Analysis.git

Get ttHTauTauAnalysis analyzer:

	git clone https://github.com/zctao/ttHTauTauAnalysis.git
	cd ttHTauTauAnalysis/
	git checkout cmssw_9_4_x
	cd -

Get interface package:

	git clone https://github.com/zctao/ttHTauTau_MEM_Interface.git

Compile:

	scram b -j 8

## Usage

	runMEM -i <input.root> -o <output.root> -m <maxevents> -s <startevent> -c <config>
	runMEM -h for help

Input ntuple is expected to be of the format defined in ttHTauTauAnalysis/ttHtautauAnalyzer/interface/eventNtuple.h
'mem_output.root' is the default output file.

Three config files are provided under python directory: 'mem_cfg_test.py', 'mem_cfg_lowpints.py', 'mem_cfg_nominal.py'. By default 'mem_cfg_test.py' is used. The other two with larger number of integration points take significantly longer time to run. 

An example input file 'nutple_ttH.root' and a script running on this file are provided in the test directory.
