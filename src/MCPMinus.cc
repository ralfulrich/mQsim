
#include "MCPMinus.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

// #include "MCPDecayChannel.hh"
//#include "G4DecayTable.hh"

double MCPMinus::fgQE = 0.001;

// ######################################################################
// ###                          MCPMINUS                             ###
// ######################################################################
MCPMinus* MCPMinus::theInstance = 0;

MCPMinus* MCPMinus::Definition()
{
  if (theInstance !=0) return theInstance;

  double mass = 20*GeV;
  double charge = -fgQE * eplus;
  
  const G4String name = "mcp-";
  // search in particle table]
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* anInstance = pTable->FindParticle(name);
  if (anInstance ==0)
  {
  // create particle
  //
  //    Arguments for constructor are as follows
  //               name             mass          width         charge
  //             2*spin           parity  C-conjugation
  //          2*Isospin       2*Isospin3       G-parity
  //               type    lepton number  baryon number   PDG encoding
  //             stable         lifetime    decay table
  //             shortlived      subType    anti_encoding
  anInstance = new G4ParticleDefinition(
                 name,     mass, 2.99598e-16*MeV, charge, 
		    1,                0,                0,          
		    0,                0,                0,             
	     "lepton",                1,                0,          19,
		true,       1e10*second,             NULL,
                false,              "mcp"
              );
    // Bohr Magnetron
   G4double muB =  -0.5*eplus*hbar_Planck/(anInstance->GetPDGMass()/c_squared) ;
   
   anInstance->SetPDGMagneticMoment( muB * 1.0011659209);

  //create Decay Table 
  //G4DecayTable* table = new G4DecayTable();

  // create a decay channel
  //G4VDecayChannel* mode = new MCPDecayChannel("mu-",1.00);
  //table->Insert(mode);
  //anInstance->SetDecayTable(table);
  }
  theInstance = reinterpret_cast<MCPMinus*>(anInstance);
  return theInstance;
}

MCPMinus*  MCPMinus::MCPMinusDefinition()
{
  return Definition();
}

MCPMinus*  MCPMinus::mCPMinus()
{
  return Definition();
}

