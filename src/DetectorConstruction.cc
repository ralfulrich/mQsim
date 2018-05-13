
#include "DetectorConstruction.hh"
//#include "RegionInformation.hh"
//#include "MagneticField.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4ChordFinder.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "G4UniformMagField.hh"
#include "G4PVParameterised.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4SystemOfUnits.hh"    
#include "G4MagneticField.hh"
#include "G4AutoDelete.hh"


const double depth = 80 * meter; // bei 2.2mm->1.2m, bei 6.5mm->3.5m
const double height_tunnel = 3 * meter;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
DetectorConstruction::~DetectorConstruction()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  //-------------------------------------------------------------------------
  // Materials
  //-------------------------------------------------------------------------

  //  Material Information imported from NIST database.
  G4NistManager* NISTman = G4NistManager::Instance();
  G4Material* air     = NISTman->FindOrBuildMaterial("G4_AIR");

  G4String symbol;             //a=mass of a mole;
  G4double a, z, density;      //z=mean number of protons;  
  G4Element* Na = new G4Element("Sodium",  symbol="Na", z=11, a=  22.99*g/mole);

  //G4Material* rock =  NISTman->FindOrBuildMaterial("G4_WATER");
  G4Material* rock = new G4Material("StandardRock",
                                     2.65*CLHEP::g/CLHEP::cm3, 1, kStateSolid);
  rock->AddElement(Na, 1);

  
  //-------------------------------------------------------------------------
  // Detector geometry
  //-------------------------------------------------------------------------
  //------------------------------ experimental hall
  G4Box * world_box = new G4Box("world", 500*meter,500*meter,depth+1*meter); // half lengths
  G4LogicalVolume * world_log = new G4LogicalVolume(world_box, air, "world_L",0,0,0);
  G4VPhysicalVolume * world_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0*meter),world_log,"world_P", 0,false,0);
  G4VisAttributes* worldVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  worldVisAtt->SetForceWireframe(true);
  world_log->SetVisAttributes(worldVisAtt);

  //------------------------------ molasse rock
  G4Box * molasse_box = new G4Box("molasse_box",500*meter, 500*meter, depth);
  fMolasse_log  = new G4LogicalVolume(molasse_box, rock, "molasse_log",0,0,0);
  G4ThreeVector molasse_pos(0,0,0);
  G4VPhysicalVolume * molasse_phys = new G4PVPlacement(0,molasse_pos,fMolasse_log,"molasse_log", world_log,false,0);
  G4VisAttributes* molasse_logVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  molasse_logVisAtt->SetForceWireframe(true);
  fMolasse_log->SetVisAttributes(molasse_logVisAtt);
  
  
  // --- tunnel
  G4Box * tunnel_box = new G4Box("tunnel_box",500*meter, 500*meter, height_tunnel/2);
  fTunnel_log  = new G4LogicalVolume(tunnel_box, rock, "tunnel_log",0,0,0);
  G4ThreeVector tunnel_pos(0,0,0);
  G4VPhysicalVolume * tunnel_phys = new G4PVPlacement(0,tunnel_pos,fTunnel_log,"tunnel_log", fMolasse_log,false,0);
  G4VisAttributes* tunnel_logVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  tunnel_logVisAtt->SetForceWireframe(true);
  fTunnel_log->SetVisAttributes(tunnel_logVisAtt);
  
  return world_phys;
}


