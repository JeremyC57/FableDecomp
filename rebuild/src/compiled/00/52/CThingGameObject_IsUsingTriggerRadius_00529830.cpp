// CThingGameObject::IsUsingTriggerRadius() const -- returns false
// Leaf: ?IsUsingTriggerRadius@CThingGameObject@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529830, thiscall, `ret`.
// Identity: slot 61 of CThingTrackNode's vtable 0x01272CE4 (defined here: the slot holds this body in 12 vtable(s), all of CThingGameObject or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 27 TLC-named slots of that vtable puts IsUsingTriggerRadius(void) at this slot; Ego_r/FableWin name set: ?IsUsingTriggerRadius@CThingGameObject@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CThingGameObject.h (opt-ins as in the landed row CThingGameObject_GetThingStandingOn_00529930.cpp).
#define FABLE_RETAIL_CTHINGGAMEOBJECT_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STANDINGON
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
bool CThingGameObject::IsUsingTriggerRadius() const
{
    return false;
}
