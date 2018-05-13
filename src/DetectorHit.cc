
#include "DetectorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "G4VPhysicalVolume.hh"

#include <iomanip>
#include <iostream>

using namespace std;


G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorHit::DetectorHit()
 : G4VHit(),
   fEdep(0.),
   fTrackLength(0.),
   fNPh(0),
   fNPE(0),
   fPhysVol(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorHit::~DetectorHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorHit::DetectorHit(const DetectorHit& right)
  : G4VHit()
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
  fNPh = right.fNPh;
  fNPE  = right.fNPE;
  fPhysVol = right.fPhysVol;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const DetectorHit& DetectorHit::operator=(const DetectorHit& right)
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
  fNPh = right.fNPh;
  fNPE  = right.fNPE;
  fPhysVol = right.fPhysVol;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int DetectorHit::operator==(const DetectorHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorHit::Print()
{
  G4cout
     << "Edep: " 
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " track length: " 
     << std::setw(7) << G4BestUnit( fTrackLength,"Length")
     << G4endl;
}

void
DetectorHit::SetPV(G4VPhysicalVolume* pv, const G4RotationMatrix& rot, const G4ThreeVector& tr)
 {
   if (fPhysVol)
     return;
   
   fPhysVol=pv;
   fRot=new G4RotationMatrix(rot);
   fTra=new G4ThreeVector(tr);
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void 
DetectorHit::Draw()
  {
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(!pVVisManager)
      return;

    if (fLogV) {
      G4Transform3D trans(fRot,fPos);
      G4VisAttributes attribs;
      G4LogicalVolume* logVol = pPhys->GetLogicalVolume();
      const G4VisAttributes* pVA = logVol->GetVisAttributes();
      if(pVA) attribs = *pVA;
      G4Colour colour(1.,0.,0.);
      attribs.SetColour(colour);
      attribs.SetForceSolid(true);

      //----- Re-visualization of a selected physical volume with red color
      pVVisManager->Draw(*pPhys,attribs,trans);

    }
  }
}
*/


void
DetectorHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(!pVVisManager)
    return;
  
  if (fPhysVol) {
    G4VisAttributes attribs;
    //cout << "fNPE=" << fNPE << endl;
    if (fNPE>1) {
      attribs = G4VisAttributes(G4Colour(0., 0., 1.));
    } else 
      attribs = G4VisAttributes(G4Colour(0.1, 0.5, .1, 0.6));

    attribs.SetForceSolid(true);
    //    G4RotationMatrix rot;
    //if(fPhysVol->GetRotation())
    //rot =* (fPhysVol->GetRotation());
    //G4Transform3D trans(rot,fPhysVol->GetTranslation());//Create transform
    G4Transform3D trans(*fRot,*fTra);//Create transform
    pVVisManager->Draw(*fPhysVol,attribs,trans);//Draw it
  }
}

