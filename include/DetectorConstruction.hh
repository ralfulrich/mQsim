#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4FieldManager.hh"
#include "G4SystemOfUnits.hh"    


class G4VPhysicalVolume;
class G4LogicalVolume;
class G4VReadOutGeometry;
class G4UniformMagField;
class G4FieldManager;
class G4MagneticField;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();
  
public:
  virtual G4VPhysicalVolume* Construct();
  
private:
  G4LogicalVolume * fMolasse_log;
  G4LogicalVolume* fTunnel_log;
};

#endif

