// Essential material properties required for scintillation simulation in Geant4

G4NistManager* nist = G4NistManager::Instance();
G4Material* det_mat = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");

auto* mpt = new G4MaterialPropertiesTable();

// Refractive index as a function of photon energy
mpt->AddProperty("RINDEX", photonEnergy, rIndex, nEntries);

// Scintillation emission spectrum (relative photon intensity vs. energy)
mpt->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintSpectrum, nEntries);

// Scintillation decay time constant
mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", decayTime);

// Number of scintillation photons produced per deposited energy
mpt->AddConstProperty("SCINTILLATIONYIELD", scintYield);

// Assign optical properties to the detector material
det_mat->SetMaterialPropertiesTable(mpt);





// Teflon reflector surface for optical photons
auto* teflonSurface = new G4OpticalSurface("TeflonSurface");

teflonSurface->SetType(dielectric_dielectric);  // Boundary between two dielectric materials
teflonSurface->SetModel(unified);               // Geant4 unified optical surface model
teflonSurface->SetFinish(groundfrontpainted);   // Rough painted surface, often used for reflectors

auto* teflonMPT = new G4MaterialPropertiesTable();

G4double photonEnergy[] = {1.0 * eV, 6.0 * eV};
G4double reflectivity[] = {0.95, 0.95};
G4int nEntries = 2;

// Reflection probability of optical photons at the Teflon surface
teflonMPT->AddProperty("REFLECTIVITY", photonEnergy, reflectivity, nEntries);

teflonSurface->SetMaterialPropertiesTable(teflonMPT);

// Attach the optical surface to a boundary between two physical volumes
new G4LogicalBorderSurface(
    "TeflonBorderSurface",
    crystalPhys,        // volume where the photon comes from
    teflonPhys,         // volume the photon is entering
    teflonSurface
);




// Apply SiPM photon detection efficiency (PDE)

if (track->GetParticleDefinition()->GetParticleName() == "opticalphoton" &&
    postVolume->GetName() == "SiPM") {

    G4double time   = step->GetPostStepPoint()->GetGlobalTime() / ns;
    G4double energy = step->GetPreStepPoint()->GetKineticEnergy() / eV;

    // Find the closest PDE value for the photon energy
    auto it = std::min_element(std::begin(photonEnergy), std::end(photonEnergy),
        [energy](G4double a, G4double b) { return std::abs(a - energy) < std::abs(b - energy); });

    G4int idx = std::distance(std::begin(photonEnergy), it);
    G4double pde = sipmEfficiency[idx];

    // Accept or reject photon detection according to PDE
    if (G4UniformRand() < pde) {
        eventAction->CountDetectedPhoton();
        analysisManager->FillH1(21, time);
    }

    // Remove photon after it reaches the SiPM
    track->SetTrackStatus(fStopAndKill);
}