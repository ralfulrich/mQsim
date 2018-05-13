
#include "DetectorConstructionDet.hh"
//#include "RegionInformation.hh"
//#include "MagneticField.hh"
#include "DetectorSD.hh"
#include "PMTSD.hh"

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

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SubtractionSolid.hh"

#include "G4SDManager.hh"

#include <iostream>
using namespace std;

const double depth_tunnel = 80 * meter; // bei 2.2mm->1.2m, bei 6.5mm->3.5m
const double height_tunnel = 2.5 * meter;

const double angle = 43.1 * degree;
const double rockIP5 = 17 * meter;
const double distanceIP5 = 33 * meter;    

const int nlay = 4;
const int nGrid = 15;
const double wCell = 5*centimeter;
const double distanceLayer = 0.3*meter;
const double heightCrystal = 0.1*meter;
const double crystalCase = 3*millimeter;

const double heightPMT = 150*millimeter;
const double radiusPMT = 2.4*centimeter;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
DetectorConstructionDet::DetectorConstructionDet(double qe)
  : G4VUserDetectorConstruction(), fQe(qe)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
DetectorConstructionDet::~DetectorConstructionDet()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
G4VPhysicalVolume* DetectorConstructionDet::Construct()
{
  const double Qe = fQe;

  //-------------------------------------------------------------------------
  // Materials
  //-------------------------------------------------------------------------

  //  Material Information imported from NIST database.
  G4NistManager* NISTman = G4NistManager::Instance();

  G4Material* air     = NISTman->FindOrBuildMaterial("G4_AIR");
  //G4Material* sci     = NISTman->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  G4Material* defaultMaterial = NISTman->FindOrBuildMaterial("G4_Galactic");

  
  // making quartz
  string name, symbol;
  double z, a;
  G4Element* O  = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a=16.00*g/mole);
  G4Element* Si = new G4Element(name="Silicon",symbol="Si" , z= 14., a=28.09*g/mole);
  G4Element* Al = new G4Element(name="Aluminium"  ,symbol="Al" , z= 13., a=26.98*g/mole);
  G4Element* Fe = new G4Element(name="Iron"  ,symbol="Fe" , z= 26., a=55.85*g/mole);  
  G4Element* C  = new G4Element( "Carbon", "C",   6. , 12.011*g/mole);
  G4Element* Co = new G4Element( "Cobalt", "Co", 27. , 58.9332*g/mole);
  G4Element* Cu = new G4Element(name="Copper"  ,symbol="Cu" , z= 29., a=63.55*g/mole);  
  G4Element* H = new G4Element(name="Hydrogen",symbol="H" , z= 1., a=1.00794*g/mole);
  G4Element* Ca = new G4Element(name="Calcium",symbol="Ca" , z= 20., a=40.078*g/mole);
  G4Element* Na = new G4Element("Sodium",  symbol="Na", z=11, a=  22.99*g/mole);
  G4Element* I = new G4Element("Iodine",  symbol="I", z=53, a=  126.9044*g/mole);
  
  double density;
  int ncomponents;
  
  //  G4Material* NaI     = NISTman->FindOrBuildMaterial("G4_SODIUM_IODIDE");
  G4Material* NaI = new G4Material("NaI", density=3.67*g/cm3, ncomponents=2);
  NaI->AddElement(Na, 1);
  NaI->AddElement(I, 1);
  
  {
    const G4int NUMENTRIES = 20;
    G4double Scnt_PP[NUMENTRIES] = { 1.38125*eV, 1.63569*eV, 1.89013*eV, 2.14457*eV, 2.39901*eV, 2.65345*eV,
				     2.90789*eV, 3.16234*eV, 3.41678*eV, 3.67122*eV, 3.92566*eV, 4.1801*eV,
				     4.43454*eV, 4.68898*eV, 4.94342*eV, 5.19786*eV, 5.4523*eV, 5.70674*eV, 5.96118*eV, 6.21562*eV};
    // G4double Scnt_FAST[NUMENTRIES] = {0.0440615, 0.0256187, 0.0121412, 0.00186181, 0.070215, 0.484923, 1.13553, 1.07503, 0.593948, 0.365924, 0.120932, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    G4double Scnt_FAST[NUMENTRIES] = {0.011211, 0.00651844, 0.00308922, 0.000473721, 0.0178656, 0.123384,
				      0.288925, 0.273532, 0.151124, 0.0931059, 0.0307701, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    G4double rindex[NUMENTRIES];
    G4double absorption[NUMENTRIES];
    for (int i=0; i<NUMENTRIES; ++i) {
      rindex[i] = 1.85;
      absorption[i] = 1*meter;
      // Scnt_FAST[i] = 10;
    }
    G4MaterialPropertiesTable* Scnt_MPT = new G4MaterialPropertiesTable();    
    Scnt_MPT->AddProperty("FASTCOMPONENT", Scnt_PP, Scnt_FAST, NUMENTRIES);
    Scnt_MPT->AddProperty("SLOWCOMPONENT", Scnt_PP, Scnt_FAST, NUMENTRIES);    
    Scnt_MPT->AddConstProperty("SCINTILLATIONYIELD", 40000./MeV * Qe*Qe);
    Scnt_MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
    Scnt_MPT->AddConstProperty("FASTTIMECONSTANT",  264*ns);
    Scnt_MPT->AddConstProperty("SLOWTIMECONSTANT", 264*ns);
    Scnt_MPT->AddConstProperty("YIELDRATIO", 1.0);
    
    Scnt_MPT->AddProperty("RINDEX", Scnt_PP, rindex, NUMENTRIES);//}->SetSpline(true);
    Scnt_MPT->AddProperty("ABSLENGTH", Scnt_PP, absorption, NUMENTRIES);//}->SetSpline(true);
    
    NaI->SetMaterialPropertiesTable(Scnt_MPT);
  }

  
  G4Material* glass = new G4Material("Glass", density=1.032*g/cm3, 2);
  glass->AddElement(C, 91.533*perCent);
  glass->AddElement(H, 8.467*perCent);

  /*
  {
    int nEntries = 5;
    G4double Glass_RIND[nEntries]={1.49,1.49,1.49,1.49,1.49};
    G4double Glass_AbsLength[nEntries]={420.*cm,420.*cm,420.*cm,420.*cm,420.*cm};
    G4MaterialPropertiesTable *Glass_mt = new G4MaterialPropertiesTable();
    Glass_mt->AddProperty("ABSLENGTH",LaBr_Energy,Glass_AbsLength,nEntries);
    Glass_mt->AddProperty("RINDEX",LaBr_Energy,Glass_RIND,nEntries);
    Glass->SetMaterialPropertiesTable(Glass_mt);
  }
  */
  
  // making quartz
  G4Material* quartz = new G4Material(name="quartz", density=2.200*g/cm3, ncomponents=2);
  quartz->AddElement(Si, 1);
  quartz->AddElement(O , 2);

  {
    const int NUMENTRIES = 3;
    G4double quartz_PP[NUMENTRIES]   = { 5.0*eV, 6.69*eV, 7.50*eV }; // lambda range 4 ri
    G4double quartz_RIND[NUMENTRIES] = { 1.51, 1.57, 1.61 };     // ref index
    //  G4double quartz_RIND[NUMENTRIES] = { 1.45, 1.51, 1.54 };     // ref index
    G4double quartz_ABSL[NUMENTRIES] = { 3.0*cm, 3.0*cm, 3.0*cm };// atten length
    G4MaterialPropertiesTable *quartz_mt = new G4MaterialPropertiesTable();
    quartz_mt->AddProperty("RINDEX", quartz_PP, quartz_RIND, NUMENTRIES);
    quartz_mt->AddProperty("ABSLENGTH", quartz_PP, quartz_ABSL, NUMENTRIES);
    quartz->SetMaterialPropertiesTable(quartz_mt);
  }
  
  // stainless steel
  G4Material* ssteel = new G4Material(name="Steel", density=7.7*g/cm3, ncomponents=3);
  ssteel->AddElement(C, 0.04);
  ssteel->AddElement(Fe, 0.88);
  ssteel->AddElement(Co, 0.08);

  // copper
  G4Material* metalCu = new G4Material(name="MetalCopper", density=8.960*g/cm3, ncomponents=1);
  metalCu->AddElement(Cu, 1);

  
  // photocathode aluminium
  G4Material* cathmetalAl = new G4Material(name="CathodeMetalAluminium", density=2.700*g/cm3, ncomponents=1);
  cathmetalAl->AddElement(Al, 1);
  {
    const int NUMENTRIES = 3;
    G4double cathmetal_PP[NUMENTRIES]   = { 5.0*eV, 6.69*eV, 7.50*eV };
    G4double cathmetal_RIND[NUMENTRIES] = { 1.51, 1.57, 1.61 };     // ref index
    G4double cathmetal_ABSL[NUMENTRIES] = { 1.e-20*m,  1.e-20*m,  1.e-20*m };// atten length
    //  G4double cathmetal_ABSL[NUMENTRIES] = { 3.0*cm, 3.0*cm, 3.0*cm };// atten length
    G4MaterialPropertiesTable *cathmetal_mt = new G4MaterialPropertiesTable();
    cathmetal_mt->AddProperty("RINDEX", cathmetal_PP, cathmetal_RIND,NUMENTRIES);
    cathmetal_mt->AddProperty("ABSLENGTH", cathmetal_PP, cathmetal_ABSL, NUMENTRIES);
    cathmetalAl->SetMaterialPropertiesTable(cathmetal_mt);
  }

  
    //concrete
  G4Material* concrete = new G4Material(name="Concrete", density=2.3*g/cm3, ncomponents=6);
  concrete->AddElement(Si, 0.227915);
  concrete->AddElement(O, 0.60541);
  concrete->AddElement(H, 0.09972);
  concrete->AddElement(Ca, 0.04986);
  concrete->AddElement(Al, 0.014245);
  concrete->AddElement(Fe, 0.00285);


  //water
  G4Material* water = new G4Material(name="water", density=1.00*g/cm3, ncomponents=2);
  water->AddElement(H , 2);
  water->AddElement(O , 1);

  
  double temperature, pressure;
  G4Material* vacuum = new G4Material(name="Vacuum", density=2.376e-15*g/cm3, ncomponents=1,
				      kStateGas,temperature=300*kelvin,pressure=2.0e-7*bar);
  vacuum->AddMaterial(air, 1.);
  

  //G4Material* rock =  NISTman->FindOrBuildMaterial("G4_WATER");
  G4Material* rock = new G4Material("StandardRock",
                                     2.65*CLHEP::g/CLHEP::cm3, 1, kStateSolid);
  rock->AddElement(Na, 1);

  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;


  const bool checkOverlays = false;
  //const bool checkOverlays = true;
  
  //-------------------------------------------------------------------------
  // Detector geometry
  //-------------------------------------------------------------------------
  //------------------------------ world
  G4Box * world_box
    = new G4Box("world", 500*meter,500*meter,depth_tunnel+1*meter); // half lengths
  
  G4LogicalVolume * world_log
    = new G4LogicalVolume(world_box, air, "world",0,0,0);

  G4VPhysicalVolume * world_phys
    = new G4PVPlacement(0,G4ThreeVector(0,0,0*meter),world_log,"world", 0,false,0,checkOverlays);
  
  world_log->SetVisAttributes(G4VisAttributes::Invisible);

  
  //------------------------------ molasse rock
  G4Box * molasse_box
    = new G4Box("molasse",500*meter, 500*meter, depth_tunnel);

  G4LogicalVolume* fMolasse_log
    = new G4LogicalVolume(molasse_box, rock, "molasse",0,0,0);

  new G4PVPlacement(0, // rotation
		    G4ThreeVector(0,0,0), // position
		    fMolasse_log, // LV
		    "molasse", // name
		    world_log, // mother volume
		    false, // boolean operation ?
		    0, // copy number
		    checkOverlays); 
		    
  G4VisAttributes* molasse_logVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  molasse_logVisAtt->SetForceWireframe(true);
  fMolasse_log->SetVisAttributes(molasse_logVisAtt);
  
  
  // --- tunnel
  G4Box * tunnel_box = new G4Box("tunnel",500*meter, 500*meter, height_tunnel/2);
  
  G4LogicalVolume*
  fTunnel_log  = new G4LogicalVolume(tunnel_box,
				     air,
				     "tunnel",
				     0,0,0);
  
  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),
		    fTunnel_log,
		    "tunnel",
		    fMolasse_log,
		    false,
		    0,
		    checkOverlays);
  
  G4VisAttributes* tunnel_logVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  tunnel_logVisAtt->SetForceWireframe(true);
  fTunnel_log->SetVisAttributes(tunnel_logVisAtt);
  

  // ---- detector
  bool det = true;
  if (det) {
    
    // --- one experiment -------------------------------------
    G4VSolid* calorimeterS
      = new G4Box("Calorimeter",     // its name
		  (wCell+2*crystalCase)*nGrid/2., (wCell+2*crystalCase)*nGrid/2., distanceLayer*nlay/2.); // its size
    
    G4LogicalVolume* calorLV
      = new G4LogicalVolume(calorimeterS,     // its solid
			    defaultMaterial,  // its material
			    "Calorimeter");   // its name
    
    new G4PVPlacement(0,                // no rotation
		      G4ThreeVector(0,0,0),  // at (0,0,0)
		      calorLV,          // its logical volume                         
		      "Calorimeter",    // its name
		      fTunnel_log,          // its mother  volume
		      false,            // no boolean operation
		      0,                // copy number
		      checkOverlays);  // checking overlaps 
    G4VisAttributes* exp_logVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
    exp_logVisAtt->SetForceWireframe(true);
    calorLV->SetVisAttributes(exp_logVisAtt);
    //calorLV->SetVisAttributes(G4VisAttributes::Invisible);


    G4OpticalSurface* crystal_opsurf = new G4OpticalSurface("crystal_opsurf", unified, polished, dielectric_metal);
    {
      const G4int NUM = 2;
      G4double case_PP[NUM]   = { 0.1*eV, 10.50*eV };
      G4double case_REFL[NUM] = { 0.7, 0.7 };
      G4MaterialPropertiesTable* case_mt = new G4MaterialPropertiesTable();
      case_mt->AddProperty("REFLECTIVITY", case_PP, case_REFL, NUM);      
      crystal_opsurf->SetMaterialPropertiesTable(case_mt);
    }   
    
    
    //                                 
    // The Layer --------------------------------------------------
    //
    for (int ilay=0; ilay<nlay; ++ilay) {
      
      ostringstream layname;
      layname << "Layer_" << ilay;

      G4VSolid* layerS 
	= new G4Box(layname.str(), (wCell+2*crystalCase)*nGrid/2., (wCell+2*crystalCase)*nGrid/2., distanceLayer/2.);
    
      G4LogicalVolume* layerLV
	= new G4LogicalVolume(layerS, air, layname.str());         // its name
    
      layerLV->SetVisAttributes(G4VisAttributes::Invisible);

      new G4PVPlacement(0,
			G4ThreeVector(0,0,distanceLayer*(ilay-double(nlay-1)/2)),
			layerLV,
			layname.str().c_str(),
			calorLV,
			false,
			0,
			checkOverlays);
    
    

      // one sci-bar grid row -------------------------------------------
      
      for (int irow=0; irow<nGrid; ++irow) {

	ostringstream rowname;
	rowname << "DetRow_" << ilay << "_" << irow;
      
	G4VSolid* rowS 
	  = new G4Box(rowname.str(), (wCell+2*crystalCase)*nGrid/2., (wCell+2*crystalCase)/2., distanceLayer/2.);
    
	G4LogicalVolume* rowLV
	  = new G4LogicalVolume(rowS, air, rowname.str());
      
	new G4PVPlacement(0,
			  G4ThreeVector(0, (wCell+2*crystalCase)*(irow-double(nGrid-1)/2), 0),
			  rowLV,
			  rowname.str(),
			  layerLV,
			  false,
			  0,
			  checkOverlays);
          
	rowLV->SetVisAttributes(G4VisAttributes::Invisible);

	
	// the individual sci-cols in the row -------------------------------
	
	for (int icol=0; icol<nGrid; ++icol) {
	  
	  ostringstream colname;
	  colname << "DetCol_" << ilay << "_" << irow << "_" << icol;
	  

	  G4VSolid* colS 
	    = new G4Box(colname.str(), (wCell+2*crystalCase)/2, (wCell+2*crystalCase)/2, distanceLayer/2);
    
	  G4LogicalVolume* colLV
	    = new G4LogicalVolume(colS, air, colname.str());

	  new G4PVPlacement(0,
			    G4ThreeVector((wCell+2*crystalCase)*(icol-double(nGrid-1)/2), 0, 0),
			    colLV,
			    colname.str(),
			    rowLV,
			    false,
			    0,
			    checkOverlays);

	  colLV->SetVisAttributes(G4VisAttributes::Invisible);


	  
	  // the individual crystal+casings -------------------------------

	  ostringstream assemblyname;
	  assemblyname << "CrystalAssembly_" << ilay << "_" << irow << "_" << icol;

	  G4VSolid* crystalAssemblyS
	    = new G4Box(assemblyname.str(), (wCell+2*crystalCase)/2, (wCell+2*crystalCase)/2, (heightCrystal+crystalCase)/2);
    
	  G4LogicalVolume* crystalAssemblyLV
	    = new G4LogicalVolume(crystalAssemblyS, metalCu, assemblyname.str());
	  
	  G4VPhysicalVolume* pv_case =
	  new G4PVPlacement(0, G4ThreeVector(0,0,-distanceLayer/2 + (heightCrystal + crystalCase)/2),
			    crystalAssemblyLV, assemblyname.str(), colLV, false, 0, checkOverlays);

	  crystalAssemblyLV->SetVisAttributes(G4VisAttributes::Invisible);
    
    
	  // NOW THE CRYSTAL METAL BOX --------------------------------------
	  ostringstream sciname;
	  sciname << "SciBlock_" << ilay << "_" << irow << "_" << icol;

	  G4VSolid* sciS = new G4Box(sciname.str(), wCell/2, wCell/2, heightCrystal/2);

	  /*
	  ostringstream casename;
	  casename << "Case_" << ilay << "_" << irow << "_" << icol;

	  G4RotationMatrix* rotThis = new G4RotationMatrix();
	  G4ThreeVector moveThis(0,0,crystalCase/2);
	  G4SubtractionSolid* crystalcaseS = new G4SubtractionSolid(sciname.str(), crystalAssemblyS, sciS, rotThis, moveThis);
	  
	  G4LogicalVolume* crystalcaseLV = new G4LogicalVolume(crystalcaseS, metalCu, casename.str()); 

	  G4VPhysicalVolume* pv_case =
	    new G4PVPlacement(0, G4ThreeVector(0,0,0),
			      crystalcaseLV, casename.str(), crystalAssemblyLV, false, 0, checkOverlays);
	  
	  
	  G4VisAttributes* case_logVisAtt = new G4VisAttributes(G4Colour(.3, 0.3, .5, 0.5));
	  case_logVisAtt->SetForceWireframe(true);
	  //case_logVisAtt->SetForceSolid(true);
	  crystalcaseLV->SetVisAttributes(case_logVisAtt);
	  */
  
	  // AND NOW the individual crystals -----------------------------------------
    
	  G4LogicalVolume* sciLV = new G4LogicalVolume(sciS, NaI, sciname.str()); 

	  G4VPhysicalVolume* pv_sci =
	    new G4PVPlacement(0, G4ThreeVector(0,0,crystalCase/2),
			      sciLV, sciname.str(), crystalAssemblyLV, false, 0, checkOverlays);

	  // ostringst
	  
	  //G4LogicalBorderSurface* pmt_surf = 
	  new G4LogicalBorderSurface("crystal_surf", pv_sci, pv_case, crystal_opsurf);
	  //new G4LogicalBorderSurface("crystal_surf", pv_case, pv_sci, crystal_opsurf);
    
    
	  G4VisAttributes* sci_logVisAtt = new G4VisAttributes(G4Colour(.0, 0.6, .2, 0.1));
	  //sci_logVisAtt->SetForceWireframe(true);
	  sci_logVisAtt->SetForceSolid(true);
	  sciLV->SetVisAttributes(sci_logVisAtt);



    
	  // The photocathode -----------------------------------------------

	  ostringstream cathodename;
	  cathodename << "Cathode_" << ilay << "_" << irow << "_" << icol;
	  
	  const double cathodeHeight = 1*millimeter;
	  G4VSolid* cathodeS = new G4Tubs(cathodename.str(), 0, radiusPMT, cathodeHeight/2, 0*degree, 360*degree);
    
	  G4LogicalVolume* cathodeLV
	    = new G4LogicalVolume(cathodeS,           // its solid
				  quartz,              // its material
				  cathodename.str());         // its name
    
	  new G4PVPlacement(0, G4ThreeVector(0, 0, -distanceLayer/2 + heightCrystal + crystalCase + cathodeHeight/2),
			    cathodeLV, cathodename.str(), colLV, false, 0, checkOverlays);
	  
	  
    
	  /*
	    G4double phcath_PP[NUM]   = { 6.00*eV, 7.50*eV };
	    // G4double phcath_REFL[NUM] = { 0.0, 0.0};
	    // G4MaterialPropertiesTable* phcath_mt = new G4MaterialPropertiesTable();
	    // phcath_mt->AddProperty("REFLECTIVITY", phcath_PP, phcath_REFL, NUM);
	    // phcath_opsurf->SetMaterialPropertiesTable(phcath_mt);
	    
	    
	    // **Photocathode surface properties
	    G4double photocath_EFF[NUM]={1.,1.}; //Enables 'detection' of photons
	    G4double photocath_ReR[NUM]={1.92,1.92};
	    G4double photocath_ImR[NUM]={1.69,1.69};
	    G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
	    photocath_mt->AddProperty("EFFICIENCY",phcath_PP,photocath_EFF,NUM);
	    photocath_mt->AddProperty("REALRINDEX",phcath_PP,photocath_ReR,NUM);
	    photocath_mt->AddProperty("IMAGINARYRINDEX",phcath_PP,photocath_ImR,NUM);
	    G4OpticalSurface* photocath_opsurf=
	    new G4OpticalSurface("photocath_opsurf",glisur,polished,
	    dielectric_metal);
	    photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);
	  */
	  
	  G4VisAttributes* cathode_logVisAtt = new G4VisAttributes(G4Colour(1.0, 0.1, 0.1, 0.05));
	  //cathode_logVisAtt->SetForceWireframe(true);
	  cathode_logVisAtt->SetForceSolid(true);
	  cathodeLV->SetVisAttributes(cathode_logVisAtt);

    
	  // The PMT --------------------------------------

	  ostringstream pmtname;
	  pmtname << "PMT_" << ilay << "_" << irow << "_" << icol;

	  G4VSolid* pmtS
	    = new G4Tubs(pmtname.str(), 0, radiusPMT, heightPMT/2, 0*degree, 360*degree);
	  
	  G4LogicalVolume* pmtLV
	    = new G4LogicalVolume(pmtS,           // its solid
				  vacuum,              // its material
				  pmtname.str());         // its name
	  
	  new G4PVPlacement(0, G4ThreeVector(0, 0, -distanceLayer/2 + heightCrystal + crystalCase + cathodeHeight + heightPMT/2),
			    pmtLV, pmtname.str(), colLV, false, 0, checkOverlays);
	  
	  G4VisAttributes* pmt_logVisAtt = new G4VisAttributes(G4Colour(0.8,0.0,0.5, 0.1));
	  pmt_logVisAtt->SetForceSolid(true);
	  pmt_logVisAtt->SetForceAuxEdgeVisible (true);
	  pmtLV->SetVisAttributes(pmt_logVisAtt);
	  
	} // end loop col
      } // end loop row
    }// end loop layers
  }
  
  return world_phys;
}


void DetectorConstructionDet::ConstructSDandField()
{
  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // 
  // Sensitive detectors
  //
  // int nofChannel = 3 * 20 * 20;  
  fSciSD = new DetectorSD("Scintillator", "ScintillatorHitsCollection", nlay, nGrid);
  //SetSensitiveDetector("SciCol", sciSD);
  //SetSensitiveDetector("SciBlock", fSciSD);


  PMTSD* aSD = new PMTSD("/Det/pmtSD");
  //SetSensitiveDetector("Cathode", aSD);
		       
  

  
  /*
  // 
  // Magnetic field
  //
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  */
  
  // Register the field messenger for deleting
  // G4AutoDelete::Register(fMagFieldMessenger);
}

