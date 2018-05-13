#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "g4root.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
RunAction::RunAction()
  :G4UserRunAction()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //   G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);

  // Book histograms, ntuple
  //
  
  // Creating histograms
  analysisManager->CreateH1("edep","Edep in sci", 100, 0., 800*MeV);
  analysisManager->CreateH1("trackL","trackL in sci", 100, 0., 1*m);
  analysisManager->CreateH1("channels","Channels", 1200, 0., 1200);
  analysisManager->CreateH1("mult","Mult", 100, 0., 100);
  int nGrid = 15;
  analysisManager->CreateH2("layer1","layer1", nGrid, 0., nGrid, nGrid, 0, nGrid);
  analysisManager->CreateH2("layer2","layer2", nGrid, 0., nGrid, nGrid, 0, nGrid);
  analysisManager->CreateH2("layer3","layer3", nGrid, 0., nGrid, nGrid, 0, nGrid);
  analysisManager->CreateH2("layer4","layer4", nGrid, 0., nGrid, nGrid, 0, nGrid);

  // Create ntuple
  analysisManager->CreateNtuple("MCPsim", "hits");
  analysisManager->CreateNtupleDColumn("dEdX");
  analysisManager->CreateNtupleDColumn("nph");
  analysisManager->CreateNtupleDColumn("nPE");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("evtId");
  analysisManager->FinishNtuple();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);

    // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "B4";
  analysisManager->OpenFile(fileName);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void RunAction::EndOfRunAction(const G4Run*)
{
  // print histogram statistics
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    
    G4cout << " EAbs : mean = " 
       << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") << G4endl;
    
    G4cout << " EGap : mean = " 
       << G4BestUnit(analysisManager->GetH1(2)->mean(), "Length") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Length") << G4endl;
    
  }

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}




