/*
 * Histograms.h
 *
 *  Created on: 23 mar 2023
 *      Author: Khaless
 */

#ifndef HISTOGRAMS_H_
#define HISTOGRAMS_H_

#include <TH1.h>
#include <TH1D.h>

#include <TH2.h>
#include <TH2D.h>

#include <TMath.h>

using namespace TMath;

void CreateHistogams();


TH1D *h_mass;
TH1D *h_pT;

TH1D *h_eta;
TH1D *h_phi;
TH2D *h_etaphi;

TH1D *h_dphi_read;
TH1D *h_dphi_rho;
TH1D *h_dphi;

TH2D *h_dphi_acc;


void CreateHistogams()
{

	// Event

	h_mass = new TH1D("h_mass", "mass; m [GeV/c^{2}]; counts", 100, 0.0, 1.0);
	h_pT = new TH1D("h_pT", "transverse momentum; p_{T} [GeV/c]; counts", 100, 0.0, 1.0);


	h_eta = new TH1D("h_eta", "#eta; #eta; counts", 200, -2.0, 2.0);
	h_phi = new TH1D("h_phi", "#phi; #phi; counts", 50, -Pi(), Pi());
	h_etaphi = new TH2D("h_etaphi", "#eta,#phi; #eta; #phi [rad]; counts", 200, -10.0, 10.0, 50, -Pi(), Pi());

	h_dphi_read = new TH1D("h_dphi_read", "read #Delta#phi; #Delta#phi; counts", 50, -Pi(), Pi());
	h_dphi_rho = new TH1D("h_dphi_rho", "#rho #Delta#phi; #Delta#phi; counts", 50, -Pi(), Pi());
	h_dphi = new TH1D("h_dphi", "#Delta#phi; #Delta#phi; counts", 50, -Pi(), Pi());

	h_dphi_acc = new TH2D("h_dphi_acc", "#phi; #phi [rad]; accepted; counts", 50, -Pi(), Pi(), 2, 0.0, 2.0);



}


void DeleteHistogams()
{

	delete h_mass;
	delete h_pT;

	delete h_eta;
	delete h_phi;
	delete h_etaphi;

	delete h_dphi_read;
	delete h_dphi_rho;
	delete h_dphi;

	delete h_dphi_acc;


}

#endif /* HISTOGRAMS_H_ */
