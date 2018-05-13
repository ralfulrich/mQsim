
#ifndef SteppingAction_H
#define SteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include <map>
#include <string>

class TH1D;
class TCanvas;
class DetectorConstructionDet;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(DetectorConstructionDet* det);
  virtual ~SteppingAction();
  
  virtual void UserSteppingAction(const G4Step*);

private:

  DetectorConstructionDet* fDet;
  
  std::map<std::string, TH1D*> fHistE;
  std::map<std::string, TH1D*> fHistZ;
};

#endif

