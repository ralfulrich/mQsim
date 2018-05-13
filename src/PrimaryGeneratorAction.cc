#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"    

#include "MCPMinus.hh"

#include <TF1.h>
#include <TGraph.h>
#include <TRandom.h>
#include <TH1D.h>
#include <TFile.h>

#include <vector>

using namespace std;


// flux in 1 / (m2 sr s GeV)

double paramFlux(double*x, double*par) {
  
  double p = x[0]; // GeV
  
  double C = par[0];
  double H1 = par[1];
  double H2 = par[2];
  double H3 = par[3];
  double S2 = par[4];
  
  /*
    0.86 Æ 0.06
    0.133 Æ 0.002
   -2.521 Æ 0.004
   -5.78 Æ 0.03
   -2.11 Æ 0.03
   */
  
  double y = log10(p); // g in GeV

  double Hy = H1 * (y*y*y/2 - 5*y*y/2 + 3*y)
    + H2 * (-2*y*y*y/3 + 3*y*y - 10*y/3 + 1)
    + H3 * (y*y*y/6 - y*y/2 + y/3)
    + S2 * (y*y*y/3 - 2*y*y + 11*y/3 - 2);

  return C * pow(10, Hy);
}

// const double deg = asin(1.0) / 90.;


// theta propto cos^3 theta
// int cos^3 theta = 1/12(9*sin(theta) + sin(3theta)) = I(theta)
class ZenithCos3CDF {
public:
  static double dFdX(const double x)  { return pow(cos(x),3); }
  static double Integral(const double x) {
    return (9.*sin(x) + sin(3.*x)) / 12.; 
  }
};

// theta propto cos^3 theta sin theta
// int cos^3 theta sin theta = - 1/4 cos^4(theta)
class ZenithCos3SinCDF {
public:
  static double dFdX(const double x) { return pow(cos(x),3)*sin(x); }
  static double Integral(const double x) {
    return -pow(cos(x),4) / 4.; 
  }
};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
PrimaryGeneratorAction::PrimaryGeneratorAction(const std::string& name)
  : G4VUserPrimaryGeneratorAction(),
    fParticleGun(0),//, fMessenger(0)
    fCount(0)
{
  G4int n_particle = 1;
  G4ParticleGun* particleGun = new G4ParticleGun(n_particle);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(name);
  const int pdg = particle->GetPDGEncoding();
  
  particleGun->SetParticleDefinition(particle);
  fParticleGun = particleGun;

  fSpect = new TF1("funcFlux", paramFlux, 10, 100, 5);
  fSpect->SetParameters(0.86, 0.133, -2.521, -5.78, -2.11);
  
    
  // normalize by 1./(I(thetamax)-I(thetamin))
  vector<double> vecP, vecT;

  const double thetamin = 0*deg;
  const double thetamax = 70*deg;
  
  for (int i=0; i<100; ++i) {
    const double theta = thetamin + (thetamax-thetamin)/99 * i;
    
    double p = ZenithCos3SinCDF::Integral(theta);
    p /= (ZenithCos3SinCDF::Integral(thetamax) - ZenithCos3SinCDF::Integral(thetamin));
    vecT.push_back(theta);
    if (i==0) {
      vecP.push_back( 0 );
    } else if (i==99) {
      vecP.push_back( 1.0 );
    } else {
      vecP.push_back( p );
    }
  }
  fZenith = new TGraph(vecP.size(), &vecP.front(), &vecT.front());

  // the histograms
  if (name.find("mcp") == 0)
    return;

    
  fMuonsfromSurface = false;

  if (!fMuonsfromSurface) {
    
    ostringstream ename, zname;
    ename << "hist_e_" << pdg;
    zname << "hist_z_" << pdg;

    cout << "reading: " << ename.str() << " " << zname.str() << endl;
    
    TFile* data = TFile::Open("fluxes.root");
    if (!data) {
      cout << "cannot read fluxes.root" << endl;
      exit(1);
    }
    fHistEnergy  = (TH1D*)data->Get(ename.str().c_str());
    fHistZenith = (TH1D*)data->Get(zname.str().c_str());
    // data->Close();
    
    if (!fHistEnergy || !fHistZenith) {
      cout << "wrong input data" << endl;
      exit(1);
    }

    fHistZenith = (TH1D*)(fHistZenith->Clone("zenith_spect"));
    fHistEnergy = (TH1D*)(fHistZenith->Clone("energy_spect"));
    fHistZenith->SetDirectory(0);
    fHistEnergy->SetDirectory(0);
    data->Close();
    
    //particle = particleTable->FindParticle("mu-");
    //particleGun->SetParticleDefinition(particle);

    double fluxtot = 0;
    for (int i=0; i<fHistEnergy->GetNbinsX(); ++i) {
      double binw = (pow(10,fHistEnergy->GetXaxis()->GetBinLowEdge(i+1)+fHistEnergy->GetXaxis()->GetBinWidth(i+1)) -
		     pow(10,fHistEnergy->GetXaxis()->GetBinLowEdge(i+1)));

      fluxtot += fHistEnergy->GetBinContent(i+1);
      fHistEnergy->SetBinContent(i+1, fHistEnergy->GetBinContent(i+1)/binw);
      
    }

    cout << " FLUXTOT= " << fluxtot << " /s/m2" << endl;
    fRate = fluxtot * 10*10; // m2
    cout << " RATE= " << fRate << " /s" << endl;
    
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4AutoLock.hh"
namespace {
  G4Mutex PrimGenDestrMutex = G4MUTEX_INITIALIZER;
  //  G4Mutex PrimGenMutex = G4MUTEX_INITIALIZER;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  G4AutoLock lock(&PrimGenDestrMutex);
  delete fParticleGun;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void
PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fCount++;

  if (fParticleGun->GetParticleDefinition()->GetParticleName().find("mcp") == 0) {
    
    const double phi = 0;
    const double theta = 0;
    const double energy = 10*GeV;    
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(sin(theta)*sin(phi),sin(theta)*cos(phi),-cos(theta)));
    fParticleGun->SetParticleEnergy(energy);
    const double x = 0;
    const double y = 0;
    fParticleGun->SetParticlePosition(G4ThreeVector(x, y, 2*meter));
    
  } if (fMuonsfromSurface) {
   
    const double theta = fZenith->Eval(gRandom->Rndm());
    double energy = fSpect->GetRandom(10, 1000) * GeV;
  
    //   cout << "gun: e=" << energy/GeV << " z=" << theta/degree << endl;
  
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,sin(theta),-cos(theta)));
    fParticleGun->SetParticleEnergy(energy);
    fParticleGun->SetParticlePosition(G4ThreeVector(0.*cm, 0.*cm, 80*meter));

  } else {
    
    const double mass = fParticleGun->GetParticleDefinition()->GetPDGMass();
    
    const double phi = gRandom->Rndm() * 360*degree;
    //const double theta = fHistZenith->GetRandom() * degree;
    //const double energy = pow(10,fHistEnergy->GetRandom()) * GeV + mass;
    const double theta = 0;
    const double energy = 10*GeV;
    
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(sin(theta)*sin(phi),sin(theta)*cos(phi),-cos(theta)));
    fParticleGun->SetParticleEnergy(energy);
    //double tile = 10*meter;
    double tile = 0.01*meter;
    const double x = (gRandom->Rndm()-0.5) * tile ;
    const double y = (gRandom->Rndm()-0.5) * tile;
    fParticleGun->SetParticlePosition(G4ThreeVector(x, y, 2*meter));
  }
  
  fParticleGun->GeneratePrimaryVertex(anEvent); 
}
