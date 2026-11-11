// CTCPhysicsBase::GetRHSet() const -- returns the static DummyRHSet (the base physics body has no orientation)
// Leaf: ?GetRHSet@CTCPhysicsBase@@UBEABVCRightHandedSet@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1290, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 72 of CTCPhysicsLight's vtable 0x01265DFC (the slot holds this body in 2 vtable(s), owner CTCPhysicsBase); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts GetRHSet(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetRHSet@CTCPhysicsBase@@UBEABVCRightHandedSet@@XZ; `ret` matches the signature.
// Was: CTCPhysicsBase::vfunc_72 (rebuild/src/compiled/00/6b/gapscan_sub006b1290_006b1290.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_DUMMYRHSET
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"
const CRightHandedSet& CTCPhysicsBase::GetRHSet() const
{
    return DummyRHSet;
}
