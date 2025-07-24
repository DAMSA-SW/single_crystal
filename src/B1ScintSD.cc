#include "B1ScintSD.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4Step.hh"

B1ScintSD::B1ScintSD(const G4String& name)
: G4VSensitiveDetector(name)
{
    outFile.open("optical_hits.txt", std::ios::app);
}

B1ScintSD::~B1ScintSD()
{
    if(outFile.is_open()) outFile.close();
}

void B1ScintSD::Initialize(G4HCofThisEvent*)
{
}

G4bool B1ScintSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    // Debugging output to confirm hit processing
    G4cout << "ProcessHits called for particle: " << step->GetTrack()->GetDefinition()->GetParticleName() << G4endl;

    // Collect necessary information
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int parentID = step->GetTrack()->GetParentID();
    G4double time = step->GetPostStepPoint()->GetGlobalTime() / ns;
    G4double energy = step->GetTrack()->GetKineticEnergy() / eV;

    // Write to file
    outFile << eventID << " "
            << parentID << " "
            << time << " "
            << energy << G4endl;

    // Indicate that the hit was processed
    G4cout << "Hit processed: Event ID = " << eventID << ", Parent ID = " << parentID 
           << ", Time = " << time << " ns, Energy = " << energy << " eV" << G4endl;

    return true;
}

void B1ScintSD::EndOfEvent(G4HCofThisEvent*)
{
    outFile.flush();
} 