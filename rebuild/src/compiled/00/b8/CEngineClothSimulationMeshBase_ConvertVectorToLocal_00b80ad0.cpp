// Leaf: ?ConvertVectorToLocal@CEngineClothSimulationMeshBase@@IBE?AVC3DVector@@ABV2@@Z
// CEngineClothSimulationMeshBase::ConvertVectorToLocal(const C3DVector& v) const -- rotate a world-space direction into the cloth mesh's local space through the 3x3 part of InverseTransform (+0x78)
// Option S unit (owner ruling, advisor decision 2026-11-24; pilot NF376): this file is ONE retail translation unit's slice and serves
//   three catalog addresses, in retail address order, one body each:
//     0x00B80AD0 ?ConvertVectorToLocal@CEngineClothSimulationMeshBase@@IBE?AVC3DVector@@ABV2@@Z   105 B  (this row)
//     0x00B80F90 ?GetGravityVector@CEngineClothSimulationMeshBase@@UBE?AVC3DVector@@XZ             69 B  (row file includes this TU)
//     0x00B80FE0 ?GetWindVector@CEngineClothSimulationMeshBase@@UBE?AVC3DVector@@XZ               112 B  (row file includes this TU)
//   All three are in one object in both PDBs (Ego_r.pdb Release\Engine_Simulation_Manager.obj #9 / #14 / #15; FableWin.pdb
//   Debug\Engine_Simulation_Manager.obj #25 / #16 / #18). Each address is graded on its own leaf and keeps its own behaviour test.
//   Before the unit, 0x00B80F90 and 0x00B80FE0 each carried a second strong ConvertVectorToLocal (IBE / QBE): retail's callers keep the
//   hidden result pointer in edx across the call (`mov eax,edx` after it), which VC7.1 does only with the callee's ordinary definition
//   earlier in the TU (GOTCHAS hand3c 2026-11-19). Here that definition is this row's own body.
// Plan phase 3 (2026-11-04), C3DVector wave: the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B80AD0, length 105 bytes (+ int3 padding).
// Model: engine/CEngineClothSimulationMeshBase.h (Ego_d.pdb LF_CLASS 0x65D16; retail CArray is 12 bytes, so ParticleSims +0x0c,
//   CurrentTransform +0x18, LastTransform +0x48, InverseTransform +0x78; ConvertVectorToLocal protected const, IBE in both PDBs).
// Shape (0x00B80AD0): C3DVector returned through the hidden pointer (`ret 8`, eax = the result pointer); the three sums are computed X, Y, Z
//   on the x87 stack and stored at the end with `fxch st(2)` + three `fstp` -- a NAMED local built field by field and returned through
//   C3DVector's member-wise user copy constructor (wave 24o). `fld [m] ; fmul [v]` is `v * m` (the right operand is loaded first). The term
//   order (the Y product first in every row, then X/Z) only reproduces statement by statement (`x = a; x += b; x += c;`): a single
//   three-term expression is re-associated by VC7.1 into one canonical order whatever the source order. Kept out of line with
//   `#pragma auto_inline(off)`, as in retail (its two TU-mates call it).
// 0x00B80F90 GetGravityVector: globals 0x01436E68 the CEngineSimulationManager (engine/CEngineSimulationManager.h: ClothGravityFactor
//   +0x1044, GravityVector +0x1048). Identity: slot 1 of vtable 0x012A3924 (RTTI .?AVCEngineClothSimulationMeshBase@@). A named factor
//   local (kept on the x87 stack) and a named C3DVector local of the three products X/Y/Z * factor, passed by reference.
// 0x00B80FE0 GetWindVector: callee CEngineSimulationManager::GetWindVectorNoise 0x00B80B70 (Ego_r.pdb + FableWin.pdb
//   ?GetWindVectorNoise@CEngineSimulationManager@@QBE?AVC3DVector@@ABV2@@Z; was the stand-in CSimulationEnvironment::GetWindAt) on the
//   global 0x01436E68; float pool 0x01230A08 = 100.0f. ONE 12-byte vector local: initialised from CurrentTransform's translation row
//   (FableWin.pdb ?GetTranslation@CMatrix3x4@@QBE?AVC3DVector@@XZ, inline: three dword moves through eax/ecx/edx), passed by reference
//   to GetWindVectorNoise, then overwritten with the scaled wind (X, Y, Z order) read through eax from the returned temporary
//   (ScaleInto below: the landed 0x00B80FE0 text's in-place scale; `v = wind * 100.0f` builds a temporary and copies it, 136 v 112 B).
// The closing `mov eax,edx` in both callers keeps the hidden result pointer in edx across the ConvertVectorToLocal call.
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD   // CMatrix3x4::GetTranslation (FableWin.pdb), inline
#include "engine/C3DVector.h"
#define CEngineSimulationManager CEngineSimulationManagerLayout
#include "engine/CEngineSimulationManager.h"
#undef CEngineSimulationManager
#include "engine/CMatrix3x4.h"
#include "engine/CEngineClothSimulationMeshBase.h"

// As the landed 0x00B80B70 row: the shared layout plus the PDB method this unit calls.
class CEngineSimulationManager : public CEngineSimulationManagerLayout {
public:
    C3DVector GetWindVectorNoise(const C3DVector& position) const;   // 0x00B80B70
};

extern CEngineSimulationManager* g_pEngineSimulationManager;     // 0x01436E68

// 0x00B80AD0
#pragma auto_inline(off)
C3DVector CEngineClothSimulationMeshBase::ConvertVectorToLocal(const C3DVector& v) const
{
    float x = v.Y * InverseTransform.E21;
    x += v.Z * InverseTransform.E31;
    x += v.X * InverseTransform.E11;
    float y = v.Y * InverseTransform.E22;
    y += v.X * InverseTransform.E12;
    y += v.Z * InverseTransform.E32;
    float z = v.Y * InverseTransform.E23;
    z += v.X * InverseTransform.E13;
    z += v.Z * InverseTransform.E33;
    C3DVector local;
    local.X = x;
    local.Y = y;
    local.Z = z;
    return local;
}
#pragma auto_inline(on)

// 0x00B80F90
C3DVector CEngineClothSimulationMeshBase::GetGravityVector() const
{
    const CEngineSimulationManager* manager = g_pEngineSimulationManager;
    float factor = manager->ClothGravityFactor;
    C3DVector gravity(manager->GravityVector_X * factor,
                      manager->GravityVector_Y * factor,
                      manager->GravityVector_Z * factor);
    return ConvertVectorToLocal(gravity);
}

// Descriptive TU-local helper (no PDB name found; the landed 0x00B80FE0 text had it as a local C3DVector member): scale `wind` into `out`.
// Inferred: FableWin.pdb's debug GetWindVector (0x02D79F60) calls GetTranslation, GetWindVectorNoise, C3DVector::operator*(float)
//   and ConvertVectorToLocal, so the source scaled with operator*. Twelve helper-free spellings (four header forms of operator*, three
//   statement shapes, coordinator 2026-11-24) grade 0x00B80FE0 DIFFER(112v112) or DIFFER(122v112); this in-place scale into the
//   translation local is the only form found that reproduces retail's storage reuse. Name and helper are descriptive.
static inline void ScaleInto(const C3DVector& wind, C3DVector& out, float scale) { out.X = wind.X * scale; out.Y = wind.Y * scale; out.Z = wind.Z * scale; }

// 0x00B80FE0
C3DVector CEngineClothSimulationMeshBase::GetWindVector() const
{
    C3DVector v(CurrentTransform.GetTranslation());
    ScaleInto(g_pEngineSimulationManager->GetWindVectorNoise(v), v, 100.0f);
    return ConvertVectorToLocal(v);
}
