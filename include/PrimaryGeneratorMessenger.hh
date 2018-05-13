#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;

class PrimaryGeneratorMessenger: public G4UImessenger
{
public:
  PrimaryGeneratorMessenger(PrimaryGeneratorAction* mpga);
  virtual ~PrimaryGeneratorMessenger();

public:
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  virtual G4String GetCurrentValue(G4UIcommand * command);

private:
  PrimaryGeneratorAction * fMyAction;
    
private: //commands
  G4UIdirectory *             fMydetDirectory;
  G4UIcmdWithAString *        fGenCmd;
    
};

#endif


