#include "EventAction.hh"
#include "DetectorSD.hh"
#include "PMTSD.hh"
#include "DetectorHit.hh"
#include "PMTHit.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "DetectorConstructionDet.hh"

#include "G4SystemOfUnits.hh"

#include "g4root.hh"

#include <iostream>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
EventAction::EventAction()
  :G4UserEventAction(), fNEvent(0), fHitCollID(-1)
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
EventAction::~EventAction()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void EventAction::BeginOfEventAction(const G4Event*)
{
  ++fNEvent;
  if (fNEvent%10 == 0) {
    cout << "Event #" << fNEvent << endl;
  }

}

void EventAction::PrintEventStatistics(G4double sciEdep, G4double sciTrackLength) const
{
  // print event statistics
  G4cout
     << "   SciBar: total energy: " 
     << std::setw(7) << G4BestUnit(sciEdep, "Energy")
     << "       total track length: " 
     << std::setw(7) << G4BestUnit(sciTrackLength, "Length")
     << G4endl;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void
EventAction::EndOfEventAction(const G4Event* evt)
{
  // Get hits collections IDs (only once)
  if ( fHitCollID == -1 ) {
    fHitCollID
      = G4SDManager::GetSDMpointer()->GetCollectionID("ScintillatorHitsCollection");
  }

  if (fHitCollID == -1) {
    cout << "NO ScintillatorHitsCollection" << endl;
    return;
  }
  
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  if (!HCE) {
    cout << "NO HCE" << endl;
    return;
  }
  
  // Get hits collections
  DetectorHitsCollection* sciHC
    = static_cast<DetectorHitsCollection*>(HCE->GetHC(fHitCollID));
  
  if ( ! sciHC ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << fHitCollID; 
    G4Exception("EventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  static int pmtCollID = -1; 
  {
    if (pmtCollID==-1) {
      G4SDManager *SDman = G4SDManager::GetSDMpointer();
      pmtCollID = SDman->GetCollectionID("pmtCollection");
    }

  }
  
  PMTHitsCollection* PHC = (PMTHitsCollection*)(HCE->GetHC(pmtCollID));
  if (!PHC) {
    G4ExceptionDescription msg;
    msg << "Cannot access pmtCollection ID " << pmtCollID;
    G4Exception("EventAction::GetHitsCollection()",
		"MyCode0003", FatalException, msg);    
  }

  map<int,int> hitMap;
  if(PHC) {
    int P_hits = PHC->entries();
    double aveTimePmtHits = 0;
    double firstLXeHitTime =  0;
    
    // average time of PMT hits
    for (G4int i=0; i<P_hits; i++) {

      cout << "PMT hit=" << ", id=" << (*PHC)[i]->GetHitId() << endl;

      G4double time = ( (*PHC)[i]->GetTime() - firstLXeHitTime );
      aveTimePmtHits += time / (G4double)P_hits;
      hitMap[(*PHC)[i]->GetHitId()]++; // count PE
      ////      if (event_id == 0) {
      //if(P_hits >= 0) {
      //	man->FillH1(7,time);
      //}
    }
  
    G4int eventID = evt->GetEventID();
    G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    //    if (eventID%printModulo == 0 && P_hits > 0) {
    G4cout << "     Average light collection time: "
	   << G4BestUnit(aveTimePmtHits,"Time") << G4endl;
    G4cout << "     Number of PMT hits (photoelectron equivalent): " 
	   << P_hits << G4endl;     
    //}
    // write out (x,y,z) of PMT hits
    //if (savePmtFlag)
    //writePmtHitsToFile(PHC);
  }

  
  // ---------------------------------------------------------------------
  
  // Get hit with total values
  DetectorHit* sciHit_sum = (*sciHC)[sciHC->entries()-1];
 
  // Print per event (modulo n)
  //
  G4int eventID = evt->GetEventID();
  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl; 
    PrintEventStatistics(sciHit_sum->GetEdep(), sciHit_sum->GetTrackLength());
  }  
  
  // Fill histograms, ntuple
  //
  
  // get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  // fill histograms
  analysisManager->FillH1(1, sciHit_sum->GetEdep()/MeV);
  analysisManager->FillH1(2, sciHit_sum->GetTrackLength()/meter);
  
  // loop all hits:
  int count = 0;
  for (int i=0; i<sciHC->entries()-1; ++i) {

    const int nrow = DetectorConstructionDet::fgNumberGrid;
    const int nrow2 = pow(nrow, 2);
    const int idLay = i / nrow2;
    const int col = (i % nrow2) / nrow;
    const int row = (i % nrow2) % nrow;
    
    DetectorHit* thesciHit = (*sciHC)[i];
    const double edep = thesciHit->GetEdep();
    const double length = thesciHit->GetTrackLength();
    const double nph = thesciHit->GetPh();
    const double nPE = (hitMap.count(i) ? hitMap[i] : 0);
    thesciHit->SetNPE(int(nPE*0.2));
    
    if (edep/keV>0 || nph>0 || nPE>0) {
      analysisManager->FillH1(3, i);
      count++;
      cout << "Cell=" << i
           << ", lay/row/col=" << idLay << "/" << row << "/" << col 
           << ", edep=" << edep/keV << "keV, ph=" << nph << ", yield=" << nph/(edep/keV) <<  " ph/keV "
           << ", length=" << length/cm << " cm"
           << ", dEdX=" << edep/(length*3.67*g/cm3) / MeV *g/cm2 << " MeV g/cm2"
           << ", nPE=" << nPE
	   << " eff_opt=" << nPE/nph
	   <<  endl;            

      // fill the ntuple
      analysisManager->FillNtupleDColumn(0, edep/keV);
      analysisManager->FillNtupleDColumn(1, nph);
      analysisManager->FillNtupleDColumn(2, nPE);
      analysisManager->FillNtupleDColumn(3, col);
      analysisManager->FillNtupleDColumn(4, row);
      analysisManager->FillNtupleDColumn(5, idLay);
      analysisManager->FillNtupleDColumn(6, eventID);
      analysisManager->AddNtupleRow();  
    }
    
    analysisManager->FillH2(1+idLay, col, row, edep/MeV);

  } // loop sciHC hits
  
  analysisManager->FillH1(4, count);
  
  // check trigger
  const int nrow = DetectorConstructionDet::fgNumberGrid;
  for (int i=0; i<nrow; ++i) {
    for (int j=0; j<nrow; ++j) {
      
    }
  }

  
}
