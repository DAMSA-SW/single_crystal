#include "B1DetectorConstruction.hh"
#include "B1ScintSD.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"

#include "OrganicScintillatorFactory.hh"
#include "InorganicScintillatorFactory.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalParameters.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume1(0),
  fScoringVolume2(0),
  fScoringVolume3(0),
  fScoringVolume4(0),
  fScoringVolume5(0)

{ }

B1DetectorConstruction::~B1DetectorConstruction()
{ }

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  G4String name;
  G4double density;
  G4int nel, natoms;

  G4NistManager* nist = G4NistManager::Instance();
  G4Material* det_mat = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
//G4OpticalParameters::Instance()->SetScintByParticleType(true);
// EJ301 (det_mat) 정의
G4double photonEnergy[48] = {
  2.06855400 * eV,
  2.10825500 * eV,
  2.14950900 * eV,
  2.19241000 * eV,
  2.23705900 * eV,
  2.28356300 * eV,
  2.33204300 * eV,
  2.38262500 * eV,
  2.43545000 * eV,
  2.49067100 * eV,
  2.54845400 * eV,
  2.60898200 * eV,
  2.67245500 * eV,
  2.73909300 * eV,
  2.80914000 * eV,
  2.88286300 * eV,
  2.96056000 * eV,
  3.04256200 * eV,
  3.12923500 * eV,
  3.22099100 * eV,
  3.31829000 * eV,
  3.42165200 * eV,
  3.51283600 * eV,
  3.57961100 * eV,
  3.63890100 * eV,
  3.67953200 * eV,
  3.71409000 * eV,
  3.74930300 * eV,
  3.77795800 * eV,
  3.81439900 * eV,
  3.84032900 * eV,
  3.86661300 * eV,
  3.92027700 * eV,
  4.02809300 * eV,
  4.14200800 * eV,
  4.20364900 * eV,
  4.23516200 * eV,
  4.25796300 * eV,
  4.29029900 * eV,
  4.32313100 * eV,
  4.37091400 * eV,
  4.41976500 * eV,
  4.47984800 * eV,
  4.55728800 * eV,
  4.62119500 * eV,
  4.70364300 * eV,
  4.85971100 * eV,
  5.08465800 * eV
};

G4double rIndex[48] = {
  1.7371, 1.7382, 1.7392, 1.7397, 1.7412, 1.7434, 1.7457, 1.7479, 1.7502, 1.7526,
  1.7554, 1.7592, 1.7642, 1.7689, 1.7735, 1.7790, 1.7846, 1.7903, 1.7963, 1.8030,
  1.8111, 1.8201, 1.8301, 1.8377, 1.8442, 1.8483, 1.8518, 1.8552, 1.8580, 1.8615,
  1.8639, 1.8663, 1.8735, 1.8884, 1.9038, 1.9154, 1.9212, 1.9254, 1.9312, 1.9370,
  1.9453, 1.9536, 1.9693, 1.9899, 2.0065, 2.0272, 2.0878, 2.1917
};
G4double absLength[48] = {
  1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m,
  1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m,
  1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m,
  1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m,
  1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m,
  1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m, 1e6 * m
};


G4double scintComp[48] = {
  0.00607500,
  0.01955900,
  0.03124500,
  0.04203300,
  0.05057300,
  0.06046100,
  0.06720300,
  0.07349600,
  0.07619300,
  0.07709200,
  0.07529400,
  0.07349600,
  0.07124900,
  0.06855200,
  0.06540600,
  0.06225900,
  0.06091100,
  0.06225900,
  0.07079900,
  0.08518200,
  0.10630800,
  0.15125500,
  0.22069900,
  0.29913200,
  0.38048700,
  0.46408900,
  0.54184800,
  0.60657200,
  0.67669000,
  0.74231300,
  0.80928500,
  0.87625700,
  0.95446500,
  0.98997400,
  0.95266700,
  0.86816600,
  0.78501400,
  0.69197200,
  0.61106700,
  0.53555500,
  0.46453800,
  0.38453200,
  0.30452600,
  0.21732800,
  0.14855800,
  0.08248600,
  0.03079600,
  0.00517600
};

// 분리된 스펙트럼 정의
  G4int N = 48;
  G4int N1 = 24;
  G4int N2 = 25;
  G4double sum1 = 0.0; // low-energy (0~23)
  G4double sum2 = 0.0; // high-energy (24~47)
  for (int i = 0; i < 23; i++) {
      sum1 += scintComp[i];
      sum2 += scintComp[i + 24];
  }
  sum1 += scintComp[23]/2;
  sum2 += scintComp[23]/2;
  sum2 += scintComp[47];
  G4double total = sum1 + sum2;
  G4double yield1 = sum1 / total;
  G4double yield2 = sum2 / total;
  // 저에너지 영역
  G4double photonEnergy1[N1];
  G4double scintComp1[N1];
  for (int i = 0; i < 23; i++) {
      photonEnergy1[i] = photonEnergy[i];
      scintComp1[i] = scintComp[i];
  }
  photonEnergy1[23] = photonEnergy[23];
  scintComp1[23] = scintComp[23]/2;
  // 고에너지 영역
  G4double photonEnergy2[N2];
  G4double scintComp2[N2];
  for (int i = 0; i < 24; i++) {
      photonEnergy2[i+1] = photonEnergy[i + 24];
      scintComp2[i+1] = scintComp[i + 24];
  }
  photonEnergy2[0] = photonEnergy[23];
  scintComp2[0] = scintComp[23]/2;
// Material Properties Table
G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

mpt->AddProperty("RINDEX", photonEnergy, rIndex, 48);
mpt->AddProperty("ABSLENGTH", photonEnergy, absLength, 48);
// Scintillation: 두 성분 나눠서 설정
mpt->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy1, scintComp1, N1);
mpt->AddProperty("SCINTILLATIONCOMPONENT2", photonEnergy2, scintComp2, N2);

mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10.0 * ns);     // 빠른 성분
mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1000.0 * ns);   // 느린 성분

mpt->AddConstProperty("SCINTILLATIONYIELD", 2000.0 / MeV);
mpt->AddConstProperty("RESOLUTIONSCALE", 1.0);

// 적용
mpt->AddConstProperty("SCINTILLATIONYIELD1", yield1);
mpt->AddConstProperty("SCINTILLATIONYIELD2", yield2);

// Material에 할당
det_mat->SetMaterialPropertiesTable(mpt);

  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  ///*
  const G4int nEntriesair = 2;
  G4double photonEnergyair[nEntriesair] = { 1.5*eV, 6.2*eV };
  G4double airRindex[nEntriesair] = { 1.0003, 1.0003 };
  G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();
  airMPT->AddProperty("RINDEX", photonEnergyair, airRindex, nEntriesair);
  world_mat->SetMaterialPropertiesTable(airMPT);
//*/
  G4bool checkOverlaps = true;

  G4RotationMatrix* Rotation = new G4RotationMatrix();
  Rotation->rotateX(90*deg);
  Rotation->rotateY(0*deg);
  Rotation->rotateZ(0*deg);

  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       3*m, 3*m, 3*m);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

// /*
G4double tarCenter =   12.0*0.5*cm;
//G4double tarCenter =   2*0.5*cm;
G4double dx = 0.5*cm;
G4double dy = tarCenter;  // height → z방향 회전돼서 이제 y축이 됨
G4double dz = 0.5*cm;
G4double sipmSizeXY = 0.6 * cm;
//    */
/*
G4double tarCenter =   12.0*0.5*cm;
//G4double tarCenter =   0.5*cm;
G4double dx = 4*0.5*cm;
G4double dy = tarCenter;  // height → z방향 회전돼서 이제 y축이 됨
G4double dz = 4*0.5*cm;
G4double sipmSizeXY = 4 * cm;
    */
/*
    G4double tarCenter =   6.0*0.5*cm;
    G4double dx = 0.15*cm;
    G4double dy = tarCenter;  // height → z방향 회전돼서 이제 y축이 됨
    G4double dz = 0.15*cm;
*/
        G4Box* solidTarget =    
          new G4Box("Target",                    //its name
              dx, dz, tarCenter); //its size
  G4LogicalVolume* logicTarget =                         
    new G4LogicalVolume(solidTarget,            //its solid
                        det_mat,             //its material
                        "Target");         //its name
  G4VPhysicalVolume* physTarget =
  new G4PVPlacement(Rotation,                       //no rotation
                    G4ThreeVector(0,0,0),         //at (0,0,0)
                    logicTarget,                //its logical volume
                    "Target",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
    // ---- Teflon 덮개 Optical Surface 설정 ----
    G4OpticalSurface* teflonSurface = new G4OpticalSurface("TeflonSurface");
    teflonSurface->SetType(dielectric_dielectric);
    teflonSurface->SetFinish(groundfrontpainted);
    teflonSurface->SetModel(unified);
    G4MaterialPropertiesTable* teflonMPT = new G4MaterialPropertiesTable();
    const G4int num = 2;
    G4double ephoton[num] = {1.0 * eV, 6.0 * eV};
    G4double reflectivity[num] = {0.98, 0.98};
    //G4double reflectivity[num] = {1, 1};
    teflonMPT->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
    teflonSurface->SetMaterialPropertiesTable(teflonMPT);

    G4Material* teflon = nist->FindOrBuildMaterial("G4_TEFLON");
    G4double teflonThickness = 0.1;
 ///*
auto surface = new G4OpticalSurface("CsI_Air_Surface");
surface->SetType(dielectric_dielectric);   // 둘 다 유전체
surface->SetModel(unified);                // 통합 모델 (더 유연함)
surface->SetFinish(polished);              // 매끄러운 계면
auto surfaceProperty = new G4MaterialPropertiesTable();
// CsI 경계면 반사율 설정 (공기 쪽 경계면 손실 표현 시 유용)
surfaceProperty->AddConstProperty("REFLECTIVITY", 0.0, true);   // 0이면 반사는 굴절률 기반 자동 계산
surfaceProperty->AddConstProperty("TRANSMITTANCE", 1.0, true);  // 완전 투과 허용

surface->SetMaterialPropertiesTable(surfaceProperty);
new G4LogicalBorderSurface("Surface",   physTarget, physWorld, surface);
//*/
/*
G4double teflon_thickness2 = 0.2 * mm;   // 커버 두께
G4double teflon_inner     = 1.0 * mm;   // 안쪽 길이
G4double teflon_outer     = 1.2 * mm;   // 바깥쪽 길이

G4Box* solidLegZ = new G4Box("LegZ", teflon_thickness2 / 2, dy, teflon_outer / 2);
G4Box* solidLegX = new G4Box("LegX", teflon_inner / 2, dy, teflon_thickness2 / 2);
G4ThreeVector transLegX( -(teflon_inner + teflon_thickness2) / 2, 0, (teflon_outer - teflon_thickness2) / 2);
G4UnionSolid* solidLBracket = new G4UnionSolid("LBracket", solidLegZ, solidLegX, 0, transLegX);
G4LogicalVolume* logicLBracket = 
    new G4LogicalVolume(solidLBracket, teflon, "LBracket");

// 0도, 90도, 180도, 270도 회전
std::vector<G4RotationMatrix*> rotations;
for (int i = 0; i < 4; ++i) {
    auto rot = new G4RotationMatrix();
    rot->rotateY(-90. * deg * i);
    rotations.push_back(rot);
}

std::vector<G4ThreeVector> original_positions = {
  G4ThreeVector(+dx + teflon_thickness2 /2, 0, +dz + teflon_thickness2 - teflon_outer / 2),
  G4ThreeVector(+dx + teflon_thickness2 - teflon_outer / 2, 0, -dz - teflon_thickness2 / 2),
  G4ThreeVector(-dx - teflon_thickness2 /2, 0, -dz - teflon_thickness2 + teflon_outer / 2),
  G4ThreeVector(-dx - teflon_thickness2 + teflon_outer / 2, 0, +dz + teflon_thickness2 / 2)
};

std::vector<G4VPhysicalVolume*> physBrackets;

for (size_t i = 0; i < 4; ++i) {
    G4ThreeVector final_pos = original_positions[i];
    G4VPhysicalVolume* physBracket = new G4PVPlacement(
        rotations[i], final_pos,
        logicLBracket, "TeflonBracket",
        logicWorld, false, i, checkOverlaps
    );
    physBrackets.push_back(physBracket);
}
for (size_t i = 0; i < 4; ++i) {
  new G4LogicalBorderSurface(
      "TeflonSurface",        // 이름은 중복 가능
      physTarget,
      physBrackets[i],        // 붙은 브래킷 물리 볼륨
      teflonSurface           // optical surface 객체
  );
  new G4LogicalBorderSurface(
    "TeflonSurface",        // 이름은 중복 가능
    physWorld,
    physBrackets[i],        // 붙은 브래킷 물리 볼륨
    teflonSurface           // optical surface 객체
);
}
//*/

G4Box* solidBottomFull = new G4Box("TeflonBottomFull", dx, teflonThickness/2, dz);
G4Box* solidTopFull    = new G4Box("TeflonTopFull",    dx, teflonThickness/2, dz);

G4Box* solidHole = new G4Box("SiPMHole", sipmSizeXY/2, teflonThickness/2, sipmSizeXY/2); 
// y 두께는 충분히 크게 (관통하도록)

G4SubtractionSolid* solidBottomWithHole = new G4SubtractionSolid(
    "TeflonBottomHole", solidBottomFull, solidHole, nullptr, G4ThreeVector(0, 0, 0)
);

G4SubtractionSolid* solidTopWithHole = new G4SubtractionSolid(
    "TeflonTopHole", solidTopFull, solidHole, nullptr, G4ThreeVector(0, 0, 0)
);

// 논리 볼륨
G4LogicalVolume* logicBottom = new G4LogicalVolume(solidBottomWithHole, teflon, "TeflonBottom");
//G4LogicalVolume* logicBottom = new G4LogicalVolume(solidBottomFull, teflon, "TeflonBottom");
G4LogicalVolume* logicTop    = new G4LogicalVolume(solidTopWithHole,    teflon, "TeflonTop");

    G4Box* teflonLeft   = new G4Box("TeflonLeft",    teflonThickness/2, dy, dz); // -x
    G4Box* teflonRight  = new G4Box("TeflonRight",   teflonThickness/2, dy, dz); // +x
    G4Box* teflonBack   = new G4Box("TeflonBack",    dx, dy, teflonThickness/2); // -z
    G4Box* teflonFront  = new G4Box("TeflonFront",   dx, dy, teflonThickness/2); // +z
    G4LogicalVolume* logicLeft   = new G4LogicalVolume(teflonLeft,   teflon, "TeflonLeft");
    G4LogicalVolume* logicRight  = new G4LogicalVolume(teflonRight,  teflon, "TeflonRight");
    //G4LogicalVolume* logicBack   = new G4LogicalVolume(teflonBack,   teflon, "TeflonBack");
    //G4LogicalVolume* logicFront  = new G4LogicalVolume(teflonFront,  teflon, "TeflonFront");

// 중심 부분을 제거할 박스 (중심 기준으로 y 방향 5cm = 총 10cm 제거)
G4Box* cutBox = new G4Box("CutBox", dx, 5*cm, teflonThickness/2);  
// dx+1, thickness 전체를 넘게 잡는 이유: 완전 관통되도록 하기 위함

// 위치는 y=0에 놓아서 중앙 제거
G4SubtractionSolid* subBack = new G4SubtractionSolid("SubTeflonBack", teflonBack, cutBox, 0, G4ThreeVector(0, 0, 0));
G4SubtractionSolid* subFront = new G4SubtractionSolid("SubTeflonFront", teflonFront, cutBox, 0, G4ThreeVector(0, 0, 0));

// 논리 볼륨 생성
G4LogicalVolume* logicBack = new G4LogicalVolume(subBack, teflon, "TeflonBack");
G4LogicalVolume* logicFront = new G4LogicalVolume(subFront, teflon, "TeflonFront");
    /*
    G4ThreeVector posLeft   = G4ThreeVector(-dx - teflonThickness/2 - teflon_thickness2, 0, 0);
    G4ThreeVector posRight  = G4ThreeVector( dx + teflonThickness/2 + teflon_thickness2, 0, 0);
    G4ThreeVector posBack   = G4ThreeVector(0, 0, -dz - teflonThickness/2 - teflon_thickness2);
    G4ThreeVector posFront  = G4ThreeVector(0, 0,  dz + teflonThickness/2 + teflon_thickness2);
    G4ThreeVector posBottom = G4ThreeVector(0, -dy - teflonThickness/2, 0);
    G4ThreeVector posTop = G4ThreeVector(0, +dy + teflonThickness/2, 0);
    */
    ///*
    G4ThreeVector posBottom = G4ThreeVector(0, -dy - teflonThickness/2, 0);
    G4ThreeVector posTop = G4ThreeVector(0, +dy + teflonThickness/2, 0);
    G4ThreeVector posLeft   = G4ThreeVector(-dx - teflonThickness/2, 0, 0);
    G4ThreeVector posRight  = G4ThreeVector( dx + teflonThickness/2, 0, 0);
    G4ThreeVector posBack   = G4ThreeVector(0, 0, -dz - teflonThickness/2);
    G4ThreeVector posFront  = G4ThreeVector(0, 0,  dz + teflonThickness/2);
    //*/
    ///*
    G4VPhysicalVolume* physTefLeft = new G4PVPlacement(0, posLeft,   logicLeft,   "TeflonLeft",   logicWorld, false, 0, checkOverlaps);
    G4VPhysicalVolume* physTefRight =new G4PVPlacement(0, posRight,  logicRight,  "TeflonRight",  logicWorld, false, 0, checkOverlaps);
    G4VPhysicalVolume* physTefBottom = new G4PVPlacement(0, posBottom, logicBottom, "TeflonBottom", logicWorld, false, 0, checkOverlaps);
    G4VPhysicalVolume* physTefTop = new G4PVPlacement(0, posTop, logicTop, "TeflonTop", logicWorld, false, 0, checkOverlaps);
    G4VPhysicalVolume* physTefBack = new G4PVPlacement(0, posBack,   logicBack,   "TeflonBack",   logicWorld, false, 0, checkOverlaps);
    G4VPhysicalVolume* physTefFront = new G4PVPlacement(0, posFront,  logicFront,  "TeflonFront",  logicWorld, false, 0, checkOverlaps);
    ///*
    new G4LogicalBorderSurface("SurfaceLeft",   physTarget, physTefLeft,   teflonSurface);
    new G4LogicalBorderSurface("SurfaceRight",  physTarget, physTefRight,  teflonSurface);
    new G4LogicalBorderSurface("SurfaceBottom", physTarget, physTefBottom,  teflonSurface);
    new G4LogicalBorderSurface("SurfaceTop", physTarget, physTefTop,  teflonSurface);
    new G4LogicalBorderSurface("SurfaceBack",   physTarget, physTefBack,  teflonSurface);
    new G4LogicalBorderSurface("SurfaceFront",  physTarget, physTefFront,  teflonSurface);
    //*/
    /*
    new G4LogicalBorderSurface("SurfaceLeft",   physWorld, physTefLeft,   teflonSurface);
    new G4LogicalBorderSurface("SurfaceRight",  physWorld, physTefRight,  teflonSurface);
    new G4LogicalBorderSurface("SurfaceBottom", physTarget, physTefBottom,  teflonSurface);
    new G4LogicalBorderSurface("SurfaceTop", physTarget, physTefTop,  teflonSurface);
    new G4LogicalBorderSurface("SurfaceBack",   physWorld, physTefBack,  teflonSurface);
    new G4LogicalBorderSurface("SurfaceFront",  physWorld, physTefFront,  teflonSurface);
    */
/*
    G4Material* aluminum = nist->FindOrBuildMaterial("G4_Al");
    G4double alThickness = 50 * micrometer;
    //???why 50*micrometer gets too big?
    G4Box* alLeft   = new G4Box("AlLeft",   alThickness/2, dy, dz);
    G4Box* alRight  = new G4Box("AlRight",  alThickness/2, dy, dz);
    G4Box* alBottom = new G4Box("AlBottom", dx, alThickness/2, dz);
    G4Box* alBack   = new G4Box("AlBack",   dx, dy, alThickness/2);
    G4Box* alFront  = new G4Box("AlFront",  dx, dy, alThickness/2);
    G4LogicalVolume* logicAlLeft   = new G4LogicalVolume(alLeft,   aluminum, "AlLeft");
    G4LogicalVolume* logicAlRight  = new G4LogicalVolume(alRight,  aluminum, "AlRight");
    G4LogicalVolume* logicAlBottom = new G4LogicalVolume(alBottom, aluminum, "AlBottom");
    G4LogicalVolume* logicAlBack   = new G4LogicalVolume(alBack,   aluminum, "AlBack");
    G4LogicalVolume* logicAlFront  = new G4LogicalVolume(alFront,  aluminum, "AlFront");
    G4ThreeVector posAlLeft   = G4ThreeVector(-dx - teflonThickness - alThickness/2, 0, 0);
    G4ThreeVector posAlRight  = G4ThreeVector( dx + teflonThickness + alThickness/2, 0, 0);
    G4ThreeVector posAlBottom = G4ThreeVector(0, -dy - teflonThickness - alThickness/2, 0);
    G4ThreeVector posAlBack   = G4ThreeVector(0, 0, -dz - teflonThickness - alThickness/2);
    G4ThreeVector posAlFront  = G4ThreeVector(0, 0,  dz + teflonThickness + alThickness/2);
    new G4PVPlacement(0, posAlLeft,   logicAlLeft,   "AlLeft",   logicWorld, false, 0, checkOverlaps);
    new G4PVPlacement(0, posAlRight,  logicAlRight,  "AlRight",  logicWorld, false, 0, checkOverlaps);
    //new G4PVPlacement(0, posAlBottom, logicAlBottom, "AlBottom", logicWorld, false, 0, checkOverlaps);
    new G4PVPlacement(0, posAlBack,   logicAlBack,   "AlBack",   logicWorld, false, 0, checkOverlaps);
    new G4PVPlacement(0, posAlFront,  logicAlFront,  "AlFront",  logicWorld, false, 0, checkOverlaps);                    
*/
      // ---- SiPM 크기 설정 ----
  //G4double sipmSizeXY = 0.6 * cm;
  G4double sipmThickness = 0.1 * cm;
  
  // ---- SiPM Material ----
  G4Material* silicon = nist->FindOrBuildMaterial("G4_Si");

  // ---- SiPM Solid / Logic ----
  G4Box* solidSiPM = new G4Box("SiPM", sipmSizeXY/2, sipmThickness/2, sipmSizeXY/2);
  G4LogicalVolume* logicSiPM = new G4LogicalVolume(solidSiPM, silicon, "SiPM");

    // 기존 위치 (위쪽)
    G4ThreeVector posSiPM_Up = G4ThreeVector(0, dy + sipmThickness/2, 0);
    new G4PVPlacement(0, posSiPM_Up, logicSiPM, "SiPM", logicWorld, false, 0, checkOverlaps);
/*
    //G4VPhysicalVolume* physTefBottom = new G4PVPlacement(0, posBottom, logicBottom, "TeflonBottom", logicWorld, false, 0, checkOverlaps);
    //new G4LogicalBorderSurface("SurfaceBottom", physTarget, physTefBottom,  teflonSurface);
    //new G4PVPlacement(0, posAlBottom, logicAlBottom, "AlBottom", logicWorld, false, 0, checkOverlaps);
  */
///*
    // 추가 위치 (아래쪽)
    G4ThreeVector posSiPM_Down = G4ThreeVector(0, -dy - sipmThickness/2, 0);
    new G4PVPlacement(0, posSiPM_Down, logicSiPM, "SiPM2", logicWorld, false, 1, checkOverlaps);
  //*/


  fScoringVolume1 = logicWorld;
  fScoringVolume2 = logicTarget;
  fScoringVolume3 = logicSiPM;

  return physWorld;
}

