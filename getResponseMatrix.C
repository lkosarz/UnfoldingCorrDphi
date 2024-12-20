#include <exception>
#include <assert.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>
#include <fstream>

#include "TChain.h"
#include "TString.h"
#include "TClonesArray.h"
#include "TRefArray.h"
#include "TRef.h"
#include "TFile.h"
#include "TArrayI.h"
#include "TTree.h"
#include "TF1.h"
#include "TH1.h"
#include "TH1I.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3D.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TBranch.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TPaveLabel.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TMath.h"
#include "TProfile.h"
#include "TCut.h"
#include "TGraphAsymmErrors.h"
#include "TFitResult.h"

#include "RooUnfold/src/RooUnfoldResponse.h"
#include "RooUnfold/src/RooUnfoldBayes.h"
//#include "RooUnfold/src/RooUnfoldSvd.h"
#include "RooUnfold/src/RooUnfoldTUnfold.h"


#include "style.C"

#include "FileList.h"
//#include "Event.h"

using namespace std;

//#pragma link C++ class Event+;

int MakeEvent(TChain *store, long ev);

bool debug = false;
bool printEvNum = true;


void getResponseMatrix(TString inputName = "data/file.list",
		TString outFileName = "outputResponse.root", long nevents = 100000);

/*
int main(int argc, char **argv)
{
	getResponseMatrix();

	return 1;
}*/

/////////////////////////////////////////////////////////////
void getResponseMatrix(TString inputName, TString outFileName, long nevents) {

	style();
	//gStyle->SetOptStat(0);
	gROOT->ForceStyle();

	//list = "data/AuAu200_P16id_SL18f_noRefMultBias_0mult10_full.root"
	gSystem->Load("RooUnfold/libRooUnfold.so");

	Int_t padw = 750, padh = 500;

	const Int_t nPadX = 2;
	const Int_t nPadY = 2;

	const Int_t nFiles = 1;

	//const Int_t nHist = 4;

	const int nOrder = 4;

	//TFile *inFile;
	//inFile = new TFile(inputName, "read");


	TChain *chain = new TChain("eventNtuple");
	//TChain *chain = new TChain();

	//if(!openFileList(chain, list)) return 0;
	std::vector<std::string> filenames = openList(inputName);

	for (int i = 0; i < filenames.size(); ++i) {
		//chain->Add(new TFile(filenames.at(i).data(), "read"));
		chain->Add(filenames.at(i).data());
	}

	//Event *eventRead = NULL;
	//Event eventRead;

	//chain->SetBranchAddress("eventNtuple", &eventRead);

    cout<<"Chain created"<<endl;

	TTree *tree = (TTree*)chain;
	tree->Print("toponly");

	float mass;
	float pt;
	float eta;
	float phi;
	float deltaPhi;
	float accepted;

    cout<<"Setting branches"<<endl;

	tree->SetBranchAddress("mass", &mass);
	tree->SetBranchAddress("pt", &pt);
	tree->SetBranchAddress("eta", &eta);
	tree->SetBranchAddress("phi", &phi);
	tree->SetBranchAddress("deltaPhi", &deltaPhi);
	tree->SetBranchAddress("accepted", &accepted);

    cout<<"Branches set"<<endl;
/*
	tree->SetBranchAddress("mass", &eventRead.mass);
	tree->SetBranchAddress("pt", &eventRead.pt);
	tree->SetBranchAddress("eta", &eventRead.eta);
	tree->SetBranchAddress("phi", &eventRead.phi);
	tree->SetBranchAddress("deltaPhi", &eventRead.deltaPhi);
	tree->SetBranchAddress("accepted", &eventRead.accepted);
*/
	//-----------

	long nEvents = tree->GetEntries();
	cout<<"Number of events = "<<nEvents<<endl;

	if(nevents>0 && nEvents>nevents) nEvents = nevents;


	TString names_3D_US[nOrder] = {
			"h_Mass_Pt_cos1dPhi_US",
			"h_Mass_Pt_cos2dPhi_US",
			"h_Mass_Pt_cos3dPhi_US",
			"h_Mass_Pt_cos4dPhi_US"
	};


	const int ny = 15;
	double *rebinArrY = new double[ny];
	double ArrayY[] = {0.0, 0.02, 0.04, 0.06, 0.08, 0.1,
			0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0
	};
	rebinArrY = ArrayY;


	double mass_hi = 0.85;
	double mass_lo = 0.65;

	//--------------------------------------------


	TString mkdir = "output";
	gSystem->MakeDirectory(mkdir.Data());
	gSystem->cd(mkdir);
	//TString mkdir2 = "output";
	//gSystem->MakeDirectory(mkdir2.Data());
	//gSystem->cd(mkdir2);

	TFile *fileOut;
	fileOut = new TFile(outFileName, "recreate");
	fileOut->cd();
	//gSystem->cd("../");
	gSystem->cd("../");

	cout<<"Outfile created!"<<endl;

	//---------------------------------------------------

	//HarmonicData *dataContainer = new HarmonicData();
	//dataContainer->ReadUS(names_3D_US, inFile);



	TH1D *h_dPhiMeas = new TH1D("h_dPhiMeas", "#Delta#phi measured", 50, -TMath::Pi(), TMath::Pi());
	TH1D *h_dPhiTrue = new TH1D("h_dPhiTrue", "#Delta#phi measured", 50, -TMath::Pi(), TMath::Pi());

	TH1D **h_cosNdPhiMeas;
	TH1D **h_cosNdPhiTrue;

	h_cosNdPhiMeas = new TH1D*[nOrder];
	h_cosNdPhiTrue = new TH1D*[nOrder];

/*
	RooUnfoldResponse *response_dphi = new RooUnfoldResponse(h_dPhiMeas, h_dPhiTrue);

	RooUnfoldResponse **response_cosNdPhi;
	response_cosNdPhi = new RooUnfoldResponse*[nOrder];
*/
	for (int i = 0; i < nOrder; ++i) {

		h_cosNdPhiMeas[i] = new TH1D(Form("h_cos%idPhiMeas", i), "cos(#Delta#phi) measured", 100, -1.0, 1.0);
		h_cosNdPhiTrue[i] = new TH1D(Form("h_cos%idPhiTrue", i), "cos(#Delta#phi) true", 100, -1.0, 1.0);

		//response_cosNdPhi[i] = new RooUnfoldResponse(h_cosNdPhiMeas[i], h_cosNdPhiTrue[i]);

	}


	for (int ev = 0; ev < nEvents; ++ev) {

		if(printEvNum) std::cout<<"reading event "<<ev<<std::endl;

		//MakeEvent(chain, ev); // execute for each event
		chain->GetEntry(ev);

		// select rho
		if(pt >= 0.065) continue;
		if(mass <= 0.7) continue;
		if(pt >= 0.9) continue;

		// fill true

		h_dPhiTrue->Fill(deltaPhi);
/*
		double cos1dPhi = TMath::Cos(deltaPhi);
		double cos2dPhi = TMath::Cos(2.0*deltaPhi);
		double cos3dPhi = TMath::Cos(3.0*deltaPhi);
		double cos4dPhi = TMath::Cos(4.0*deltaPhi);*/


		for (int i = 0; i < nOrder; ++i) {

			h_cosNdPhiTrue[i]->Fill(TMath::Cos((i+1.0)*deltaPhi));

		}

		cout<<"mass = "<<mass<<endl;
		cout<<"pt = "<<pt<<endl;
		cout<<"eta = "<<eta<<endl;
		cout<<"phi = "<<phi<<endl;
		cout<<"deltaPhi = "<<deltaPhi<<endl;
		cout<<"accepted = "<<accepted<<endl;

		if(accepted == 1)
		{

			// fill measured

			h_dPhiMeas->Fill(deltaPhi);

			for (int i = 0; i < nOrder; ++i) {

				h_cosNdPhiMeas[i]->Fill(TMath::Cos((i+1.0)*deltaPhi));

			}

		} // accepted pair



		if(debug) std::cout<<"End of event"<<std::endl;

	} // events


	//-------------


	//---------------------------------------------------


	// Corrections
/*
	TCanvas *cnv_DistCorr_2D = new TCanvas("cnv_DistCorr_2D", "Distortion corrected cos(ndPhi) vs. pT 2D", nPadX*padw, nPadY*padh);

	cnv_DistCorr_2D->Divide(nPadX,nPadY);


	for (int i = 0; i < nOrder; ++i) {

		//cnv_DistCorr_2D->cd(i+1);
	}*/






	//--------------------------------------------

	cout << "Writing!" << endl;

	gSystem->cd(mkdir);
	//gSystem->cd(mkdir2);

	//sigFracContainer->Save(Form("SigFrac_%s.root", outFileName.Data()));
	//cout<<"Signal fraction histograms saved!"<<endl;



	//cnv_SigFrac->cd(1)->SaveAs("h_pT_US.png");

	for (int i = 0; i < nOrder; ++i) {
		// 2D
		//cnv_DistCorr_2D->cd(i+1)->SaveAs(Form("%s.png", CosNdPhi_DistCorr_2D[i]->GetName()));
		//cnv_FullCorr_2D->cd(i+1)->SaveAs(Form("%s.png", CosNdPhi_Corr_2D[i]->GetName()));
	}


	gSystem->cd("../");
	//gSystem->cd("../");

	fileOut->cd();
	fileOut->Write();



}


/*
int MakeEvent(TChain *store, long ev)
{



}*/
