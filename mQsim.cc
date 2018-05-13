#include "G4RunManager.hh"

#if defined(G4UI_USE_QT)
#include "G4UIQt.hh"
#endif
 

#include "DetectorConstruction.hh"
#include "DetectorConstructionDet.hh"
#include "PhysicsList.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include <iostream>
using namespace std;





int
main(int argc, char** argv)
{
 G4RunManager * runManager = new G4RunManager;

#ifdef G4VIS_USE
  // Visualization manager construction
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  if (argc>1 && argc!=4) {
    cout << "either start without arguments for GUI, or specify: mu+/mu-  nEvt" << endl;
    return 1;
  }
  
  const bool batch = (argc==4);
  //string proj = "mu-";
  string proj = "mcp-";
  int nEvt = 1;
  double Qe = 0.003;
  if (batch) {
    proj = argv[1];
    nEvt = atoi(argv[2]);
    Qe = atof(argv[3]);
    cout << "BATCH mode" << endl;
  }

  cout << "primary particle: " << proj  << " n=" << nEvt<<  " Qe=" << Qe << endl;

  DetectorConstructionDet* detector = new DetectorConstructionDet(Qe);//.5e-2);
  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(new PhysicsList(Qe));

  PrimaryGeneratorAction* gun = new PrimaryGeneratorAction(proj);
  runManager->SetUserAction(gun);
  runManager->SetUserAction(new SteppingAction(detector));
  runManager->SetUserAction(new StackingAction(detector));
  
  runManager->SetUserAction(new RunAction());
  runManager->SetUserAction(new EventAction());
  
  runManager->Initialize();
  
#if defined(G4UI_USE_QT)
  if (!batch) {
    G4UIQt* ui = new G4UIQt(argc, argv);
    ui->SessionStart();
    // the line next to the "SessionStart" is necessary to finish the session
    delete ui;
  } else {
    runManager->BeamOn(nEvt);
  }
#elif
  runManager->BeamOn(nEvt);
#endif

  cout << " TIME: " << gun->GetSeonds() << " seconds" << endl;

#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;

  
  return 0;
}

