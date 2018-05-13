
#include "SteppingAction.hh"
#include "DetectorConstructionDet.hh"
#include "DetectorHit.hh"
#include "DetectorSD.hh"

#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"    
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"

#include "G4Scintillation.hh"

#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TLegend.h>

#include <iostream>
#include <sstream>

using namespace std;


  /*
void
LayerInfo::Draw(TCanvas* canvas)
{
  canvas->Divide(3);
  
  map<string, TH1D*>::iterator it1;
  map<string, TH1D*>::iterator it2;
  map<string, TH1D*>::iterator it3;
  
  TLegend* leg = new TLegend(0.6,0.6,0.8,0.99,0,"brNDC");
  
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  
  
  double maxE = 0;
  double maxA = 0;
  double maxP = 0;
  for (it1=fHistE.begin(),it2=fHistA.begin(),it3=fHistP.begin(); it1!=fHistE.end(); it1++, it2++, it3++) {
    maxE = max(maxE, it1->second->GetMaximum());
    maxA = max(maxA, it2->second->GetMaximum());
    maxP = max(maxA, it3->second->GetMaximum());
  }
  
  bool first = true;
  int color = 1;
  for (it1=fHistE.begin(),it2=fHistA.begin(),it3=fHistP.begin(); it1!=fHistE.end(); it1++, it2++,it3++) {
    ostringstream pname;
    pname << it1->first;
    
    canvas->cd(1);
    it1->second->SetMaximum(maxE);
    it1->second->SetLineColor(color);
    it1->second->SetLineWidth(3);
    it1->second->Draw( (first? "":"same") );
    //out->Write(it1->second);
    leg->AddEntry(it1->second, pname.str().c_str(), "l");
    
    canvas->cd(2);
    it2->second->SetMaximum(maxA);
    it2->second->SetLineColor(color);
    it2->second->SetLineWidth(3);
    it2->second->Draw( (first? "":"same") );
    //out->Write(it2->second);

    canvas->cd(3);
    it3->second->SetMaximum(maxP);
    it3->second->SetLineColor(color);
    it3->second->SetLineWidth(3);
    it3->second->Draw( (first? "":"same") );

    color ++;
    first = false;
  }
  
  canvas->cd(1);
  leg->Draw();
  canvas->cd(2);
  leg->Draw();
}
  */


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
SteppingAction::SteppingAction(DetectorConstructionDet* det)
  : G4UserSteppingAction(), fDet(det)
{
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
SteppingAction::~SteppingAction()
{
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  
  ostringstream cname1, cname2, cname3;
  cname1 << "energy_spectrum.pdf";
  cname2 << "zenith_distribution.pdf";
  
  // TFile* out = TFile::Open("output.root");
  TCanvas* canvas1 = new TCanvas("energy_spectrum");
  canvas1->SetLogy(1);
  
  TCanvas* canvas2 = new TCanvas("zenith_spectrum");
  canvas2->SetLogy(1);

  TLegend* leg = new TLegend(0.6,0.6,0.8,0.99,0,"brNDC");  
  leg->SetBorderSize(0);
  leg->SetFillColor(0);

  map<string, TH1D*>::iterator it1;
  map<string, TH1D*>::iterator it2;

  //double flux = 1./(200./second/meter/meter /* Hz/m2 */ * 3.141*pow(80*meter * sin(70*degree),2)) ;
  double flux = 200. / 1000;//1./(200./second/meter/meter);

  cout << "flux " << flux << endl;
  
  double maxE = 0;
  double maxZ = 0;
  for (it1=fHistE.begin(),it2=fHistZ.begin(); it1!=fHistE.end(); it1++, it2++) {
    it1->second->Scale(flux);
    it2->second->Scale(flux);
    maxE = max(maxE, it1->second->GetMaximum());
    maxZ = max(maxZ, it2->second->GetMaximum());
  }

  bool first = true;
  int color = 1;

  TFile* fOut = TFile::Open("output.root", "update");
  fOut->cd();
  
  for (it1=fHistE.begin(),it2=fHistZ.begin(); it1!=fHistE.end(); it1++, it2++) {

    ostringstream pname;
    pname << it1->first;
    
    canvas1->cd();
    it1->second->SetMaximum(maxE);
    it1->second->SetLineColor(color);
    it1->second->SetLineWidth(3);
    it1->second->SetYTitle("Events / s / m^{2}");
    it1->second->Draw( (first? "":"same") );
    it1->second->Write();
    //out->Write(it1->second);
    leg->AddEntry(it1->second, pname.str().c_str(), "l");

    canvas2->cd();
    it2->second->SetMaximum(maxZ);
    it2->second->SetLineColor(color);
    it2->second->SetLineWidth(3);    
    it2->second->SetYTitle("Events / s / m^{2}");
    it2->second->Draw( (first? "":"same") );
    it2->second->Write();

    color++;
    first = false;    
  }

  fOut->Flush();
  
  canvas1->cd();
  leg->Draw();
  canvas1->SaveAs(cname1.str().c_str());
  canvas2->SaveAs(cname2.str().c_str());

  fOut->Close();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void
SteppingAction::UserSteppingAction(const G4Step * theStep)
{
  // check if it is alive
  G4Track * theTrack = theStep->GetTrack();

  bool countSciPh = false;
  if (countSciPh) {
    
    // count PH production !!!
    if (dynamic_cast<const G4Scintillation*>(theTrack->GetCreatorProcess()) != 0) {
      //if (dynamic_cast<const G4Scintillation*>(theStep->GetPreStepPoint()->GetProcessDefinedStep())) {
    
      G4TouchableHistory* touchable
	= (G4TouchableHistory*)(theStep->GetPreStepPoint()->GetTouchable());
      
      // Get calorimeter cell id
      const int id1 = 0;
      G4int layerNumber0 = touchable->GetReplicaNumber(id1+1);
      G4int layerNumber1= touchable->GetReplicaNumber(id1+2);
      G4int layerNumber2 = touchable->GetReplicaNumber(id1+3);
      G4int layerNumber3 = touchable->GetReplicaNumber(id1+4);
      
      int fNGrid = 15;
      int layerNumber = layerNumber3 * fNGrid*fNGrid + layerNumber2 * fNGrid + layerNumber1;
      
      //if (layerNumber>300) {
      // cout << "layerNumber " << layerNumber0 << " " << layerNumber1 << " "<< layerNumber2 << " " << layerNumber3 << " " << layerNumber << endl;
      //    cout << touchable->GetVolume()->
      //}
      
      
      static DetectorHitsCollection* sciHC = 0;
      if (!sciHC) {
	sciHC = fDet->GetCells()->GetCollection();
      }
      
      if (sciHC>0) {
	
	// Get hit accounting data for this cell
	DetectorHit* hit = (*sciHC)[layerNumber];
	if ( ! hit ) {
	  G4ExceptionDescription msg;
	  msg << "Cannot access hit " << layerNumber; 
	  G4Exception("DetectorSD::ProcessHits()",
		      "MyCode0004", FatalException, msg);
	}
	
	// Get hit for total accounting
	DetectorHit* hitTotal = (*sciHC)[sciHC->entries()-1];
	
	hit->AddPh(1.);
	hitTotal->AddPh(1.);
      }
    } // END OPTICAL PHOTON ------------------------------------------------------------
  } // count sciphot
  
  bool noFluxCalc = true;
  if (noFluxCalc) {
    return;
  }
  

  if (theTrack->GetTrackStatus()!=fAlive) {
    return;
  }

  
  
  const double zpre = theStep->GetPreStepPoint()->GetPosition().z();
  const double zpos = theStep->GetPostStepPoint()->GetPosition().z();
  
  const double zSave = 0 * meter;

  const double energy = theStep->GetPostStepPoint()->GetTotalEnergy();  
  
  
  // cout << "step " << abs(theStep->GetTrack()->GetDefinition()->GetPDGEncoding()) << " z " << zpre/m << " " << zpos/m << " " << energy/GeV << endl;

  if (!(zpre>zSave && zpos<=zSave))
    return; 

  const int pdg = abs(theStep->GetTrack()->GetDefinition()->GetPDGEncoding());
  const double angle = acos(-theStep->GetTrack()->GetMomentumDirection().z()); 

  // cout << "pdg " << abs(theStep->GetTrack()->GetDefinition()->GetPDGEncoding()) << " z " << zpre/m << " " << zpos/m << " " << energy/GeV << endl;

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  const string pdgname (particleTable->FindParticle(pdg)->GetParticleName());

  if (fHistE.count(pdgname)==0) {
    ostringstream hname1, hname2;
    hname1 << "hist_e_" << pdg;
    hname2 << "hist_z_" << pdg;
    fHistE[pdgname] = new TH1D(hname1.str().c_str(), pdgname.c_str(), 200, -3, 3);
    fHistE[pdgname]->SetXTitle("lg(E / GeV)");
    fHistZ[pdgname] = new TH1D(hname2.str().c_str(), pdgname.c_str(), 100, 0, 90);
    fHistZ[pdgname]->SetXTitle("Zenith / deg");
  }


  fHistE[pdgname]->Fill(log10(energy/GeV));
  fHistZ[pdgname]->Fill(angle/degree);

  theTrack->SetTrackStatus(fStopAndKill);

  /*
  if (!(zpre<fReadoutDistance && zpos>=fReadoutDistance) &&
      !(zpre<fAfterD1 && zpos>=fAfterD1) &&
      !(zpre<130*m && zpos>=130*m))
    return;
  */  
  //cout << "ok" << endl;
  /*
  
  //G4RunManager* runManager = G4RunManager::GetRunManager();
  //PrimaryGeneratorAction* generator = (PrimaryGeneratorAction*) runManager->GetUserPrimaryGeneratorAction();

  const double radial = sqrt(pow(theStep->GetPostStepPoint()->GetPosition().x(),2) + pow(theStep->GetPostStepPoint()->GetPosition().y(),2));
  
  
  if (zpre<fReadoutDistance && zpos>=fReadoutDistance) {
    
    if (fLayer1.fHistE.count(pdgname)==0) {
      ostringstream hname1, hname2, hname3;
      hname1 << "hist_e_" << pdg << "_layer1";
      hname2 << "hist_a_" << pdg << "_layer1";
      hname3 << "hist_p_" << pdg << "_layer1";
      fLayer1.fHistE[pdgname] = new TH1D(hname1.str().c_str(), pdgname.c_str(), 100, -3, 0.01);
      fLayer1.fHistE[pdgname]->SetXTitle("log_{10}(E / E_{beam})");
      fLayer1.fHistA[pdgname] = new TH1D(hname2.str().c_str(), pdgname.c_str(), 100, 0, 90);
      fLayer1.fHistA[pdgname]->SetXTitle("Angel / deg");
      fLayer1.fHistP[pdgname] = new TH1D(hname3.str().c_str(), pdgname.c_str(), 100, 0, 100);
      fLayer1.fHistP[pdgname]->SetXTitle("Radial / cm");
    }
    
    fLayer1.fHistA[pdgname]->Fill(angle/degree);
    fLayer1.fHistP[pdgname]->Fill(radial/centimeter);
    
  } else if (zpre<fAfterD1 && zpos>=fAfterD1) {
    
    if (fLayer3.fHistE.count(pdgname)==0) {
      ostringstream hname1, hname2, hname3;
      hname1 << "hist_e_" << pdg << "_layer3";
      hname2 << "hist_a_" << pdg << "_layer3";
      hname3 << "hist_p_" << pdg << "_layer3";
      fLayer3.fHistE[pdgname] = new TH1D(hname1.str().c_str(), pdgname.c_str(), 100, -3, 0.01);
      fLayer3.fHistE[pdgname]->SetXTitle("log_{10}(E / E_{beam})");
      fLayer3.fHistA[pdgname] = new TH1D(hname2.str().c_str(), pdgname.c_str(), 100, -5, 1);
      fLayer3.fHistA[pdgname]->SetXTitle("log_{10}(tan#theta) (r<10cm)");
      fLayer3.fHistP[pdgname] = new TH1D(hname3.str().c_str(), pdgname.c_str(), 100, 0, 100);
      fLayer3.fHistP[pdgname]->SetXTitle("Radial r / cm");
    }
    
    if (radial<10*centimeter) {
      //cout << log10(abs(tan(abs(angle)))) << endl;
      fLayer3.fHistA[pdgname]->Fill(log10(abs(tan(abs(angle)))));
    }
    fLayer3.fHistP[pdgname]->Fill(radial/centimeter);
    
  } else if (zpre<130*m && zpos>=130*m) {
    
    if (fLayer2.fHistE.count(pdgname)==0) {
      ostringstream hname1, hname2, hname3;
      hname1 << "hist_e_" << pdg << "_layer2";
      hname2 << "hist_a_" << pdg << "_layer2";
      hname3 << "hist_p_" << pdg << "_layer2";
      fLayer2.fHistE[pdgname] = new TH1D(hname1.str().c_str(), pdgname.c_str(), 100, -3, 0.01);
      fLayer2.fHistE[pdgname]->SetXTitle("log_{10}(E / E_{beam})");
      fLayer2.fHistA[pdgname] = new TH1D(hname2.str().c_str(), pdgname.c_str(), 100, 0, 90);
      fLayer2.fHistA[pdgname]->SetXTitle("Angel / deg");
      fLayer2.fHistP[pdgname] = new TH1D(hname3.str().c_str(), pdgname.c_str(), 100, 0, 100);
      fLayer2.fHistP[pdgname]->SetXTitle("Radial / cm");
    }
    
    fLayer2.fHistA[pdgname]->Fill(angle/degree);
    fLayer2.fHistP[pdgname]->Fill(radial/centimeter);
    
    //theTrack->SetTrackStatus(fStopAndKill);
  } else {
    cout << "ERROR" << endl;
    exit(1);
  }
  */
}
