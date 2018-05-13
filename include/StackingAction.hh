#ifndef StackingAction_H
#define StackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"


//class StackingActionMessenger;
class G4Navigator;
class G4Track;
class DetectorSD;
class DetectorConstructionDet;

class StackingAction : public G4UserStackingAction {

  public:
    StackingAction(DetectorConstructionDet* det);
    virtual ~StackingAction();

  public:
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();

  //    StackingActionMessenger* theMessenger;


  private:

  //DetectorSD* fCells;
  DetectorConstructionDet* fDet;
  
    G4Navigator* gNavigator; 

  public:

};

#endif

