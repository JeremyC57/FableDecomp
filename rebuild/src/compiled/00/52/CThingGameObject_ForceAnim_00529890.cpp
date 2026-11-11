// CThingGameObject::ForceAnim(CCharString const &, bool) -- does nothing
// Leaf: ?ForceAnim@CThingGameObject@@UAEXABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529890, thiscall, `ret 8`.
// Identity: slot 69 of CThingTrackNode's vtable 0x01272CE4 (defined here: the slot holds this body in 10 vtable(s), all of CThingGameObject or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 27 TLC-named slots of that vtable puts ForceAnim(CCharString const &, bool) at this slot; Ego_r/FableWin name set: ?ForceAnim@CThingGameObject@@UAEXABVCCharString@@_N@Z; `ret` matches the signature.
// Header: engine/CThingGameObject.h (opt-ins as in the landed row CThingGameObject_GetThingStandingOn_00529930.cpp).
class CCharString;
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
void CThingGameObject::ForceAnim(CCharString const &, bool)
{
}
