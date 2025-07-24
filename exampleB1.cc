#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"
#include "B1PhysicsList.hh"
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"
#include "FTFP_BERT.hh"
#include "FTFP_BERT_HP.hh"
#include "QGSP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

int main(int argc, char** argv)
{
  // 첫 번째 인자는 파일 이름(pGun.mac)
  std::string macroFile = argv[1];

  // Cluster and Process ID arguments
  G4int clusterId = std::stoi(argv[2]);
  G4int procId = std::stoi(argv[3]);

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
  // Set random seed using cluster and process IDs
  G4long seed = clusterId * 1000 + procId;
  G4Random::setTheSeed(seed);
  G4cout << "Random seed set to: " << seed << G4endl;

  // 환경변수 설정 (파일 이름용)
  std::string jobId = std::to_string(clusterId) + "_" + std::to_string(procId);
  setenv("B1_JOB_ID", jobId.c_str(), 1);
  
  // Construct the default run manager
  #ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  #else
  G4RunManager* runManager = new G4RunManager;
  #endif

  // Set mandatory initialization classes
  runManager->SetUserInitialization(new B1DetectorConstruction());

  // Physics list
  G4VModularPhysicsList* phys = new FTFP_BERT_HP;
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  phys->RegisterPhysics(opticalPhysics);
  runManager->SetUserInitialization(phys);

  // User action initialization
  runManager->SetUserInitialization(new B1ActionInitialization());

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/control/execute " + macroFile);  // Use macro file name

  delete runManager;
  return 0;
}
