#ifndef PMTHit_h
#define PMTHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


class PMTHit : public G4VHit 
{
  public:

      PMTHit();
     ~PMTHit();

      PMTHit(const PMTHit&);
      const PMTHit& operator=(const PMTHit&);
      int operator==(const PMTHit&) const;

      inline void* operator new(size_t);
      inline void  operator delete(void*);

      void Draw();
      void Print();

      void SetHitId(const int id) { fHitId=id; }
      int GetHitId() const { return fHitId; }
  
  private:
     G4ThreeVector pos;
     G4double time;

     int fHitId;


  public:
     inline void SetPos(G4ThreeVector xyz)       {pos=xyz;};
     inline G4ThreeVector GetPos()               const {return pos;};

     inline void SetTime(G4double t)             {time=t;};
     inline G4double GetTime()                   const {return time;};


};


// vector collection of one type of hits
typedef G4THitsCollection<PMTHit> PMTHitsCollection;


extern G4ThreadLocal G4Allocator<PMTHit> *PMTHitsAllocator;


inline void* PMTHit::operator new(size_t) {
  if (!PMTHitsAllocator)
    PMTHitsAllocator = new G4Allocator<PMTHit>;
  return (void*) PMTHitsAllocator->MallocSingle();
}


inline void PMTHit::operator delete(void* aHit) {
  PMTHitsAllocator->FreeSingle((PMTHit*) aHit);
}

#endif

