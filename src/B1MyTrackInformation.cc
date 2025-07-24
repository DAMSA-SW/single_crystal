#include "B1MyTrackInformation.hh"
#include "G4SystemOfUnits.hh"

B1MyTrackInformation::B1MyTrackInformation(int pdg)
  : G4VUserTrackInformation(), totalReflections(0), tag(pdg) {}

B1MyTrackInformation::~B1MyTrackInformation() {}

void B1MyTrackInformation::AddReflection() {
    totalReflections++;
}

G4int B1MyTrackInformation::GetReflectionCount() const {
    return totalReflections;
}
