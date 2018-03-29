// based mostly on https://github.com/tstreble/ttH_Htautau_MEM_Analysis/blob/master/MEM/bin/test.cpp

//#include "ttH_Htautau_MEM_Analysis/MEM/interface/PyRun2EventData_t.h"
//#include "ttH_Htautau_MEM_Analysis/MEM/interface/EventReader.h"

#include "ttH_Htautau_MEM_Analysis/MEMAlgo/interface/Processes.h"
#include "ttH_Htautau_MEM_Analysis/MEMAlgo/interface/ThreadScheduler.h"
#include "ttH_Htautau_MEM_Analysis/MEMAlgo/interface/RunConfig.h"
#include "ttH_Htautau_MEM_Analysis/MEMAlgo/interface/NodeScheduler.h"

#include "ttHTauTau_MEM_Interface/MEM_Interface/interface/EventReader_MEM.h"

#include <iostream>

#include "boost/program_options.hpp"

// Root
#include "TFile.h"
#include "TTree.h"

NodeScheduler *scheduler; 

int main(const int argc, const char** argv)
{
	using namespace std;
	
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//@@@                   Configuration                                      @@@
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//

	namespace po = boost::program_options;

	string configName, inputFileName, outputName;
	int maxNbrOfEventsToRead, startEvents;
	
	po::options_description desc("Options");
	desc.add_options()
		("help,h", "produce help message")
		("input,i", po::value<string>(&inputFileName), "input ntuple name")
		("output,o", po::value<string>(&outputName)->default_value("mem_output.root"), "output name")
		("maxevents,m", po::value<int>(&maxNbrOfEventsToRead)->default_value(-1), "max number of event to process. -1 to process all events.")
		("startEvents,s", po::value<int>(&startEvents)->default_value(0), "start event index")
		("config,c", po::value<string>(&configName)->default_value("ttHTauTau_MEM_Interface/MEM_Interface/mem_cfg.py"), "config file name");

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout << desc << endl;
		return 1;
	}
	
	// Remove ".py"
	size_t pos = configName.find(".py", configName.length()-3);
	if ( pos != string::npos )
		configName.resize( configName.length()-3 );
	
	//
	const RunConfig* runConfig = new RunConfig(configName.c_str());  
	scheduler = new ThreadScheduler();

	EventReader_MEM* evtRead =
		new EventReader_MEM(inputFileName,startEvents,maxNbrOfEventsToRead,false,
							"ttHtaus/eventTree");
	
	int nEvents = evtRead->numberOfEvents();

	//eventList_t eventList;
	std::vector<IntegrationMsg_t> eventList;
	eventList.resize(nEvents);
	
	evtRead->fillEventList(&eventList[0]);

	delete evtRead;
	
	scheduler->initNodeScheduler( runConfig, 0 );
	scheduler->runNodeScheduler ( &eventList[0], nEvents );

	// Write output to tree
	TFile* f_out = new TFile(outputName.c_str(), "recreate");
	TTree* tree_mem = new TTree("mem", "mem");
	
	double Integral_ttH;
	double Integral_ttH_err;
	double Integral_ttZ;
	double Integral_ttZ_err;
	double Integral_ttZ_Zll;
	double Integral_ttZ_Zll_err;
	double Integral_ttbar;
	double Integral_ttbar_err;
	int Integration_type;
	double MEM_LR;

	// set up branches
	tree_mem->Branch("Integral_ttH", &Integral_ttH);
	tree_mem->Branch("Integral_ttH_err", &Integral_ttH_err);
	tree_mem->Branch("Integral_ttZ", &Integral_ttZ);
	tree_mem->Branch("Integral_ttZ_err", &Integral_ttZ_err);
	tree_mem->Branch("Integral_ttZ_Zll", &Integral_ttZ_Zll);
	tree_mem->Branch("Integral_ttZ_Zll_err", &Integral_ttZ_Zll_err);
	tree_mem->Branch("Integral_ttbar", &Integral_ttbar);
	tree_mem->Branch("Integral_ttbar_err", &Integral_ttbar_err);
	tree_mem->Branch("Integration_type", &Integration_type);
	tree_mem->Branch("MEM_LR", &MEM_LR);
	
	for (int event=0; event<nEvents;event++) {
		Integral_ttH = eventList[event].weight_ttH_;
		Integral_ttH_err = eventList[event].weight_error_ttH_;
		Integral_ttZ = eventList[event].weight_ttZ_;
		Integral_ttZ_err = eventList[event].weight_error_ttZ_;
		Integral_ttZ_Zll = eventList[event].weight_ttZ_Zll_;
		Integral_ttZ_Zll_err = eventList[event].weight_error_ttZ_Zll_;
		Integral_ttbar = eventList[event].weight_ttbar_DL_fakelep_;
		Integral_ttbar_err = eventList[event].weight_error_ttbar_DL_fakelep_;
		Integration_type = eventList[event].integration_type_;

	    double k_ttZ = Integration_type ? 5.e-2 : 1.e-1;
		double k_ttZll = Integration_type ? 5.e-1 : 2.e-1;
		double k_tt = Integration_type ? 5.e-15 : 1.e-18;

		MEM_LR = Integral_ttH / (Integral_ttH + k_tt*Integral_ttbar +
								 k_ttZ*Integral_ttZ + k_ttZll*Integral_ttZ_Zll);

		tree_mem->Fill();
	}

	f_out->Write();

	f_out->Close();
	
	return 0;
}
