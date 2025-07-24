#pragma once
#include "G4VUserTrackInformation.hh"

class B1MyTrackInformation : public G4VUserTrackInformation {
public:
    B1MyTrackInformation(int pdg = 0);
    virtual ~B1MyTrackInformation();

    void SetTag(int pdg) { tag = pdg; }
    int GetTag() const { return tag; }
    void AddReflection();
    G4int GetReflectionCount() const;

private:
    G4int totalReflections = 0;
    int tag;
};
