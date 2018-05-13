
#ifndef MCPMinus_h
#define MCPMinus_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

// ######################################################################
// ###                        MCPMINUS                               ###
// ######################################################################

class MCPMinus : public G4ParticleDefinition
{
 private:
   static MCPMinus* theInstance;
   MCPMinus(){}
   ~MCPMinus(){}

  static double fgQE;

 public:
   static MCPMinus* Definition();
   static MCPMinus* MCPMinusDefinition();
   static MCPMinus* mCPMinus();

  static void SetQE(double qe) {fgQE = qe;}
};

#endif


