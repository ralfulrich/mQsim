
#if defined(G4UI_USE_QT)
#include "G4UIQt.hh"
#endif
 

#include "DetectorConstruction.hh"
#include "DetectorConstructionDet.hh"
#include "PhysicsList.hh"


//#ifdef G4MULTITHREADED
//#warning multi-threaded
//#include "G4MTRunManager.hh"
//#else
#warning not-multi-threaded
#include "G4RunManager.hh"
//#endif

#include "G4UImanager.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"

#ifdef G4VIS_USE
#warning vis-use
#include "G4VisExecutive.hh"
#endif


#ifdef G4UI_USE
#warning ui-use
#include "G4UIExecutive.hh"
#endif

#include "G4UItcsh"

#include <iostream>
using namespace std;





int
main(int argc, char** argv)
{

#ifdef G4UI_USE
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }
#endif

  //#ifdef G4MULTITHREADED
  //  auto runManager = new G4MTRunManager;
  //#else
  auto runManager = new G4RunManager;
  //#endif


#ifdef G4VIS_USE
  // Visualization manager construction
  auto visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  if (argc>1 && argc!=4) {
    cout << "either start without arguments for GUI, or specify: mu+/mu-/mpc-  nEvt  mQ" << endl;
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


  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();
  
#ifdef G4VIS_USE
  UImanager->ApplyCommand("/control/execute vis.mac"); 
  G4UIsession* session = new G4UIterminal(new G4UItcsh);
  session->SessionStart();
#else
  UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
    
#if defined(G4UI_USE_QT)
  if (!batch) {
    G4UIQt* ui = new G4UIQt(argc, argv);
    ui->SessionStart();
    // the line next to the "SessionStart" is necessary to finish the session
    delete ui;
  } else {
    runManager->BeamOn(nEvt);
  }
#else
  runManager->BeamOn(nEvt);
#endif

  cout << " TIME: " << gun->GetSeonds() << " seconds" << endl;

#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;

  
  return 0;
}

