
#include "B1PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//#include "HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonINCLXXPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
//#include "GammaNuclearPhysics.hh"

//#include "ElectromagneticPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
//#include "StepMaxBuilder.hh"
#include "G4StepLimiterPhysics.hh"

B1PhysicsList::B1PhysicsList()
:G4VModularPhysicsList()
{
  G4int verb = 1;
  SetVerboseLevel(verb);
  new G4UnitDefinition( "millielectronVolt", "meV", "Energy", 1.e-3*eV);   
  new G4UnitDefinition( "mm2/g",  "mm2/g", "Surface/Mass", mm2/g);
  new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);

  //RegisterPhysics( new HadronElasticPhysicsHP(verb) );

  RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP(verb));

  RegisterPhysics( new G4IonElasticPhysics(verb));

  RegisterPhysics( new G4IonPhysicsXS(verb));

  //RegisterPhysics( new GammaNuclearPhysics("gamma"));

  //RegisterPhysics(new ElectromagneticPhysics());

  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4EmStandardPhysics());
  RegisterPhysics(new G4EmExtraPhysics());

  RegisterPhysics(new G4RadioactiveDecayPhysics());

  //RegisterPhysics(new StepMaxBuilder());
  RegisterPhysics( new G4StepLimiterPhysics());

}

B1PhysicsList::~B1PhysicsList()
{ }

void B1PhysicsList::SetCuts()
{
  SetCutValue(0*mm, "*");
}

