#include "PMTHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<PMTHit> *PMTHitsAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

PMTHit::PMTHit() {
  
  pos = G4ThreeVector(0., 0., 0.);
  time=0.;
  fHitId = 0;
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

PMTHit::~PMTHit() {;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

PMTHit::PMTHit(const PMTHit& right) : G4VHit(right) {

  pos  = right.pos;
  time = right.time;
  fHitId = right.fHitId;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const PMTHit& PMTHit::operator=(const PMTHit& right) {

  pos  = right.pos;
  time = right.time;
  fHitId = right.fHitId;

  return *this;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

int PMTHit::operator==(const PMTHit& right) const {

  return (this==&right) ? 1 : 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PMTHit::Draw()  {

  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

  if(pVVisManager) {
    G4Circle circle(pos);
    circle.SetScreenSize(0.002);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,1.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PMTHit::Print() {

  //  G4cout << "      PMT hit: " << G4BestUnit(pos,"Length") << G4endl;

  G4cout << "      PMT hit: " << std::setw(5) << G4BestUnit(time,"Time") 
	 << ", at " << G4BestUnit(pos,"Length") << G4endl;


}



