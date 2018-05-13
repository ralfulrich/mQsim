#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4PrimaryParticle;

class EventAction : public G4UserEventAction
{
public:
  EventAction();//G4bool);
  virtual ~EventAction();

public:
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);

private:
  int fNEvent;

  G4int fHitCollID;
  
  void PrintEventStatistics(G4double sciEdep, G4double sciTrackLength) const;

  //  G4int fTrackerCollID;
  // 

  
  //  void PrintPrimary(G4PrimaryParticle* pp,G4int ind);
};

#endif

    
