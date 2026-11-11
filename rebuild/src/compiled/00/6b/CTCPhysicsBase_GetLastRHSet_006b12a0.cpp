// CTCPhysicsBase::GetLastRHSet() const -- returns the static DummyRHSet (the base physics body has no orientation)
// Leaf: ?GetLastRHSet@CTCPhysicsBase@@UBEABVCRightHandedSet@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B12A0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 73 of CTCPhysicsLight's vtable 0x01265DFC (the slot holds this body in 2 vtable(s), owner CTCPhysicsBase); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts GetLastRHSet(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetLastRHSet@CTCPhysicsBase@@UBEABVCRightHandedSet@@XZ; `ret` matches the signature.
// Was: CTCPhysicsBase::vfunc_73 (rebuild/src/compiled/00/6b/gapscan_sub006b12a0_006b12a0.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_DUMMYRHSET
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"
const CRightHandedSet& CTCPhysicsBase::GetLastRHSet() const
{
    return DummyRHSet;
}
