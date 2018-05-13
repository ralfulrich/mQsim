#ifndef PMTSD_h
#define PMTSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

#include "PMTHit.hh"
//#include "DetectorHit.hh"

class G4Step;
class G4HCofThisEvent;

class PMTSD : public G4VSensitiveDetector {

   public:
  
  PMTSD(G4String);
     ~PMTSD();
  
  void Initialize(G4HCofThisEvent*);
  //G4bool ProcessHits_constStep(const G4Step* , G4TouchableHistory* );
     G4bool ProcessHits(G4Step*,G4TouchableHistory*);
     void EndOfEvent(G4HCofThisEvent*);
     void clear();
     void DrawAll();
     void PrintAll();
  
  private:
  
     PMTHitsCollection* pmtCollection;
     G4int HitID;
};

#endif
