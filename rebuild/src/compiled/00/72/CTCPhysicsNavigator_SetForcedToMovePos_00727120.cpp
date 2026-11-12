// CTCPhysicsNavigator::SetForcedToMovePos(C3DVector const&, bool) -- records the forced move-to position and whether it uses physics, and sets ForcedMoveToPosSet
// Leaf: ?SetForcedToMovePos@CTCPhysicsNavigator@@UAEXABVC3DVector@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00727120, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 34 of CTCPhysicsNavigator's vtable 0x01266314 (the slot holds this body in 1 vtable(s), owner CTCPhysicsNavigator); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts SetForcedToMovePos(C3DVector const &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetForcedToMovePos@CTCPhysicsNavigator@@UAEXABVC3DVector@@_N@Z; `ret` matches the signature.
// Was: CTCPhysicsNavigator::vfunc_34 (rebuild/src/compiled/00/72/CTCPhysicsNavigator_vfunc34_00727120.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSNAVIGATOR
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsNavigator.h"
void CTCPhysicsNavigator::SetForcedToMovePos(const C3DVector& pos, bool usePhysics)
{
    ForcedMoveToPos = pos;
    ForcedMoveToPosUsePhysics = usePhysics;
    ForcedMoveToPosSet = true;
}
