#include "StackingAction.hh"
#include "DetectorHit.hh"

#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "DetectorSD.hh"
#include "DetectorConstructionDet.hh"

#include <iostream>
using namespace std;

StackingAction::StackingAction(DetectorConstructionDet* det) : fDet(det) {

  // new messenger
  //  theMessenger = new StackingActionMessenger(this);

  // global geometry navigator
  gNavigator = G4TransportationManager::GetTransportationManager()
    ->GetNavigatorForTracking();
}


StackingAction::~StackingAction() {
  
  //  delete theMessenger; 
}


G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
  G4ClassificationOfNewTrack classification = fWaiting;

  G4ParticleDefinition* particleType = aTrack->GetDefinition();
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()) {
    
    const G4Step* step = aTrack->GetStep();

    G4TouchableHandle touchable = aTrack->GetTouchableHandle();
  
    //cout << " " << ->GetCopyNumber(2) << endl;
    
    const int id1 = 0;
    G4int layerNumber0 = touchable->GetReplicaNumber(id1+1);
    G4int layerNumber1= touchable->GetReplicaNumber(id1+2);
    G4int layerNumber2 = touchable->GetReplicaNumber(id1+3);
    G4int layerNumber3 = touchable->GetReplicaNumber(id1+4);

    int fNGrid = 15;
    int layerNumber = layerNumber3 * fNGrid*fNGrid + layerNumber2 * fNGrid + layerNumber1;

    //cout << "layerNumber " << layerNumber0 << " " << layerNumber1 << " "<< layerNumber2 << " " << layerNumber3 << " " << layerNumber << endl;

    

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
    DetectorHit* hitTotal  = (*sciHC)[sciHC->entries()-1];

    hit->AddPh(1.);
    hitTotal->AddPh(1.);

  } else {

    //    cout << "created other "  << particleType->GetPDGEncoding() << endl;
  }
  
  return classification;

}


void StackingAction::NewStage() {;}

    
void StackingAction::PrepareNewEvent() {;}




