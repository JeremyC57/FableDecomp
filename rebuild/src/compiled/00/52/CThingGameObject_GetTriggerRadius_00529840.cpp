// CThingGameObject::GetTriggerRadius() const -- the base game object has no trigger radius: returns 0
// Leaf: ?GetTriggerRadius@CThingGameObject@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529840, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 62 of CThingTrackNode's vtable 0x01272CE4 (the slot holds this body in 12 vtable(s), owner CThingGameObject); the Xbox 360 Test PDB virtual order (third source) aligned on 27 TLC-named slots of that vtable puts GetTriggerRadius(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetTriggerRadius@CThingGameObject@@UBEMXZ; `ret` matches the signature.
// Was: CThingGameObject::vfunc_62 (rebuild/src/compiled/00/52/CThingGameObject_vfunc62_00529840.cpp).
#define FABLE_RETAIL_CTHINGGAMEOBJECT_VSLOTS2
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
float CThingGameObject::GetTriggerRadius() const
{
    return 0.0f;
}
