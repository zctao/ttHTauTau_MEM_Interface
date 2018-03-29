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
	cd ttHTauTau_MEM_Interface/
	git checkout cmssw_9_4_x
	cd -

Compile:

	scram b -j 8

## Usage

	runMEM -i <input.root> -o <output.root> -m <maxevents> -s <startevent> -c <config.py>

Input ntuple is expected to be of the format defined in ttHTauTauAnalysis/ttHtautauAnalyzer/interface/eventNtuple.h

An example input file 'nutple_ttH.root' and a script running on this file are provided in the test directory. By default, it uses MEM config file 'mem_cfg.py' under python directory and starts from event #0. 'mem_output.root' is the default output file.
