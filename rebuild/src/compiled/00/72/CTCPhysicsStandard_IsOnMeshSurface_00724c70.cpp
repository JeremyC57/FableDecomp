// CTCPhysicsStandard::IsOnMeshSurface() const -- returns OnMesh
// Leaf: ?IsOnMeshSurface@CTCPhysicsStandard@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724C70, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 55 of CTCPhysicsNavigator's vtable 0x01266314 (the slot holds this body in 3 vtable(s), owner CTCPhysicsStandard); the Xbox 360 Test PDB virtual order (third source) aligned on 46 TLC-named slots of that vtable puts IsOnMeshSurface(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsOnMeshSurface@CTCPhysicsStandard@@UBE_NXZ; `ret` matches the signature.
// Was: CTCPhysicsStandard::vfunc_55 (rebuild/src/compiled/00/72/CTCPhysicsStandard_vfunc55_00724c70.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_OVERRIDES
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_FLAGS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
bool CTCPhysicsStandard::IsOnMeshSurface() const
{
    return OnMesh;
}
