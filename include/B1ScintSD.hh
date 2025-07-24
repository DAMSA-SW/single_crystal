#ifndef B1ScintSD_h
#define B1ScintSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include <fstream>

class B1ScintSD : public G4VSensitiveDetector
{
  public:
    B1ScintSD(const G4String& name);
    virtual ~B1ScintSD();

    virtual void Initialize(G4HCofThisEvent*);
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    virtual void EndOfEvent(G4HCofThisEvent*);

  private:
    std::ofstream outFile;
};

#endif