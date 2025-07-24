//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under the terms  and *
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
// $Id: B1SteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1Analysis.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4INCLGlobals.hh"
#include "G4UImanager.hh"
#include <fstream>
#include <iomanip>
#include <cstdlib> // getenv 사용을 위한 헤더 추가
#include "G4Threading.hh"  // 스레드 ID 가져오기 위한 헤더 추가
#include "G4EventManager.hh"
#include "B1MyTrackInformation.hh"

thread_local std::map<int, int> gTrackIDtoTag;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume1(0),
  fScoringVolume2(0),
  fScoringVolume3(0),
  fScoringVolume4(0),
  fScoringVolume5(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  auto& outFile = fEventAction->GetOutputFile();
  auto analysisManager = G4RootAnalysisManager::Instance();

G4StepPoint* preStepPoint = step->GetPreStepPoint();
G4Track* tr = step->GetTrack();

// 두 번째로 시간 출력될 때 기준 시간 저장
          if (tr->GetTrackID()==1 && step->GetSecondaryInCurrentStep() && !step->GetSecondaryInCurrentStep()->empty()) {
            const auto* secondaries = step->GetSecondaryInCurrentStep();
            for (const auto& sec : *secondaries) {
              const_cast<G4Track*>(sec)->SetGlobalTime(0.);
            }
          }
        //G4cout << preStepPoint->GetGlobalTime() / ns << " ns" << G4endl;
  /*
  if (tr->GetParticleDefinition()->GetParticleName() != "opticalphoton"){
    B1EventAction* eventAction = (B1EventAction*)G4EventManager::GetEventManager()->GetUserEventAction();
    G4int uniqueCode = 100 * tr->GetTrackID() + tr->GetParticleDefinition()->GetPDGEncoding();
    //if (tr->GetParticleDefinition()->GetParticleName() != "opticalphoton") eventAction->LogStep(tr, step);
    G4String volumeName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
    eventAction->SetLastVolume(uniqueCode, volumeName);
  }
    */
    /*
    G4String pname = tr->GetDefinition()->GetParticleName();

      if (pname.contains("Bi207") || pname.contains("Pb207")) {
          const auto* secondaries = step->GetSecondaryInCurrentStep();
          if (secondaries && !secondaries->empty()) {
  
              // 관심 있는 입자 종류 및 에너지 (단위: keV)
              std::vector<std::pair<G4String, double>> target_particles = {
                  {"gamma", 569.697}, {"gamma", 1063.650}, {"gamma", 75.250},
                  {"gamma", 73.039}, {"gamma", 10.551}, {"gamma", 12.656},
                  {"gamma", 1770.210}, {"gamma", 85.235},
                  {"gamma", 84.741}, {"e-", 975.648}
              };
  
              double energy_tolerance = 0.5; // 허용 오차 (keV)
  
              bool matched = false;
              for (const auto& sec : *secondaries) {
                  G4String sec_name = sec->GetDefinition()->GetParticleName();
                  G4double sec_energy = sec->GetKineticEnergy() / keV; // keV로 변환
  
                  for (const auto& target : target_particles) {
                      if (sec_name == target.first &&
                          std::abs(sec_energy - target.second) < energy_tolerance) {
  
                          // 처음 매칭될 때만 헤더 출력
                          if (!matched) {
                              outFile << "Decay from: " << pname << G4endl;
                              matched = true;
                          }
  
                          outFile << "  --> Detected " << sec_name
                                 << " with E = " << sec_energy << " keV (matched target "
                                 << target.second << " keV)" << G4endl;
                      }
                  }
              }
            }
          }
    */
    /*
    G4String pname = tr->GetDefinition()->GetParticleName();
    if (pname.contains("Pb207")) {
    const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
    if (secondaries && !secondaries->empty()) {
        for (const auto& sec : *secondaries) {
            G4String secName = sec->GetDefinition()->GetParticleName();
            if (secName == "e-") {
              G4double e = sec->GetKineticEnergy() / MeV;
              G4double pz = sec->GetMomentumDirection().z();
              
              if (e > 0.5 && pz > 0) {
                  outFile << "Got conversion e-"
                          << " with E = " << e << " MeV"
                          << ", pz = " << pz
                          << G4endl;
              }   
            }
            if (secName == "gamma") {
              G4double e = sec->GetKineticEnergy() / MeV;
              G4double pz = sec->GetMomentumDirection().z();
              
              if (e > 0.5 && pz > 0) {
                  outFile << "Got gamma"
                          << " with E = " << e << " MeV"
                          << ", pz = " << pz
                          << G4endl;
              }   
            }
        }
      }
    }
    */
  //G4cout << preStepPoint->GetGlobalTime() / ns << G4endl;
  // Get volume of the current step
  if (!fScoringVolume1) { 
    const B1DetectorConstruction* detectorConstruction
      = static_cast<const B1DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume1 = detectorConstruction->GetScoringVolume1();   
    fScoringVolume2 = detectorConstruction->GetScoringVolume2();   
    fScoringVolume3 = detectorConstruction->GetScoringVolume3();   
    fScoringVolume4 = detectorConstruction->GetScoringVolume4();   
    fScoringVolume5 = detectorConstruction->GetScoringVolume5();   
  }
  
  G4LogicalVolume* volume 
    = preStepPoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
      
  // Check if we are in scoring volume
  G4String particleName = tr->GetDefinition()->GetParticleName();
  G4UImanager* UI = G4UImanager::GetUIpointer();

  G4LogicalVolume* preVolume = 
      preStepPoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  G4LogicalVolume* postVolume = nullptr;
  if(step->GetPostStepPoint()->GetTouchableHandle() && 
     step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()) {
      postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  }
// 현재 트랙이 tag를 갖고 있는지 확인
const B1MyTrackInformation* parentInfo =
    dynamic_cast<const B1MyTrackInformation*>(tr->GetUserInformation());
//if(parentInfo) G4cout << "  DONE?  " << parentInfo->GetTag() << G4endl;
if (parentInfo && step->GetSecondaryInCurrentStep()->size() > 0)
{/*
  if (pname.contains("Bi207") || pname.contains("Pb207")) gTrackIDtoTag[tr->GetTrackID()] = 4;
  else{
  */
    int parentTag = parentInfo->GetTag();  // 부모 tag 복사
      gTrackIDtoTag[tr->GetTrackID()] = parentTag;
  //}
}

  // Check scoring volume 1
  if (volume == fScoringVolume1) {
    auto x = preStepPoint->GetPosition()[0] / cm;
    auto y = preStepPoint->GetPosition()[1] / cm;
    auto z = preStepPoint->GetPosition()[2] / cm;
    analysisManager->FillH2(0, z, y);
    analysisManager->FillH2(3, z, x);
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 11) analysisManager->FillH2(1, z, y);
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 22) analysisManager->FillH2(2, z, y);
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2112) analysisManager->FillH2(4, z, y);
    auto ed = step->GetTotalEnergyDeposit() / MeV;
    analysisManager->FillH1(20, 0, ed);
    if(tr->GetParticleDefinition()->GetParticleName() == "opticalphoton" && tr->GetLogicalVolumeAtVertex() == fScoringVolume2){
      analysisManager->FillH2(98, z+1, abs(6+y));
    }
  }
  /*
if(postVolume != nullptr && tr->GetParticleDefinition()->GetParticleName() == "opticalphoton"){
//if((volume == fScoringVolume2 && postVolume->GetName().find("Teflon") != std::string::npos) || (volume->GetName().find("Teflon") != std::string::npos && postVolume == fScoringVolume2)){
  if((volume->GetName().find("Teflon") != std::string::npos && postVolume == fScoringVolume2)){
            G4ThreeVector momDir = step->GetTrack()->GetMomentumDirection();  // 단위벡터
            G4ThreeVector yAxis(0, 1, 0);  // Y축
            // 각도 계산 (라디안 → 도)
            double angleRad = momDir.angle(yAxis);
            double angleDeg = angleRad / CLHEP::deg;
            G4cout << momDir << " | angle = " << angleRad << " rad, " << angleDeg << " deg" << G4endl;
}
  }
*/
  // Check scoring volume 2
  if (volume == fScoringVolume2) {
    G4String name = postVolume->GetName();
    /*
    if ((name.find("Teflon") != std::string::npos && tr->GetParticleDefinition()->GetParticleName() == "opticalphoton") || (name == "World" && tr->GetParticleDefinition()->GetParticleName() == "opticalphoton")) {
            G4ThreeVector momDir = step->GetTrack()->GetMomentumDirection();  // 단위벡터
            G4ThreeVector yAxis(0, 1, 0);  // Y축
            // 각도 계산 (라디안 → 도)
            double angleRad = momDir.angle(yAxis);
            double angleDeg = angleRad / CLHEP::deg;
            const char* jobId = std::getenv("B1_JOB_ID");
            if (jobId) {
              std::string fileName = "Little_" + std::string(jobId) + "_" +std::to_string(tr->GetParticleDefinition()->GetPDGEncoding())+".txt";
              std::ofstream outFile(fileName, std::ios::app);
                      outFile << step->GetPreStepPoint()->GetKineticEnergy()/eV << "  eV,  "<< angleDeg << " deg" << G4endl;
            }
        auto* info = dynamic_cast<B1MyTrackInformation*>(tr->GetUserInformation());
        if (info) {
            info->AddReflection();
        }
    }
        */
    B1EventAction* eventAction = (B1EventAction*)G4EventManager::GetEventManager()->GetUserEventAction();
    if (tr->GetParticleDefinition()->GetParticleName() == "opticalphoton" &&
    step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "SiPM") {
      if (G4UniformRand() < 0.03) return;
analysisManager->FillH1(30,(CLHEP::h_Planck * CLHEP::c_light / step->GetPreStepPoint()->GetKineticEnergy()) / CLHEP::nanometer);
    G4double time = step->GetPostStepPoint()->GetGlobalTime() / ns;
    G4double energy = step->GetPreStepPoint()->GetKineticEnergy() / eV;

if (energy < photonEnergy2[42] && energy > 3.6) {

  auto it = std::min_element(std::begin(photonEnergy2), std::end(photonEnergy2),
  [energy](G4double a, G4double b) {
      return std::abs(a - energy) < std::abs(b - energy);
  });

int idx = std::distance(std::begin(photonEnergy2), it);
G4double efficiency = sipmEfficiency2[idx];

  // 해당 인덱스의 efficiency
  double eff = sipmEfficiency2[idx];

  // 난수 발생 후 efficiency보다 작으면 기록
  if (G4UniformRand() < eff/100) {
    analysisManager->FillH1(33, time);
    //eventAction->CountDetectedPhoton2();
    auto* info = dynamic_cast<B1MyTrackInformation*>(tr->GetUserInformation());
    if(info->GetTag()==1) eventAction->CountDetectedPhotoncompt();
    if(info->GetTag()==2) eventAction->CountDetectedPhotonphot();
    //if(info->GetTag()==5) eventAction->CountDetectedPhotoncompt();
    //if(info->GetTag()==6) eventAction->CountDetectedPhotonphot();
    if(info->GetTag()==3) eventAction->CountDetectedPhotonconv();
    if(info->GetTag()==0) eventAction->CountDetectedPhotonleft();

    eventAction->CountDetectedPhoton();
    //G4cout << "finally?" << G4endl;
    analysisManager->FillH1(32,(CLHEP::h_Planck * CLHEP::c_light / step->GetPreStepPoint()->GetKineticEnergy()) / CLHEP::nanometer);
        if (info) {
            G4int reflections = info->GetReflectionCount();
            analysisManager->FillH1(37, reflections);
            //G4cout << "reflect2   " << reflections << G4endl;
        }
      }
}

if (energy < photonEnergy[0] || energy > photonEnergy[76]) {
  step->GetTrack()->SetTrackStatus(fStopAndKill);
  return;
}
    auto it = std::min_element(std::begin(photonEnergy), std::end(photonEnergy),
    [energy](G4double a, G4double b) {
        return std::abs(a - energy) < std::abs(b - energy);
    });

int idx = std::distance(std::begin(photonEnergy), it);
G4double efficiency = sipmEfficiency[idx];

    // 해당 인덱스의 efficiency
    double eff = sipmEfficiency[idx];

    // 난수 발생 후 efficiency보다 작으면 기록
    if (G4UniformRand() < eff) {
        analysisManager->FillH1(21, time);
        ///////////////eventAction->CountDetectedPhoton();
        //G4cout << "finally?" << G4endl;
        analysisManager->FillH1(29,(CLHEP::h_Planck * CLHEP::c_light / step->GetPreStepPoint()->GetKineticEnergy()) / CLHEP::nanometer);
        auto* info = dynamic_cast<B1MyTrackInformation*>(tr->GetUserInformation());
        if (info) {
            G4int reflections = info->GetReflectionCount();
            analysisManager->FillH1(36, reflections);
            //G4cout << "reflect1   " << reflections << G4endl;
        }
    }

    // 감지 여부와 관계없이 Photon 종료
    //G4cout << "killed" << G4endl;
    tr->SetTrackStatus(fStopAndKill);
    return;
}
if (tr->GetParticleDefinition()->GetParticleName() == "opticalphoton" &&
    step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "SiPM2") {
      if (G4UniformRand() < 0.03) return;
analysisManager->FillH1(40,(CLHEP::h_Planck * CLHEP::c_light / step->GetPreStepPoint()->GetKineticEnergy()) / CLHEP::nanometer);
    G4double time = step->GetPostStepPoint()->GetGlobalTime() / ns;
    G4double energy = step->GetPreStepPoint()->GetKineticEnergy() / eV;

if (energy < photonEnergy2[42] && energy > 3.6) {

  auto it = std::min_element(std::begin(photonEnergy2), std::end(photonEnergy2),
  [energy](G4double a, G4double b) {
      return std::abs(a - energy) < std::abs(b - energy);
  });

int idx = std::distance(std::begin(photonEnergy2), it);
G4double efficiency = sipmEfficiency2[idx];

  // 해당 인덱스의 efficiency
  double eff = sipmEfficiency2[idx];

  // 난수 발생 후 efficiency보다 작으면 기록
  if (G4UniformRand() < eff/100) {
    analysisManager->FillH1(42, time);
    auto* info = dynamic_cast<B1MyTrackInformation*>(tr->GetUserInformation());
    if(info->GetTag()==1) eventAction->CountDetectedPhotoncompt();
    if(info->GetTag()==2) eventAction->CountDetectedPhotonphot();
    //if(info->GetTag()==5) eventAction->CountDetectedPhotoncompt();
    //if(info->GetTag()==6) eventAction->CountDetectedPhotonphot();
    if(info->GetTag()==3) eventAction->CountDetectedPhotonconv();
    if(info->GetTag()==0) eventAction->CountDetectedPhotonleft();
    //eventAction->CountDetectedPhoton();
    eventAction->CountDetectedPhoton2();
    //G4cout << "finally?" << G4endl;
    analysisManager->FillH1(41,(CLHEP::h_Planck * CLHEP::c_light / step->GetPreStepPoint()->GetKineticEnergy()) / CLHEP::nanometer);
        if (info) {
            G4int reflections = info->GetReflectionCount();
            analysisManager->FillH1(37, reflections);
            //G4cout << "reflect2   " << reflections << G4endl;
        }
      }
}

if (energy < photonEnergy[0] || energy > photonEnergy[76]) {
  step->GetTrack()->SetTrackStatus(fStopAndKill);
  return;
}
    auto it = std::min_element(std::begin(photonEnergy), std::end(photonEnergy),
    [energy](G4double a, G4double b) {
        return std::abs(a - energy) < std::abs(b - energy);
    });

int idx = std::distance(std::begin(photonEnergy), it);
G4double efficiency = sipmEfficiency[idx];

    // 해당 인덱스의 efficiency
    double eff = sipmEfficiency[idx];

    // 난수 발생 후 efficiency보다 작으면 기록
    if (G4UniformRand() < eff) {
        analysisManager->FillH1(38, time);
        //////////eventAction->CountDetectedPhoton();
        //G4cout << "finally?" << G4endl;
        analysisManager->FillH1(39,(CLHEP::h_Planck * CLHEP::c_light / step->GetPreStepPoint()->GetKineticEnergy()) / CLHEP::nanometer);
        auto* info = dynamic_cast<B1MyTrackInformation*>(tr->GetUserInformation());
        if (info) {
            G4int reflections = info->GetReflectionCount();
            analysisManager->FillH1(36, reflections);
            //G4cout << "reflect1   " << reflections << G4endl;
        }
    }

    // 감지 여부와 관계없이 Photon 종료
    //G4cout << "killed" << G4endl;
    tr->SetTrackStatus(fStopAndKill);
    return;
}
  G4Track* track = step->GetTrack();

    if (tr->GetParticleDefinition()->GetParticleName() == "opticalphoton" && track->GetTrackStatus() == fStopAndKill) analysisManager->FillH1(35,(CLHEP::h_Planck * CLHEP::c_light / step->GetPreStepPoint()->GetKineticEnergy()) / CLHEP::nanometer);
    // Optical photon 생성 카운트
    /*
    if (tr->GetParticleDefinition()->GetParticleName() == "opticalphoton" &&
        tr->GetCurrentStepNumber() == 0) {
            G4ThreeVector momDir = step->GetTrack()->GetMomentumDirection();  // 단위벡터
            G4ThreeVector yAxis(0, 1, 0);  // Y축
            // 각도 계산 (라디안 → 도)
            double angleRad = momDir.angle(yAxis);
            double angleDeg = angleRad / CLHEP::deg;
            G4cout << momDir << " | angle = " << angleRad << " rad, " << angleDeg << " deg" << G4endl;
      G4double time = step->GetPostStepPoint()->GetGlobalTime() / ns;
      G4double energy = step->GetPostStepPoint()->GetKineticEnergy() / MeV;
      analysisManager->FillH1(22, time);
        eventAction->CountCreatedPhoton();
        analysisManager->FillH1(28,(CLHEP::h_Planck * CLHEP::c_light / step->GetPreStepPoint()->GetKineticEnergy()) / CLHEP::nanometer);
    }
        */
    auto x = preStepPoint->GetPosition()[0] / cm;
    auto y = preStepPoint->GetPosition()[1] / cm;
    auto z = preStepPoint->GetPosition()[2] / cm;
/*
    if (tr->GetParticleDefinition()->GetParticleName() != "opticalphoton"){
  const char* jobId = std::getenv("B1_JOB_ID");
  if (jobId) {
    std::string fileName = "Little_" + std::string(jobId) + "_" +std::to_string(tr->GetParticleDefinition()->GetPDGEncoding())+".txt";
    std::ofstream outFile(fileName, std::ios::app);
    //G4cout << "hmm.." << G4endl;
    outFile << tr->GetTrackID() << "     " << x << "     " << y << "     " << z << std::endl;
  }
}
  */
    auto r = sqrt(x * x + y * y);
    auto theta = preStepPoint->GetMomentumDirection().theta();
    auto e = preStepPoint->GetKineticEnergy() / MeV;
    //G4cout << tr->GetPosition()[2] / cm << "=====" << z <<"     "<< tr->GetKineticEnergy() / MeV << "     " << e << G4endl;
    auto ed = step->GetTotalEnergyDeposit() / MeV;
    /*
    if (tr->GetDefinition()->GetParticleName() == "e-" && step->GetSecondaryInCurrentStep()->size() > 0 && parentInfo->GetTag() == 5) {
      auto secondaries = step->GetSecondaryInCurrentStep();
      for (auto* sec : *secondaries) {
        if(sec->GetDefinition()->GetParticleName() == "gamma") outFile << "huh??  " << sec->GetKineticEnergy() << G4endl; 
        if(sec->GetDefinition()->GetParticleName() == "gamma") G4cout << "huh??  " << sec->GetKineticEnergy() << G4endl; 
      }
    }
      */
    if (tr->GetDefinition()->GetParticleName() == "gamma" && step->GetSecondaryInCurrentStep()->size() > 0 && parentInfo->GetTag() == 0) {
    G4String processName = "Unknown";
    if (step->GetPostStepPoint()->GetProcessDefinedStep()) {
        processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    }
/*
    // 감마 에너지 추출
    G4double gammaE = step->GetPreStepPoint()->GetKineticEnergy() / keV;
//G4cout << "why?" << gammaE << G4endl;
    // 감시 대상 에너지 리스트 (±0.5 keV 허용)
    std::vector<G4double> target_energies = {
        569.697, 1063.650, 75.250, 73.039, 10.551, 12.656
    };

    for (const auto& targetE : target_energies) {
        if (std::abs(gammaE - targetE) < 0.5) {
            outFile << "Matched gamma energy: " << gammaE << " keV"
                   << " | Process: " << processName << G4endl;
            break;  // 중복 출력 방지
        }
    }
        */
        auto* info = new B1MyTrackInformation();

        // process 이름에 따라 tag 부여
        if (processName == "compt") {
            info->SetTag(1);  // 예: 컴프턴 산란
        }
        else if (processName == "phot") {
            info->SetTag(2);  // 광전효과
        }
        else if (processName == "conv") {
            info->SetTag(3);  // 쌍생성
        }
        else {
            info->SetTag(0);  // 기타
        }
      gTrackIDtoTag[tr->GetTrackID()] = info->GetTag();
      //G4cout << gTrackIDtoTag[tr->GetTrackID()] << "    " << tr->GetTrackID() << G4endl;
      const char* jobId = std::getenv("B1_JOB_ID");
      if (jobId) {
        //outFile << tr->GetTrackID() << "     " << tr->GetDefinition()->GetParticleName() << "     " << e << "     " << step->GetPostStepPoint()->GetKineticEnergy() << "     " << ed << "     "  << processName << std::endl;
      }
    }
    if(ed > 0) {
      //outFile <<"hmmm...   " << ed << G4endl; G4cout <<"hmmm...   " << ed << G4endl;
      eventAction->TotalED(ed);
      /*
      if(parentInfo->GetTag()==5) {
        eventAction->TotalED2(ed); 
        //outFile <<"hmmm...   " << ed << G4endl; G4cout <<"hmmm...   " << ed << G4endl;
      }
      if(parentInfo->GetTag()==6) eventAction->TotalED3(ed);
      */
    analysisManager->FillH1(20, 1, ed);
}
    if (tr->GetParticleDefinition()->GetParticleName() == "opticalphoton" &&
        tr->GetCurrentStepNumber() == 1) analysisManager->FillH2(99, z+1, abs(6+y));
    //if(ed > 1) G4cout << tr->GetParticleDefinition()->GetPDGEncoding() <<"   "<< z <<"   "<< y <<"   "<< ed << G4endl;
if (tr->GetParticleDefinition()->GetPDGEncoding() == 11) {
        /*
            auto secondaries = step->GetSecondaryInCurrentStep();
          if (!secondaries || secondaries->empty()) return;
          auto process = step->GetPostStepPoint()->GetProcessDefinedStep();
          if (!process) return;
          G4String procName = process ? process->GetProcessName() : "unknown_process";

          G4double totalOpticalE = 0.0;
          int opticalCount = 0;

          std::vector<std::pair<G4String, G4double>> nonOpticalSecondaries;

          for (auto* sec : *secondaries) {
              G4String secName = sec->GetDefinition()->GetParticleName();
              G4double secE = sec->GetKineticEnergy();

              if (secName == "opticalphoton") {
                  totalOpticalE += secE;
                  opticalCount++;
              } else {
                  nonOpticalSecondaries.emplace_back(secName, secE);
              }
          }

          // 총 opticalphoton 정보 출력
          if (opticalCount > 0) {
            if(step->GetTrack()->GetParentID()==1) outFile << "[HIGH-e-from-gamma] ";
              outFile << procName << " [e- secondaries] TrackLength = " << tr->GetTrackLength() / mm
                    << " mm, Total Optical Energy = " << totalOpticalE / eV
                    << " eV, Optical Count = " << opticalCount << std::endl;
          }

          // 나머지 secondaries 출력
          for (const auto& [name, energy] : nonOpticalSecondaries) {
            if(step->GetTrack()->GetParentID()==1) outFile << "[HIGH-e-from-gamma] ";
              outFile << procName << " [e- secondaries] -> secondary = " << name
                    << ", E = " << energy / MeV << " MeV" << std::endl;
          }
                    */
      analysisManager->FillH2(5, z+1, abs(6+y), ed);
      analysisManager->FillH2(6, abs(x+1), abs(6+y), ed);
      analysisManager->FillH2(7, e, theta);
      analysisManager->FillH2(8, e, r);
    }
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 22) {
        /*
            auto process = step->GetPostStepPoint()->GetProcessDefinedStep();
          if (!process) return;
          G4String procName = process ? process->GetProcessName() : "unknown_process";
              G4double trackLength = tr->GetTrackLength(); // 지금까지의 감마선 트랙 길이
              G4StepPoint* postPoint = step->GetPostStepPoint();

              const auto& secondaries = *step->GetSecondaryInCurrentStep();
              for (auto* sec : secondaries) {
                  if (sec->GetDefinition()->GetParticleName() == "e-") {
                      G4double e_energy = sec->GetKineticEnergy(); // 생성된 전자의 에너지
                      // 에너지가 0.1 MeV 이상이면 특별 표시
                //if (e_energy > 0.1 * MeV) {
                  if (tr->GetTrackID()==1) {
                    outFile << "[HIGH-e-from-gamma] ";
                }
                      // 로그 출력 (또는 파일 기록 가능)
                      outFile << procName << "  TrackLength = " << trackLength / mm
                            << " mm, e- energy = " << e_energy / MeV << " MeV" << std::endl;
                }
              }
                */
      analysisManager->FillH2(9, z+1, abs(6+y), ed);
      analysisManager->FillH2(10, abs(x+1), abs(6+y), ed);
      analysisManager->FillH2(11, e, theta);
      analysisManager->FillH2(12, e, r);
      analysisManager->FillH1(31, z+0.15, e);
    }
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2212) {
      analysisManager->FillH2(13, z, abs(y));
      analysisManager->FillH2(14, abs(x), abs(y));
      analysisManager->FillH2(15, e, theta);
      analysisManager->FillH2(16, e, r);
    }
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2112) {
      analysisManager->FillH2(17, z, abs(y));
      analysisManager->FillH2(18, abs(x), abs(y));
      analysisManager->FillH2(19, e, theta);
      analysisManager->FillH2(20, e, r);
    }
    if(postVolume != fScoringVolume2){
      //G4int uniqueCode = 100 * tr->GetTrackID() + tr->GetParticleDefinition()->GetPDGEncoding();
      //if (tr->GetParticleDefinition()->GetParticleName() != "opticalphoton") eventAction->WriteEscapes(uniqueCode, e);
      //if(tr->GetParticleDefinition()->GetParticleName() == "opticalphoton") analysisManager->FillH2(93, z+1, abs(6+y), e);
      if (tr->GetParticleDefinition()->GetPDGEncoding() == 11) analysisManager->FillH2(63, z+1, abs(6+y), e);
      //if (tr->GetParticleDefinition()->GetPDGEncoding() == -11) analysisManager->FillH2(64, e, theta);
      /*
      if (parentInfo->GetTag()==5 && tr->GetParticleDefinition()->GetParticleName() != "opticalphoton") {analysisManager->FillH2(64, e, theta); 
        outFile << "escapes? :" << tr->GetParticleDefinition()->GetParticleName() << e << G4endl;
        //G4cout << "escapes? :" << tr->GetParticleDefinition()->GetParticleName() << e << G4endl;
      }
        */
      if (tr->GetParticleDefinition()->GetPDGEncoding() == 22) analysisManager->FillH2(65, z+1, abs(6+y), e);
      if (tr->GetParticleDefinition()->GetPDGEncoding() == 2112) analysisManager->FillH2(66, e, theta);
      if (tr->GetParticleDefinition()->GetPDGEncoding() == 2212) analysisManager->FillH2(67, e, theta);
    }
  }
  /*
  G4String name = volume->GetName();
    if (name.find("Al") != std::string::npos && postVolume == fScoringVolume1 && tr->GetParticleDefinition()->GetParticleName() != "opticalphoton"){
B1EventAction* eventAction = (B1EventAction*)G4EventManager::GetEventManager()->GetUserEventAction();
G4int uniqueCode = 100 * tr->GetTrackID() + tr->GetParticleDefinition()->GetPDGEncoding();
eventAction->WriteEscapes(uniqueCode, step->GetPostStepPoint()->GetKineticEnergy());
    }
*/
  if(volume != fScoringVolume2 && postVolume == fScoringVolume2){
G4int threadID = G4Threading::G4GetThreadId();
G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
G4int trackID = tr->GetTrackID();
auto x = step->GetPostStepPoint()->GetPosition()[0] / cm;
auto y = step->GetPostStepPoint()->GetPosition()[1] / cm;
auto z = step->GetPostStepPoint()->GetPosition()[2] / cm;
auto r = sqrt(x * x + y * y);
auto e = step->GetPostStepPoint()->GetKineticEnergy() / MeV;
auto t = step->GetPostStepPoint()->GetGlobalTime() / ns; 
        if (tr->GetParticleDefinition()->GetPDGEncoding() == 11) analysisManager->FillH2(68, e, t);
        if (tr->GetParticleDefinition()->GetPDGEncoding() == -11) analysisManager->FillH2(69, e, t);
        if (tr->GetParticleDefinition()->GetPDGEncoding() == 22) analysisManager->FillH2(70, e, t);
        if (tr->GetParticleDefinition()->GetPDGEncoding() == 2112) analysisManager->FillH2(71, e, t);
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2212) analysisManager->FillH2(72, e, t);
        if (tr->GetParticleDefinition()->GetPDGEncoding() == 11) analysisManager->FillH2(78, z, y);
        if (tr->GetParticleDefinition()->GetPDGEncoding() == -11) analysisManager->FillH2(79, z, y);
        if (tr->GetParticleDefinition()->GetPDGEncoding() == 22) analysisManager->FillH2(80, z, y);
        if (tr->GetParticleDefinition()->GetPDGEncoding() == 2112) analysisManager->FillH2(81, z, y);
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2212) analysisManager->FillH2(82, z, y);
        if (tr->GetParticleDefinition()->GetPDGEncoding() == 11) analysisManager->FillH1(23, e);
        //if (tr->GetParticleDefinition()->GetPDGEncoding() == -11) analysisManager->FillH1(24, t, e);
        /*
        if (parentInfo->GetTag()==5 && tr->GetParticleDefinition()->GetParticleName() != "opticalphoton") {analysisManager->FillH1(24, e);
          outFile << "Enters :" << tr->GetParticleDefinition()->GetParticleName() << e << G4endl;
        //G4cout << "Enters :" << tr->GetParticleDefinition()->GetParticleName() << e << G4endl;
        }
        */
        if (tr->GetParticleDefinition()->GetPDGEncoding() == 22) analysisManager->FillH1(25, e);
        if (tr->GetParticleDefinition()->GetPDGEncoding() == 2112) analysisManager->FillH1(26, t, e);
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2212) analysisManager->FillH1(27, t, e);
}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

