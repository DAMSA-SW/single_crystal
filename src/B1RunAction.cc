//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1RunAction.cc 87359 2014-12-01 16:04:27Z gcosmo $
//
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1Run.hh"
#include "B1Analysis.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <math.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction()
: G4UserRunAction()
{ 
  // add new units for dose
  // 
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;  
  const G4double picogray  = 1.e-12*gray;
   
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);        

  auto analysisManager = G4RootAnalysisManager::Instance();
 
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  analysisManager->SetVerboseLevel(1);

   // jobId 환경 변수 가져오기
   const char* jobIdEnv = std::getenv("B1_JOB_ID");
  
   // 결과 파일 이름 설정 (jobId만 사용하여 고유한 이름으로 생성)
   G4String fileName = jobIdEnv ? 
       "Little_" + G4String(jobIdEnv) + ".root" : 
       "Little_default.root";  // 기본 파일 이름 설정
 
   analysisManager->SetFileName(fileName);
   
  std::vector<double> eAxis;
  //eAxis.push_back(0.1); 
  for (double i = 0; i < 5; i += 0.1){eAxis.push_back(i);}
  /*for (double i = 1E0; i < 1E1; i += 1E0){eAxis.push_back(i);}
  for (double i = 1E1; i < 1E2; i += 1E1){eAxis.push_back(i);}
  for (double i = 1E2; i < 1E3; i += 1E2){eAxis.push_back(i);}
  for (double i = 1E3; i < 1E4; i += 1E3){eAxis.push_back(i);}*/
  std::vector<double> tAxis;
  //for (double i = 0.0; i < 1E-2; i += 1E-3){tAxis.push_back(i);}
  //for (double i = 1E-2; i < 1E-1; i += 1E-2){tAxis.push_back(i);}
  for (double i = 0.0; i < 1.571; i += 1E-1){tAxis.push_back(i);}
  std::vector<double> zAxis;
  std::vector<double> ztAxis;
  for (double i =   0.0; i < 11.1; i += 0.1){ztAxis.push_back(i);}
  for (double i =   0.0; i < 50; i += 1){zAxis.push_back(i);}
  std::vector<double> xAxis;
  for (double i =   0.0; i < 13.; i += 0.1){xAxis.push_back(i);}
  std::vector<double> timeAxis;
  for (double i =   0.0; i < 100; i += 1){timeAxis.push_back(i);}
  std::vector<double> wzAxis;
  std::vector<double> wrAxis;
  for (double i =   -100.; i < 200.; i += 1){wzAxis.push_back(i);}
  for (double i =   290; i < 410.; i += 1){wrAxis.push_back(i);}
  std::vector<double> peAxis;
  for (double i =   20.0; i < 301; i += 10){peAxis.push_back(i);}
  std::vector<double> yAxis;
  for (double i =   -25.0; i < 26; i += 1){yAxis.push_back(i);}
  std::vector<double> edAxis;
  for (double i =   0; i < 5; i += 0.1){edAxis.push_back(i);}


  analysisManager->CreateH2("w_all_zy", "All Particles Z [cm] Y [cm] in Wold", zAxis, yAxis);                           //0
  analysisManager->CreateH2("w_electron_zy", "Electron Z [cm] Y [cm] in Wold", zAxis, yAxis);                      //1
  analysisManager->CreateH2("w_photon_zy", "Photon Z [cm] Y [cm] in Wold", zAxis, yAxis);                        //2
  analysisManager->CreateH2("w_all_zx", "All Particles Z [cm] X [cm] in Wold", zAxis, yAxis);                        //3
  analysisManager->CreateH2("w_neutron_zy", "Neutron Z [cm] Y [cm] in Wold", zAxis, yAxis);                       //4

  analysisManager->CreateH2("t_electron_zy", "electron Z [cm] Y [cm] in target", ztAxis, xAxis);                              //5
  analysisManager->CreateH2("t_electron_xy", "electron X [cm] Y [cm] in target", xAxis, xAxis);                               //6
  analysisManager->CreateH2("t_electron_et", "electron E [MeV] (1 Mev to 10 GeV)  Theta [rad] in target", eAxis, tAxis);     //7
  analysisManager->CreateH2("t_electron_er", "electron E [MeV] (1 Mev to 10 GeV)  R [cm] in target", eAxis, xAxis);           //8

  analysisManager->CreateH2("t_photon_zy", "photon Z [cm] Y [cm] in target", ztAxis, xAxis);                              //9
  analysisManager->CreateH2("t_photon_xy", "photon X [cm] Y [cm] in target", xAxis, xAxis);                               //10
  analysisManager->CreateH2("t_photon_et", "photon E [MeV] (1 Mev to 10 GeV)  Theta [rad] in target", eAxis, tAxis);      //11
  analysisManager->CreateH2("t_photon_er", "photon E [MeV] (1 Mev to 10 GeV)  R [cm] in target", eAxis, xAxis);           //12

  analysisManager->CreateH2("t_proton_zy", "proton Z [cm] Y [cm] in target", ztAxis, xAxis);                              //13
  analysisManager->CreateH2("t_proton_xy", "proton X [cm] Y [cm] in target", xAxis, xAxis);                               //14
  analysisManager->CreateH2("t_proton_et", "proton E [MeV] (1 Mev to 10 GeV)  Theta [rad] in target", eAxis, tAxis);      //15
  analysisManager->CreateH2("t_proton_er", "proton E [MeV] (1 Mev to 10 GeV)  R [cm] in target", eAxis, xAxis);           //16

  analysisManager->CreateH2("t_neutron_zy", "neutron Z [cm] Y [cm] in target", ztAxis, xAxis);                              //17
  analysisManager->CreateH2("t_neutron_xy", "neutron X [cm] Y [cm] in target", xAxis, xAxis);                               //18
  analysisManager->CreateH2("t_neutron_et", "neutron E [MeV] (1 Mev to 10 GeV)  Theta [rad] in target", eAxis, tAxis);      //19
  analysisManager->CreateH2("t_neutron_er", "neutron E [MeV] (1 Mev to 10 GeV)  R [cm] in target", eAxis, xAxis);           //20




  analysisManager->CreateH2("d1_electron_zy", "electron Z [cm] Y [cm] in detector1", zAxis, xAxis);                             //21
  analysisManager->CreateH2("d1_electron_xy", "electron X [cm] Y [cm] in detector1", xAxis, xAxis);                             //22
  analysisManager->CreateH2("d1_electron_et", "electron E [MeV] (1 Mev to 10 GeV)  Theta [rad] in detector1", eAxis, tAxis);    //23
  analysisManager->CreateH2("d1_electron_er", "electron E [MeV] (1 Mev to 10 GeV)  R [cm] in detector1", eAxis, xAxis);         //24

  analysisManager->CreateH2("d1_photon_zy", "photon Z [cm] Y [cm] in detector1", zAxis, xAxis);                             //25
  analysisManager->CreateH2("d1_photon_xy", "photon X [cm] Y [cm] in detector1", xAxis, xAxis);                             //26
  analysisManager->CreateH2("d1_photon_et", "photon E [MeV] (1 Mev to 10 GeV)  Theta [rad] in detector1", eAxis, tAxis);    //27
  analysisManager->CreateH2("d1_photon_er", "photon E [MeV] (1 Mev to 10 GeV)  R [cm] in detector1", eAxis, xAxis);         //28

  analysisManager->CreateH2("d1_proton_zy", "proton Z [cm] Y [cm] in detector1", zAxis, xAxis);                             //29
  analysisManager->CreateH2("d1_proton_xy", "proton X [cm] Y [cm] in detector1", xAxis, xAxis);                             //30
  analysisManager->CreateH2("d1_proton_et", "proton E [MeV] (1 Mev to 10 GeV) Theta [rad] in detector1", eAxis, tAxis);    //31
  analysisManager->CreateH2("d1_proton_er", "proton E [MeV] (1 Mev to 10 GeV) R [cm] in detector1", eAxis, xAxis);         //32

  analysisManager->CreateH2("d1_neutron_zy", "neutron Z [cm] Y [cm] in detector1", zAxis, xAxis);                             //33
  analysisManager->CreateH2("d1_neutron_xy", "neutron X [cm] Y [cm] in detector1", xAxis, xAxis);                             //34
  analysisManager->CreateH2("d1_neutron_et", "neutron E [MeV] (1 Mev to 10 GeV) Theta [rad] in detector1", eAxis, tAxis);    //35
  analysisManager->CreateH2("d1_neutron_er", "neutron E [MeV] (1 Mev to 10 GeV) R [cm] in detector1", eAxis, xAxis);         //36

  analysisManager->CreateH2("d1_electron_edt", "Electron energy deposit [MeV] Global Time [ns] in the detector1", edAxis, timeAxis); //37
  analysisManager->CreateH2("d1_photon_edt", "Photon energy deposit [MeV] Global Time [ns] in the detector1", edAxis, timeAxis); //38
  analysisManager->CreateH2("d1_proton_edt", "Proton energy deposit [MeV] Global Time [ns] in the detector1", edAxis, timeAxis); //39
  analysisManager->CreateH2("d1_neutron_edt", "Neutron energy deposit [MeV] Global Time [ns] in the detector1", edAxis, timeAxis); //40
  analysisManager->CreateH2("d1_all_edt", "All energy deposit [MeV] Global Time [ns] in the detector1", edAxis, timeAxis); //41

  analysisManager->CreateH2("d2_electron_zy", "electron Z [cm] Y [cm] in detector2", zAxis, xAxis);                             //42
  analysisManager->CreateH2("d2_electron_xy", "electron X [cm] Y [cm] in detector2", xAxis, xAxis);                             //43
  analysisManager->CreateH2("d2_electron_et", "electron E [MeV] (1 Mev to 10 GeV) Theta [rad] in detector2", eAxis, tAxis);    //44
  analysisManager->CreateH2("d2_electron_er", "electron E [MeV] (1 Mev to 10 GeV) R [cm] in detector2", eAxis, xAxis);         //45

  analysisManager->CreateH2("d2_photon_zy", "photon Z [cm] Y [cm] in detector2", zAxis, xAxis);                             //46
  analysisManager->CreateH2("d2_photon_xy", "photon X [cm] Y [cm] in detector2", xAxis, xAxis);                             //47
  analysisManager->CreateH2("d2_photon_et", "photon E [MeV] (1 Mev to 10 GeV) Theta [rad] in detector2", eAxis, tAxis);    //48
  analysisManager->CreateH2("d2_photon_er", "photon E [MeV] (1 Mev to 10 GeV) R [cm] in detector2", eAxis, xAxis);         //49

  analysisManager->CreateH2("d2_proton_zy", "proton Z [cm] Y [cm] in detector2", zAxis, xAxis);                             //50
  analysisManager->CreateH2("d2_proton_xy", "proton X [cm] Y [cm] in detector2", xAxis, xAxis);                             //51
  analysisManager->CreateH2("d2_proton_et", "proton E [MeV] (1 Mev to 10 GeV) Theta [rad] in detector2", eAxis, tAxis);    //52
  analysisManager->CreateH2("d2_proton_er", "proton E [MeV] (1 Mev to 10 GeV) R [cm] in detector2", eAxis, xAxis);         //53

  analysisManager->CreateH2("d2_neutron_zy", "neutron Z [cm] Y [cm] in detector2", zAxis, xAxis);                             //54
  analysisManager->CreateH2("d2_neutron_xy", "neutron X [cm] Y [cm] in detector2", xAxis, xAxis);                             //55
  analysisManager->CreateH2("d2_neutron_et", "neutron E [MeV] (1 Mev to 10 GeV) Theta [rad] in detector2", eAxis, tAxis);    //56
  analysisManager->CreateH2("d2_neutron_er", "neutron E [MeV] (1 Mev to 10 GeV) R [cm] in detector2", eAxis, xAxis);         //57

  analysisManager->CreateH2("d2_electron_edt", "Electron energy deposit [MeV] Global Time [ns] in the detector2", edAxis, timeAxis); //58
  analysisManager->CreateH2("d2_photon_edt", "Photon energy deposit [MeV] Global Time [ns] in the detector2", edAxis, timeAxis); //59
  analysisManager->CreateH2("d2_proton_edt", "Proton energy deposit [MeV] Global Time [ns] in the detector2", edAxis, timeAxis); //60
  analysisManager->CreateH2("d2_neutron_edt", "Neutron energy deposit [MeV] Global Time [ns] in the detector2", edAxis, timeAxis); //61
  analysisManager->CreateH2("d2_all_edt", "All energy deposit [MeV] Global Time [ns] in the detector2", edAxis, timeAxis); //62

  analysisManager->CreateH2("escape_electron_et", "electron E [MeV] (1 Mev to 10 GeV)  Theta [rad] out of target", ztAxis, xAxis);  //63
  analysisManager->CreateH2("escape_positron_et", "positron E [MeV] (1 Mev to 10 GeV)  Theta [rad] out of target", eAxis, tAxis);      //64
  analysisManager->CreateH2("escape_photon_et", "photon E [MeV] (1 Mev to 10 GeV)  Theta [rad] out of target", ztAxis, xAxis);          //65
  analysisManager->CreateH2("escape_neutron_et", "neutron E [MeV] (1 Mev to 10 GeV)  Theta [rad] out of target", eAxis, tAxis);      //66
  analysisManager->CreateH2("escape_proton_et", "proton E [MeV] (1 Mev to 10 GeV)  Theta [rad] out of target", eAxis, tAxis);      //67

  analysisManager->CreateH2("first_d1_electron_et", "Electron energy deposit [MeV] Global Time [ns] in the detector1", eAxis, timeAxis); //68
  analysisManager->CreateH2("first_d1_positron_et", "Positron energy deposit [MeV] Global Time [ns] in the detector1", eAxis, timeAxis); //69
  analysisManager->CreateH2("first_d1_photon_et", "Photon energy deposit [MeV] Global Time [ns] in the detector1", eAxis, timeAxis); //70
  analysisManager->CreateH2("first_d1_neutron_et", "Neutron energy deposit [MeV] Global Time [ns] in the detector1", eAxis, timeAxis); //71
  analysisManager->CreateH2("first_d1_proton_et", "Proton energy deposit [MeV] Global Time [ns] in the detector1", eAxis, timeAxis); //72
  
  analysisManager->CreateH2("first_d2_electron_et", "Electron energy deposit [MeV] Global Time [ns] in the detector2", eAxis, timeAxis); //73
  analysisManager->CreateH2("first_d2_positron_et", "Positron energy deposit [MeV] Global Time [ns] in the detector2", eAxis, timeAxis); //74
  analysisManager->CreateH2("first_d2_photon_et", "Photon energy deposit [MeV] Global Time [ns] in the detector2", eAxis, timeAxis); //75
  analysisManager->CreateH2("first_d2_neutron_et", "Neutron energy deposit [MeV] Global Time [ns] in the detector2", eAxis, timeAxis); //76
  analysisManager->CreateH2("first_d2_proton_et", "Proton energy deposit [MeV] Global Time [ns] in the detector2", eAxis, timeAxis); //77

  analysisManager->CreateH2("first_d1_electron_zy", "Electron zy in the detector1", 133, 15.6, 15.6+13.3, 133, -13.3/2, 13.3/2); //78
  analysisManager->CreateH2("first_d1_positron_zy", "Positron zy in the detector1", 133, 15.6, 15.6+13.3, 133, -13.3/2, 13.3/2); //79
  analysisManager->CreateH2("first_d1_photon_zy", "Photon zy in the detector1", 133, 15.6, 15.6+13.3, 133, -13.3/2, 13.3/2); //80
  analysisManager->CreateH2("first_d1_neutron_zy", "Neutron zy in the detector1", 133, 15.6, 15.6+13.3, 133, -13.3/2, 13.3/2); //81
  analysisManager->CreateH2("first_d1_proton_zy", "Proton zy in the detector1", 133, 15.6, 15.6+13.3, 133, -13.3/2, 13.3/2); //82
  
  analysisManager->CreateH2("first_d2_electron_zy", "Electron zy in the detector2", 133, 34.4, 34.4+13.3, 133, -13.3/2, 13.3/2); //83
  analysisManager->CreateH2("first_d2_positron_zy", "Positron zy in the detector2", 133, 34.4, 34.4+13.3, 133, -13.3/2, 13.3/2); //84
  analysisManager->CreateH2("first_d2_photon_zy", "Photon zy in the detector2", 133, 34.4, 34.4+13.3, 133, -13.3/2, 13.3/2); //85
  analysisManager->CreateH2("first_d2_neutron_zy", "Neutron zy in the detector2", 133, 34.4, 34.4+13.3, 133, -13.3/2, 13.3/2); //86
  analysisManager->CreateH2("first_d2_proton_zy", "Proton zy in the detector2", 133, 34.4, 34.4+13.3, 133, -13.3/2, 13.3/2); //87

  analysisManager->CreateH2("first_d1_electron_ed", "Electron ed in the detector1", 133, 15.6, 15.6+13.3, 133, -13.3/2, 13.3/2); //88
  analysisManager->CreateH2("first_d1_positron_ed", "Positron ed in the detector1", 133, 15.6, 15.6+13.3, 133, -13.3/2, 13.3/2); //89
  analysisManager->CreateH2("first_d1_photon_ed", "Photon ed in the detector1", 133, 15.6, 15.6+13.3, 133, -13.3/2, 13.3/2); //90
  analysisManager->CreateH2("first_d1_neutron_ed", "Neutron ed in the detector1", 133, 15.6, 15.6+13.3, 133, -13.3/2, 13.3/2); //91
  analysisManager->CreateH2("first_d1_proton_ed", "Proton ed in the detector1", 133, 15.6, 15.6+13.3, 133, -13.3/2, 13.3/2); //92
  
  analysisManager->CreateH2("first_d2_electron_ed", "Electron ed in the detector2", 133, 34.4, 34.4+13.3, 133, -13.3/2, 13.3/2); //93
  analysisManager->CreateH2("first_d2_positron_ed", "Positron ed in the detector2", 133, 34.4, 34.4+13.3, 133, -13.3/2, 13.3/2); //94
  analysisManager->CreateH2("first_d2_photon_ed", "Photon ed in the detector2", 133, 34.4, 34.4+13.3, 133, -13.3/2, 13.3/2); //95
  analysisManager->CreateH2("first_d2_neutron_ed", "Neutron ed in the detector2", 133, 34.4, 34.4+13.3, 133, -13.3/2, 13.3/2); //96
  analysisManager->CreateH2("first_d2_proton_ed", "Proton ed in the detector2", 133, 34.4, 34.4+13.3, 133, -13.3/2, 13.3/2); //97

  analysisManager->CreateH2("escape_opticalphoton_et", "photon E [MeV] (1 Mev to 10 GeV)  Theta [rad] out of target", ztAxis, xAxis); //98                             //5
  analysisManager->CreateH2("produced_opticalphoton_et", "photon E [MeV] (1 Mev to 10 GeV)  Theta [rad] out of target", ztAxis, xAxis); //99                             //5


  analysisManager->CreateH1("d1_all_ft", "All particles from the target in the detector1", 30,0,30); //0
  analysisManager->CreateH1("d1_electron_ft", "Electron from the target in the detector1", 30,0,30); //1
  analysisManager->CreateH1("d1_photon_ft", "Photon from the target in the detector1", 30,0,30); //2
  analysisManager->CreateH1("d1_proton_ft", "Proton from the target in the detector1", 30,0,30); //3
  analysisManager->CreateH1("d1_neutron_ft", "Neutron from the target in the detector1", 30,0,30); //4
  analysisManager->CreateH1("d2_all_ft", "All particles from the target in the detector2", 30,0,30); //5
  analysisManager->CreateH1("d2_electron_ft", "Electron from the target in the detector2", 30,0,30); //6
  analysisManager->CreateH1("d2_photon_ft", "Photon from the target in the detector2", 30,0,30); //7
  analysisManager->CreateH1("d2_proton_ft", "Proton from the target in the detector2", 30,0,30); //8
  analysisManager->CreateH1("d2_neutron_ft", "Neutron from the target in the detector2", 30,0,30); //9

  analysisManager->CreateH1("d1_electron_ed", "Electron energy deposit [MeV] in the detector1", edAxis); //10
  analysisManager->CreateH1("d2_electron_ed", "Electron energy deposit [MeV] in the detector2", edAxis); //11
  analysisManager->CreateH1("d1_electron_e", "Electron Energy [MeV] in the detector1",eAxis); //12
  analysisManager->CreateH1("d1_proton_e", "Proton Energy [MeV] in the detector1",eAxis); //13
  analysisManager->CreateH1("d2_electron_e", "Electron Energy [MeV] in the detector2",eAxis); //14
  analysisManager->CreateH1("d2_proton_e", "Proton Energy [MeV] in the detector2",eAxis); //15
  analysisManager->CreateH1("d1_photon_e", "Photon Energy [MeV] in the detector1",eAxis); //16
  analysisManager->CreateH1("d1_neutron_e", "Neutron Energy [MeV] in the detector1",eAxis); //17
  analysisManager->CreateH1("d2_photon_e", "Photon Energy [MeV] in the detector2",eAxis); //18
  analysisManager->CreateH1("d2_neutron_e", "Neutron Energy [MeV] in the detector2",eAxis); //19
  analysisManager->CreateH1("w_all_ed", "Energy Deposit [MeV]",50,0,5); //20
  analysisManager->CreateH1("d1_opticalphoton_et", "Opticalphoton Energy [eV] in the detector1",300, 0, 300); //21
  analysisManager->CreateH1("d0_opticalphoton_et", "Opticalphoton Energy [eV] in the detector2",300, 0, 300); //22
  analysisManager->CreateH1("first_d1_electron_e", "Electron energy deposit [MeV] Global Time [ns] in the detector1", eAxis); //23
  analysisManager->CreateH1("first_d1_positron_te", "Positron energy deposit [MeV] Global Time [ns] in the detector1", eAxis); //24
  analysisManager->CreateH1("first_d1_photon_e", "Photon energy deposit [MeV] Global Time [ns] in the detector1", eAxis); //25
  analysisManager->CreateH1("first_d1_neutron_te", "Neutron energy deposit [MeV] Global Time [ns] in the detector1", timeAxis); //26
  analysisManager->CreateH1("first_d1_proton_te", "Proton energy deposit [MeV] Global Time [ns] in the detector1", timeAxis); //27
  analysisManager->CreateH1("optical_shape", "Optical shape in the detector1", 500, 200, 700); //28
  analysisManager->CreateH1("detected_shape", "Optical shape in the detector1", 500, 200, 700); //29
  analysisManager->CreateH1("entered_shape", "Optical shape in the detector1", 500, 200, 700); //30
  analysisManager->CreateH1("Photon Z-E", "Photon Z-E", 300, 0, 0.3); //31
  analysisManager->CreateH1("detected_shape_v2", "Optical shape in the detector1", 500, 200, 700); //32
  analysisManager->CreateH1("d1_opticalphoton_et_v2", "Opticalphoton Energy [eV] in the detector1",300, 0, 300); //33
  analysisManager->CreateH1("AbsLength", "AbsLength",300, 0, 300); //34
  analysisManager->CreateH1("outed_shape", "Optical shape in the detector1", 500, 200, 700); //35
  analysisManager->CreateH1("Reflected1", "Reflected1",100, 0, 100); //36
  analysisManager->CreateH1("Reflected2", "Reflected2",100, 0, 100); //37
  analysisManager->CreateH1("d2_opticalphoton_et", "Opticalphoton Energy [eV] in the detector1",300, 0, 300); //38
  analysisManager->CreateH1("detected2_shape", "Optical shape in the detector1", 500, 200, 700); //39
  analysisManager->CreateH1("entered2_shape", "Optical shape in the detector1", 500, 200, 700); //40
  analysisManager->CreateH1("detected2_shape_v2", "Optical shape in the detector1", 500, 200, 700); //41
  analysisManager->CreateH1("d2_opticalphoton_et_v2", "Opticalphoton Energy [eV] in the detector1",300, 0, 300); //42


}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* B1RunAction::GenerateRun()
{
  return new B1Run; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run*)
{ 
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  auto analysisManager = G4RootAnalysisManager::Instance();
  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
  auto analysisManager = G4RootAnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile(); 
 
  const B1Run* b1Run = static_cast<const B1Run*>(run);

  // Compute dose
  //
  G4double edep  = b1Run->GetEdep();
  G4double edep2 = b1Run->GetEdep2();
  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  const B1DetectorConstruction* detectorConstruction
   = static_cast<const B1DetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double mass = detectorConstruction->GetScoringVolume1()->GetMass();
  G4double dose = edep/mass;
  G4double rmsDose = rms/mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const B1PrimaryGeneratorAction* generatorAction
   = static_cast<const B1PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }
        
  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
  
  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << " Dose in scoring volume : " 
     << G4BestUnit(dose,"Dose") << " +- " << G4BestUnit(rmsDose,"Dose")
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
