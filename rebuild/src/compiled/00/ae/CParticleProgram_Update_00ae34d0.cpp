// CParticleProgram::Update() -- sort the named export particles (the vector-map at +0x88) when dirty, then recompute the average patch size from the initial simulation
// Leaf: ?Update@CParticleProgram@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE34D0, length 59 bytes (`ret`, no arguments); no direct callers or vtable references.
// Identity (NF166, round 10, q4364): Ego_r.pdb 0x00BEF280 / FableWin.pdb 0x03334C30 ?Update@CParticleProgram@@QAEXXZ.
//   - The body passes `this+8` (InitialSimulation, Ego_r TPI CParticleProgram +0x08) to the private
//     ?CalculateAveragePatchSize@CParticleProgram@@AAEXABVCParticleSimulation@@@Z 0x00AE0710 on `this`, so the caller is a CParticleProgram member;
//     +0x88 is ExportParticles (CVectorMap<CCharString,unsigned long,...>: Dirty +0x95, Compare +0x94, the /Gr std::sort 0x009B8EF0 that
//     ReadFromFile 0x00AE3C30 also reaches through ExportParticles.Update()).
//   - The no-argument non-const void members of the PDB set are Update, Clear (0x00AE3360), ClearGroups (0x00AE3510), AddConstraintEndRepeat
//     and the private OptimiseConstraints; the last two write / read the Constraints stream (+0x114), which this body never touches.
//   - FableWin.pdb's Update spans 0x40 bytes (to OptimiseConstraints 0x03334C70), the retail span 0x00AE34D0..0x00AE3510.
//   Was the descriptive CSortedMapOwner_00ae34d0::SortAndProcess with local models (catalog label CTCVillage::OnInitialActivate was propagated).
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_READ_PASS24
#define FABLE_RETAIL_CPARTICLEPROGRAM_UPDATE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVECTORMAP_SORT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CDataInputStream.h"
#include "engine/CParticleProgram.h"

void CParticleProgram::Update()
{
    ExportParticles.Update();
    CalculateAveragePatchSize(InitialSimulation);
}
