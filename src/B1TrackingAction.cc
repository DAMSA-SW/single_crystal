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
// $Id: B1SteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1TrackingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1Analysis.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4INCLGlobals.hh"

#include "B1MyTrackInformation.hh"
#include "G4Track.hh"
#include "B1SteppingAction.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1TrackingAction::B1TrackingAction(B1EventAction* eventAction)
: G4UserTrackingAction(),
  fEventAction(eventAction),
  fScoringVolume1(0),
  fScoringVolume2(0),
  fScoringVolume3(0),
  fScoringVolume4(0),
  fScoringVolume5(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1TrackingAction::~B1TrackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1TrackingAction::PreUserTrackingAction(const G4Track* tr)
{
  auto* info = new B1MyTrackInformation();

  // trackID 자체로부터 tag 가져오기
  if (gTrackIDtoTag.count(tr->GetParentID()) > 0){
    if(gTrackIDtoTag[tr->GetParentID()] == 4){
      if(tr->GetDefinition()->GetParticleName() == "e-")  info->SetTag(5);
      else if(tr->GetDefinition()->GetParticleName() == "gamma")  info->SetTag(6);
      else info->SetTag(0);
    }
    else info->SetTag(gTrackIDtoTag[tr->GetParentID()]);
  }

  const_cast<G4Track*>(tr)->SetUserInformation(info);

 /*
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
    = tr->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

  auto analysisManager = G4RootAnalysisManager::Instance();

  if (volume == fScoringVolume1) {
    auto z = tr->GetPosition()[2]/cm;
    auto y = tr->GetPosition()[1]/cm;
    auto x = tr->GetPosition()[0]/cm;
    analysisManager->FillH2(0, z, y);
    analysisManager->FillH2(3, z, x);
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 11) analysisManager->FillH2(1, z, y);
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 22) analysisManager->FillH2(2, z, y);
    //if (tr->GetParticleDefinition()->GetPDGEncoding() == 2212) analysisManager->FillH2(3, z, y);
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2112) analysisManager->FillH2(4, z, y);
  }
  if (volume == fScoringVolume2) {
    auto x = tr->GetPosition()[0]/cm;
    auto y = tr->GetPosition()[1]/cm;
    auto z = tr->GetPosition()[2]/cm;
    auto r = sqrt(x*x + y*y);
    auto theta = tr->GetMomentumDirection().theta();
    auto e = tr->GetKineticEnergy()/MeV;

    if (tr->GetParticleDefinition()->GetPDGEncoding() == 11) {
      analysisManager->FillH2(5, z, abs(x));
      analysisManager->FillH2(6, abs(x), abs(y));
      analysisManager->FillH2(7, e, theta);
      analysisManager->FillH2(8, e ,r);
    }
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 22) {
      analysisManager->FillH2(9, z, abs(x));
      analysisManager->FillH2(10, abs(x), abs(y));
      analysisManager->FillH2(11, e, theta);
      analysisManager->FillH2(12, e ,r);
    }
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2212) {
      analysisManager->FillH2(13, z, abs(x));
      analysisManager->FillH2(14, abs(x), abs(y));
      analysisManager->FillH2(15, e, theta);
      analysisManager->FillH2(16, e ,r);
    }
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2112) {
      analysisManager->FillH2(17, z, abs(x));
      analysisManager->FillH2(18, abs(x), abs(y));
      analysisManager->FillH2(19, e, theta);
      analysisManager->FillH2(20, e ,r);
    }
  }
  if (volume == fScoringVolume3) {
    auto x = tr->GetPosition()[0]/cm;
    auto y = tr->GetPosition()[1]/cm;
    auto z = tr->GetPosition()[2]/cm;
    auto r = sqrt(x*x + y*y);
    auto theta = tr->GetMomentumDirection().theta();
    auto e = tr->GetKineticEnergy()/MeV;

    if (tr->GetParticleDefinition()->GetPDGEncoding() == 11) {
      analysisManager->FillH2(21, z, abs(x));
      analysisManager->FillH2(22, abs(x), abs(y));
      analysisManager->FillH2(23, e, theta);
      analysisManager->FillH2(24, e ,r);
      analysisManager->FillH1(12, e);
    }
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 22) {
      analysisManager->FillH2(25, z, abs(x));
      analysisManager->FillH2(26, abs(x), abs(y));
      analysisManager->FillH2(27, e, theta);
      analysisManager->FillH2(28, e ,r);
      analysisManager->FillH1(16, e);
    }
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2212) {
      analysisManager->FillH2(29, z, abs(x));
      analysisManager->FillH2(30, abs(x), abs(y));
      analysisManager->FillH2(31, e, theta);
      analysisManager->FillH2(32, e ,r);
      analysisManager->FillH1(13, e);
    }
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2112) {
      analysisManager->FillH2(33, z, abs(x));
      analysisManager->FillH2(34, abs(x), abs(y));
      analysisManager->FillH2(35, e, theta);
      analysisManager->FillH2(36, e ,r);
      analysisManager->FillH1(17, e);
    }
  }
  if (volume == fScoringVolume4) {
    auto x = tr->GetPosition()[0]/cm;
    auto y = tr->GetPosition()[1]/cm;
    auto z = tr->GetPosition()[2]/cm;
    auto r = sqrt(x*x + y*y);
    auto theta = tr->GetMomentumDirection().theta();
    auto e = tr->GetKineticEnergy()/MeV;

    if (tr->GetParticleDefinition()->GetPDGEncoding() == 11) {
      analysisManager->FillH2(42, z, abs(x));
      analysisManager->FillH2(43, abs(x), abs(y));
      analysisManager->FillH2(44, e, theta);
      analysisManager->FillH2(45, e ,r);
      analysisManager->FillH1(14, e);
    }
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 22) {
      analysisManager->FillH2(46, z, abs(x));
      analysisManager->FillH2(47, abs(x), abs(y));
      analysisManager->FillH2(48, e, theta);
      analysisManager->FillH2(49, e ,r);
      analysisManager->FillH1(18, e);
    }
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2212) {
      analysisManager->FillH2(50, z, abs(x));
      analysisManager->FillH2(51, abs(x), abs(y));
      analysisManager->FillH2(52, e, theta);
      analysisManager->FillH2(53, e ,r);
      analysisManager->FillH1(15, e);
    }
    if (tr->GetParticleDefinition()->GetPDGEncoding() == 2112) {
      analysisManager->FillH2(54, z, abs(x));
      analysisManager->FillH2(55, abs(x), abs(y));
      analysisManager->FillH2(56, e, theta);
      analysisManager->FillH2(57, e ,r);
      analysisManager->FillH1(19, e);
    }
  }
    */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

