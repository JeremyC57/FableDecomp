// NThingShot::CTCShotDefinitionBase::IsCreateParticleEffectOnHit() const -- returns true
// Leaf: ?IsCreateParticleEffectOnHit@CTCShotDefinitionBase@NThingShot@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5487, thiscall, `ret`.
// Identity: slot 43 of NThingShot::CTCShotDefinitionBase's vtable 0x0126E1A4 (defined here: the slot holds this body in 13 vtable(s), all of NThingShot::CTCShotDefinitionBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 25 TLC-named slots of that vtable puts IsCreateParticleEffectOnHit(void) at this slot; Ego_r/FableWin name set: ?IsCreateParticleEffectOnHit@CTCShotDefinitionBase@NThingShot@@MBE_NXZ; `ret` matches the signature.
// Header: engine/NThingShot_CTCShotDefinitionBase.h (opt-ins as in the landed row NThingShotCTCShotDefinitionBase_GetPWeapon_007beff0.cpp).
#define FABLE_RETAIL_NTHINGSHOT_CTCSHOTDEFINITIONBASE_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThing.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
bool NThingShot::CTCShotDefinitionBase::IsCreateParticleEffectOnHit() const
{
    return true;
}
