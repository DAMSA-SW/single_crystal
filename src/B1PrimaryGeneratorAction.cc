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
// $Id: B1PrimaryGeneratorAction.cc 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file B1PrimaryGeneratorAction.cc
/// \brief Implementation of the B1PrimaryGeneratorAction class

#include "B1PrimaryGeneratorAction.hh"
#include "B1EventAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4IonTable.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), 
  fEnvelopeBox(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    G4ParticleDefinition* Am241 = G4IonTable::GetIonTable()->GetIon(95, 241, 0.0); // Z=83, A=207
    fParticleGun->SetParticleDefinition(Am241);
    //G4ParticleDefinition* bi207 = G4IonTable::GetIonTable()->GetIon(83, 207, 0.0); // Z=83, A=207
    //fParticleGun->SetParticleDefinition(bi207);
    //G4ParticleDefinition* Co60 = G4IonTable::GetIonTable()->GetIon(27, 60, 0.0); // Z=83, A=207
    //fParticleGun->SetParticleDefinition(Co60);
    fParticleGun->SetParticleCharge(0.);
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -0.6*cm));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleEnergy(0.0);  // rest
    fParticleGun->GeneratePrimaryVertex(anEvent);
    /*
    // 이벤트 액션 가져와서 theta/phi 전달
auto eventAction = const_cast<B1EventAction*>(
    static_cast<const B1EventAction*>(
        G4RunManager::GetRunManager()->GetUserEventAction()
    )
);
    // 입자 생성기 초기화
    //G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    //fParticleGun->SetParticleDefinition(particle);
    G4double theta_min = 0.0;
    G4double theta_max = 30.0 * deg;
    //G4double theta_max = 10.0 * deg;
    
    // 1. Uniform in cos(theta)
    // 세타, 파이 계산
    G4double costheta = std::cos(0.0) - G4UniformRand() * (std::cos(theta_min) - std::cos(theta_max));
    G4double sintheta = std::sqrt(1.0 - costheta * costheta);
    G4double theta = std::acos(costheta);
    G4double phi = 2.0 * CLHEP::pi * G4UniformRand();
    //G4double E = 100*G4UniformRand();
    G4double E = 1;
    G4double dirX = sintheta * std::cos(phi);
    G4double dirY = sintheta * std::sin(phi);
    G4double dirZ = costheta;

    G4double radius = 0.45 * cm;
    G4double y_range = 5.95 * cm;
    G4double x = (2.0 * G4UniformRand() - 1.0) * radius;  // x in [-0.1 cm, 0.1 cm]
    G4double y = (2.0 * G4UniformRand() - 1.0) * y_range; // y in [-2.95 cm, 2.95 cm]
    G4ThreeVector position(x, y, -0.6 * cm);
    //G4ThreeVector position(0.6 * cm, 0, -0.49999 * cm);
    fParticleGun->SetParticlePosition(position);
    // 4. Set momentum direction /////////////////////////////////////////////////////////important
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(dirX, dirY, dirZ));
    //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
    //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(-1, 0, 0));
  
    // ---------- 에너지 설정 (Co-60: 대표적 감마선 1.17 MeV & 1.33 MeV 중 택일) ----------
    G4double energies[2] = {1.17 * MeV, 1.33 * MeV};
    G4int index = G4RandFlat::shootInt(2);
    fParticleGun->SetParticleEnergy(energies[index]);
    eventAction->SetThetaPhi(theta, phi, x, y, energies[index]/MeV);
    //fParticleGun->SetParticleEnergy(1.33*MeV);
    //fParticleGun->SetParticleEnergy(E*MeV);
    // 이벤트에 입자 쏘기
    fParticleGun->GeneratePrimaryVertex(anEvent);
    */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

