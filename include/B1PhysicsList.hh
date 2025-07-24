#ifndef B1PhysicsList_h
#define B1PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class B1PhysicsList: public G4VModularPhysicsList
{
public:
  B1PhysicsList();
 ~B1PhysicsList();

public:
  virtual void SetCuts();
};
#endif


