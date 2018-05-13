#include "PMTSD.hh"

//#include "DetectorConstructionDet.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"

#include "G4OpticalPhoton.hh"

#include <iostream>

using namespace std;


PMTSD::PMTSD(G4String name)
  : G4VSensitiveDetector(name)  //: fNLayer(nlay), fNGrid(ngrid) {
{
  G4String HCname="pmtCollection";
  collectionName.insert(HCname);

  //  G4String HCname="sciCollection";
  // collectionName.insert(HCname);
}


PMTSD::~PMTSD() {;}


////////////////////////////////////////////////////////////////////////////
void PMTSD::Initialize(G4HCofThisEvent*) {

  pmtCollection = new PMTHitsCollection
    (SensitiveDetectorName,collectionName[0]); 

  HitID = -1;
}

////////////////////////////////////////////////////////////////////////////
G4bool
PMTSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  //  return false;
  //}

//G4bool PMTSD::ProcessHits_constStep(const G4Step* aStep,
//                                     G4TouchableHistory* ){


  // cout << " pmtsd: " << aStep->GetTrack()->GetDefinition()->GetPDGEncoding() << endl;
  
  //need to know if this is an optical photon
  if(aStep->GetTrack()->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition())
    return false;
  

  // make known hit position
  PMTHit* aPmtHit = new PMTHit();
  aPmtHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
  aPmtHit->SetTime(aStep->GetPostStepPoint()->GetGlobalTime());

  
  G4TouchableHistory* touchable
    = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
    
  // Get calorimeter cell id 
  G4int layerNumber0 = touchable->GetReplicaNumber(1);
  G4int layerNumber1= touchable->GetReplicaNumber(2);
  G4int layerNumber2 = touchable->GetReplicaNumber(3);
  G4int layerNumber3 = touchable->GetReplicaNumber(4);

  int fNGrid = 15;  
  int layerNumber = layerNumber3 * fNGrid*fNGrid + layerNumber2 * fNGrid + layerNumber1;
  
  //if (layerNumber>300) {
  //cout << "layerNumber " << layerNumber0 << " " << layerNumber1 << " "<< layerNumber2 << " " << layerNumber3 << " " << layerNumber << endl;
  //    cout << touchable->GetVolume()->
    //}

  aPmtHit->SetHitId(layerNumber); 

  HitID = pmtCollection->insert(aPmtHit);

  /*
  static DetectorHitsCollection* sciHC = 0;
  if (!sciHC) {
    sciHC = fDet->GetCells()->GetCollection();
  }
    
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
  */

  aStep->GetTrack()->SetTrackStatus(fStopAndKill);

  return true; 
}



////////////////////////////////////////////////////////////////////////////
void PMTSD::EndOfEvent(G4HCofThisEvent* HCE) {

  G4String HCname = collectionName[0];

  static G4int HCID = -1;
  if(HCID<0)
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(HCname);
  HCE->AddHitsCollection(HCID,pmtCollection);
  
  G4int nHits = pmtCollection->entries();
  if (verboseLevel>=1) {
    G4cout << "     PMT collection: " << nHits << " hits" << G4endl;
    if (verboseLevel>=2)
      pmtCollection->PrintAllHits();
  }


}


////////////////////////////////////////////////////////////////////////////
void PMTSD::clear()    {;}


void PMTSD::DrawAll()  {;}


void PMTSD::PrintAll() {;}




