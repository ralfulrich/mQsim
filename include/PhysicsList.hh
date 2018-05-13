#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"

class PhysicsList : public G4VUserPhysicsList 
{
public:
  PhysicsList(double Qe);
  virtual ~PhysicsList();

  virtual void SetCuts();

protected:
  virtual void ConstructParticle();
  virtual void ConstructProcess();

  // these methods Construct physics processes and register them
  virtual void ConstructGeneral();
  virtual void ConstructEM();
  virtual void ConstructHad();
  virtual void ConstructOp();

  virtual void AddTransportation();


  double fQe;
};
#endif
