#include "G4RunManager.hh"

#if defined(G4UI_USE_QT)
#include "G4UIQt.hh"
#endif
 

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"

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

  if (argc>1 && argc!=3) {
    cout << "either start without arguments for GUI, or specify: mu+/mu-  nEvt" << endl;
    return 1;
  }
  
  const bool batch = (argc==3);
  string proj = "mu+";
  int nEvt = 10;
  if (batch) {
    proj = argv[1];
    nEvt = atoi(argv[2]);
    cout << "BATCH mode" << endl;
    cout << "particle: " << proj  << " n=" << nEvt<< endl;
  }

  DetectorConstruction* detector = new DetectorConstructionDet();
  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(new PhysicsList);
  
  runManager->SetUserAction(new PrimaryGeneratorActionDet(proj));
  // runManager->SetUserAction(new SteppingActionDet());
  
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
  
#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;

  return 0;
}

