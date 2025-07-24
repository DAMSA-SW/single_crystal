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
// $Id: B1EventAction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1Run.hh"
#include "B1Analysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4Step.hh"

#include "B1SteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction()
: G4UserEventAction(),
  fEdep(0.),
  nPTr1(0.),
  nETr1(0.),
  nPhTr1(0.),
  nPrTr1(0.),
  nNeTr1(0.),
  nPTr2(0.),
  nETr2(0.),
  nPhTr2(0.),
  nPrTr2(0.),
  nNeTr2(0.),
  thetaDeg(-1),
  phiDeg(-1),
  xpoint(-1121),
  ypoint(-1121),
  Energy(-1)
{
  const char* jobId = std::getenv("B1_JOB_ID");
  if (jobId) {
    std::string fileName = "LittleDet1_" + std::string(jobId) + std::to_string(G4Threading::G4GetThreadId()) + "_photon_counts.txt";
    outFile.open(fileName, std::ios::out);
  }
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::ofstream& B1EventAction::GetOutputFile() {
  return outFile;
}

B1EventAction::~B1EventAction()
{
  outFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
  nPTr1 = 0;
  nETr1 = 0;
  nPhTr1 = 0;
  nPrTr1 = 0;
  nNeTr1 = 0;
  nPTr2 = 0;
  nETr2 = 0;
  nPhTr2 = 0;
  nPrTr2 = 0;
  nNeTr2 = 0;
  
    createdPhotons = 0;
    detectedPhotons = 0;
    detectedPhotons2 = 0;
    detectedPhotonscompt = 0;
    detectedPhotonsphot = 0;
    detectedPhotonsconv = 0;
    detectedPhotonsleft = 0;
    TED = 0;
    TED2 = 0;
    TED3 = 0;
    escapeParticles.clear();
    gTrackIDtoTag.clear();
}

void B1EventAction::EndOfEventAction(const G4Event* event)
{
  // accumulate statistics in B1Run
  B1Run* run 
    = static_cast<B1Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->AddEdep(fEdep);
  
  auto analysisManager = G4RootAnalysisManager::Instance();
  analysisManager->FillH1(0, nPTr1);
  analysisManager->FillH1(1, nETr1);
  analysisManager->FillH1(2, nPhTr1);
  analysisManager->FillH1(3, nPrTr1);
  analysisManager->FillH1(4, nNeTr1);
  analysisManager->FillH1(5, nPTr2);
  analysisManager->FillH1(6, nETr2);
  analysisManager->FillH1(7, nPhTr2);
  analysisManager->FillH1(8, nPrTr2);
  analysisManager->FillH1(9, nNeTr2);
  analysisManager->FillH2(98, 1, 11);
//if(createdPhotons == 0) return;
     outFile << "Energy: " << Energy
             << " Theta(deg): " << thetaDeg
             << " Phi(deg): " << phiDeg
             << " Xpoint: " << xpoint
             << " Ypoint: " << ypoint
             << " EventID: " << event->GetEventID()
             << " Created: " << createdPhotons
             << " Detected: " << detectedPhotons << "  ,   " << detectedPhotons2
             << "  compt:   " << detectedPhotonscompt << "  phot:   " << detectedPhotonsphot << "  conv:   " << detectedPhotonsconv << "  ,   " << detectedPhotonsleft
             << " TotalED: " << TED
             << " TotalED_ConvE: " << TED2
             << " TotalED_Gamma: " << TED3
             << std::endl;
             /*
             //double lastEscapeEnergySum = 0.0;
             for (const auto& pair : escapeParticles) {
              int uniqueCode = pair.first;
              const auto& energies = pair.second;
          
              // 마지막 볼륨이 "World"일 때만 기록
              if (lastVolumeOfTrack.count(uniqueCode) &&
                  lastVolumeOfTrack[uniqueCode] == "World") {
        
                  if (!energies.empty()) {
                      lastEscapeEnergySum += energies.back();
                  }

                  outFile << " Escape PDG: " << uniqueCode << " |";
                  for (double e : energies) {
                      outFile << " " << e;
                  }
                  outFile << std::endl;
              }
          }
        // 추가 로그 출력
        //if (lastEscapeEnergySum > Energy) DumpStepLogs(outFile);
    
        // 다음 이벤트 준비
        //ClearStepLogs();
        */
}
void B1EventAction::WriteEscapes(int pdg, double energy) {
  escapeParticles[pdg].push_back(energy);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void B1EventAction::LogStep(G4Track* track, const G4Step* step) {
  StepLogEntry entry;
  entry.trackID = track->GetTrackID();
  entry.parentID = track->GetParentID();
  entry.pdg = track->GetParticleDefinition()->GetPDGEncoding();
  entry.volumeName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
  entry.kineticEnergy = step->GetPreStepPoint()->GetKineticEnergy();

  stepLogs.push_back(entry);
}

void B1EventAction::ClearStepLogs() {
  stepLogs.clear();
}

void B1EventAction::DumpStepLogs(std::ofstream& outFile) const {
  outFile << " StepLog:" << std::endl;
  for (const auto& entry : stepLogs) {
      outFile << "  TrackID: " << entry.trackID
              << " ParentID: " << entry.parentID
              << " PDG: " << entry.pdg
              << " Volume: " << entry.volumeName
              << " KE: " << entry.kineticEnergy << std::endl;
  }
}