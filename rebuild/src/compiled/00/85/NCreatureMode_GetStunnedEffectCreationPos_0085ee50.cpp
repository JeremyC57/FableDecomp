// NCreatureMode::GetStunnedEffectCreationPos(const CThingCreatureBase& creature) -- where the stunned effect is spawned: the creature's head position raised by 0.75
// Leaf: ?GetStunnedEffectCreationPos@NCreatureMode@@YI?AVC3DVector@@ABVCThingCreatureBase@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085EE50, length 53 bytes (+11 int3). `__fastcall` free function: the hidden
//   C3DVector result in ecx, the creature in edx, plain `ret`.
// Mangled (Ego_r.pdb): ?GetStunnedEffectCreationPos@NCreatureMode@@YI?AVC3DVector@@ABVCThingCreatureBase@@@Z.
// Name: the catalog row says "GetHeadPosWithOffset" (descriptive); the only caller is NCreatureMode::CModeOutOfActionBase::
//   OnActivate 0x008D7270 (the stun effect), and the PDB's GetStunnedEffectCreationPos is the NCreatureMode helper with this
//   exact signature (C3DVector by value from a const CThingCreatureBase&).
// Callees: CThingCreatureBase::GetHeadPos(bool) const 0x00661B40 (C3DVector through the hidden pointer, built in the frame).
// Data: 0.75f @ 0x01238174.
// Shape: C3DVector has a user constructor (hidden-pointer return); the result is constructed from the head position's
//   X and Y (dword moves) and Z + 0.75f (x87), as a by-value return of a temporary.
#include "engine/C3DVector.h"
class CThingCreatureBase {
public:
    C3DVector GetHeadPos(bool includeOffset) const;     // 0x00661B40
};
namespace NCreatureMode {
C3DVector __fastcall GetStunnedEffectCreationPos(const CThingCreatureBase& creature);
}

C3DVector __fastcall NCreatureMode::GetStunnedEffectCreationPos(const CThingCreatureBase& creature)
{
    C3DVector head = creature.GetHeadPos(true);
    return C3DVector(head.X, head.Y, head.Z + 0.75f);
}