// CTCPhysicsBase::UpdateInterpolation() -- the base physics component's interpolation update just refreshes the old position (virtual)
// Leaf: ?UpdateInterpolation@CTCPhysicsBase@@MAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AF780, 8 bytes: `mov eax,[ecx] ; jmp [eax+180h]` (tail call of virtual slot 96).
// Identity (two sources): slot 94 of CTCPhysicsBase's vtable 0x0125D2C4 (rtti-vtables.tsv), declared in engine/CTCPhysicsBase.h's
//   PDB order GetPhysicsMeshInfo (93, 0x006B13B0) / UpdateInterpolation (94) / UpdateOldRHSet (95, 0x006B13C0) / UpdateOldPos (96,
//   0x006AF740); Ego_r.pdb emits ?UpdateOldPos@CTCPhysicsBase@@MAEXXZ 0x006EC380 then ?UpdateInterpolation@CTCPhysicsBase@@MAEXXZ
//   0x006EC3C0, as retail 0x006AF740 then 0x006AF780; the body calls slot 96 (+0x180) = UpdateOldPos.
// Callees: virtual CTCPhysicsBase::UpdateOldPos() (slot 96).
// Header: engine/CTCPhysicsBase.h (FABLE_RETAIL_CTCPHYSICSBASE over FABLE_RETAIL_CTCBASE).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CTCPhysicsBase.h"
void CTCPhysicsBase::UpdateInterpolation()
{
    UpdateOldPos();
}