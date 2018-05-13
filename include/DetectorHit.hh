
#ifndef DetectorHit_h
#define DetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

#include "G4RotationMatrix.hh"

class G4VPhysicalVolume;
// class G4RotationMatrix;

class DetectorHit : public G4VHit
{
  public:
  DetectorHit(); 
    DetectorHit(const DetectorHit&);
    virtual ~DetectorHit();

    // operators
    const DetectorHit& operator=(const DetectorHit&);
    G4int operator==(const DetectorHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Print();

    // methods to handle data
    void Add(G4double de, G4double dl);
    void AddPh(double ph);

  void SetNPE(double pe) { fNPE=pe; }
  
  // get methods
    G4double GetEdep() const;
    G4double GetTrackLength() const;
    double GetPh() const;

  void SetPV(G4VPhysicalVolume* pv, const G4RotationMatrix& rot, const G4ThreeVector& tr);
  
  virtual void Draw();
  
  private:  
    G4double fEdep;        ///< Energy deposit in the sensitive volume
    G4double fTrackLength; ///< Track length in the  sensitive volume
  G4double fNPh;
  G4double fNPE;

  G4VPhysicalVolume* fPhysVol;
  G4RotationMatrix* fRot;
  G4ThreeVector* fTra;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<DetectorHit> DetectorHitsCollection;

extern G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* DetectorHit::operator new(size_t)
{
  if(!DetectorHitAllocator)
    DetectorHitAllocator = new G4Allocator<DetectorHit>;
  void *hit;
  hit = (void *) DetectorHitAllocator->MallocSingle();
  return hit;
}

inline void DetectorHit::operator delete(void *hit)
{
  if(!DetectorHitAllocator)
    DetectorHitAllocator = new G4Allocator<DetectorHit>;
  DetectorHitAllocator->FreeSingle((DetectorHit*) hit);
}

inline void DetectorHit::Add(G4double de, G4double dl) {
  fEdep += de; 
  fTrackLength += dl;
}

inline void DetectorHit::AddPh(G4double ph) {fNPh += ph;}

inline G4double DetectorHit::GetEdep() const { 
  return fEdep; 
}

inline G4double DetectorHit::GetTrackLength() const { 
  return fTrackLength; 
}

inline double DetectorHit::GetPh() const  {return fNPh;}

#endif
