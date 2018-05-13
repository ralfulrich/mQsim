#ifndef DetectorSD_h
#define DetectorSD_h 1

#include "G4VSensitiveDetector.hh"

#include "DetectorHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

/// Calorimeter sensitive detector class
///
/// In Initialize(), it creates one hit for each calorimeter layer and one more
/// hit for accounting the total quantities in all layers.
///
/// The values are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step.

class DetectorSD : public G4VSensitiveDetector
{
  public:
    DetectorSD(const G4String& name, 
	       const G4String& hitsCollectionName, 
	       G4int nl, int ngr);
    virtual ~DetectorSD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  int GetLayer(const int i) { return i % (fNGrid*fNGrid); }
  int GetColumn(const int i) { return (i / (fNGrid*fNGrid)) % fNGrid; }
  int GetRow(const int i) { return (i / (fNGrid*fNGrid)) / fNGrid; }

  DetectorHitsCollection* GetCollection() { return fHitsCollection; }
  
  private:
    DetectorHitsCollection* fHitsCollection;
  int     fNLayer;
  int fNGrid;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

