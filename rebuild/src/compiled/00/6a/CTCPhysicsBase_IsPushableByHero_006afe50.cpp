// CTCPhysicsBase::IsPushableByHero() const -- returns the physics def's PushableByHero
// Leaf: ?IsPushableByHero@CTCPhysicsBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AFE50, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 83 of CTCPhysicsRigidBody's vtable 0x0126996C (the slot holds this body in 3 vtable(s), owner CTCPhysicsBase); the Xbox 360 Test PDB virtual order (third source) aligned on 38 TLC-named slots of that vtable puts IsPushableByHero(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsPushableByHero@CTCPhysicsBase@@UBE_NXZ; `ret` matches the signature.
// Was: CTCPhysicsBase::vfunc_83 (rebuild/src/compiled/00/6a/CTCPhysicsBase_vfunc83_006afe50.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_COLLISION_RESPONSE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"
bool CTCPhysicsBase::IsPushableByHero() const
{
    return PhysicsDef->PushableByHero;
}
