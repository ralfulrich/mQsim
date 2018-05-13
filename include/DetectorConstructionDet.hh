#ifndef DetectorConstructionDet_h
#define DetectorConstructionDet_h 1

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
class DetectorSD;


class DetectorConstructionDet : public G4VUserDetectorConstruction
{
public:
  DetectorConstructionDet(const double qe);
  virtual ~DetectorConstructionDet();
  virtual void ConstructSDandField();

  DetectorSD* GetCells() { return fSciSD; }
  
public:
  virtual G4VPhysicalVolume* Construct();


private:
  DetectorSD* fSciSD;
  double fQe;
};

#endif

