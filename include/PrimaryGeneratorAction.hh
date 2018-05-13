#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4VPrimaryGenerator;
class G4Event;
class G4GeneralParticleSource;
class G4ParticleGun;
//class PrimaryGeneratorMessenger;

class TF1;
class TGraph;
class TH1D;

class PrimaryGeneratorAction:public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction(const std::string& name);
  virtual ~PrimaryGeneratorAction();

  G4ParticleGun* GetParticleSource()           {return fParticleGun;} 

  int GetCount() const { return fCount; }
  double GetSeonds() const { return double(fCount) / fRate; }
  
public:
  virtual void GeneratePrimaries(G4Event* anEvent);
  
private:
  G4ParticleGun* fParticleGun; //G4VPrimaryGenerator* fParticleGun;
  //  PrimaryGeneratorMessenger* fMessenger;

  int fCount;
  double fRate;
  
  TF1* fSpect;
  //TF1* fZenith;
  TGraph* fZenith;

  bool fMuonsfromSurface;
  TH1D* fHistEnergy;
  TH1D* fHistZenith;
  
};

#endif


