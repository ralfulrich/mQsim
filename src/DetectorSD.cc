
#include "DetectorSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"    

#include "G4OpticalPhoton.hh"

#include <iostream>
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorSD::DetectorSD(const G4String& name, 
		       const G4String& hitsCollectionName,
		       G4int nlayers,
		       int ngrid)
  : G4VSensitiveDetector(name),
    fHitsCollection(0),
    fNLayer(nlayers),
    fNGrid(ngrid)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorSD::~DetectorSD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new DetectorHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits  
  for (G4int i=0; i<fNLayer*fNGrid*fNGrid+1; i++ ) {
    fHitsCollection->insert(new DetectorHit());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool DetectorSD::ProcessHits(G4Step* step, 
			       G4TouchableHistory*)
{  
  //need to know if this is an optical photon and exclude it:
  if(step->GetTrack()->GetDefinition()
     == G4OpticalPhoton::OpticalPhotonDefinition()) {
    // cout << "OPTICALPHOTON" << endl;
    return false;
  }
  
  
  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();

  //  cout << "processhits " << edep / GeV << endl;
  
  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
  }

  /*
  cout << step->GetTrack()->GetDefinition()->GetPDGCharge()
       << " " << step->GetTrack()->GetDefinition()->GetPDGEncoding()
    //<< " " << step->GetTrack()->GetDefinition()->GetPDGName()
       << " " << endl;
  */
  
  if ( edep==0. && stepLength == 0. )
    return false;      

  G4TouchableHistory* touchable
    = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    
  // Get calorimeter cell id 
  // G4int layerNumber0 = touchable->GetReplicaNumber(1);
  G4int layerNumber1= touchable->GetReplicaNumber(2);
  G4int layerNumber2 = touchable->GetReplicaNumber(3);
  G4int layerNumber3 = touchable->GetReplicaNumber(4);

  int layerNumber = layerNumber3 * fNGrid*fNGrid + layerNumber2 * fNGrid + layerNumber1;
  
  //if (layerNumber>300) {
  //  cout << "layerNumber " << layerNumber0 << " " << layerNumber1 << " "<< layerNumber2 << " " << layerNumber3 << " " << layerNumber << endl;
    //    cout << touchable->GetVolume()->
    //}
  
  
  // Get hit accounting data for this cell
  DetectorHit* hit = (*fHitsCollection)[layerNumber];
  if ( ! hit ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber; 
    G4Exception("DetectorSD::ProcessHits()",
      "MyCode0004", FatalException, msg);
  }

  //hit->SetPV(touchable->GetVolume(0));
  G4AffineTransform transform = touchable->GetHistory()->GetTopTransform();
  transform.Invert();
  hit->SetPV(touchable->GetVolume(), // touchable->GetVolume(0)->GetLogicalVolume(),
	     transform.NetRotation(),
	     transform.NetTranslation());
  
  // Get hit for total accounting
  DetectorHit* hitTotal 
    = (*fHitsCollection)[fHitsCollection->entries()-1];
  
  // Add values
  hit->Add(edep, stepLength);
  hitTotal->Add(edep, stepLength); 

  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { 
     G4int nofHits = fHitsCollection->entries();
     G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nofHits 
       << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ )
       (*fHitsCollection)[i]->Print();
  }
}
