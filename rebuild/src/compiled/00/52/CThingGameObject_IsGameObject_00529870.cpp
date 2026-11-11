// CThingGameObject::IsGameObject() const -- returns true
// Leaf: ?IsGameObject@CThingGameObject@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529870, thiscall, `ret`.
// Identity: slot 66 of CThingTrackNode's vtable 0x01272CE4 (defined here: the slot holds this body in 13 vtable(s), all of CThingGameObject or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 27 TLC-named slots of that vtable puts IsGameObject(void) at this slot; Ego_r/FableWin name set: ?IsGameObject@CThingGameObject@@UBE_NXZ; `ret` matches the signature.
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
bool CThingGameObject::IsGameObject() const
{
    return true;
}
