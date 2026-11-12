// CThingGameObject::CopyGameState(CThing const&) -- copies the CThingPhysical game state (tail call to CThingPhysical::CopyGameState 0x006A4B70)
// Leaf: ?CopyGameState@CThingGameObject@@UAEXABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00838940, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 7 of CThingTrackNode's vtable 0x01272CE4 (the slot holds this body in 10 vtable(s), owner CThingGameObject); the Xbox 360 Test PDB virtual order (third source) aligned on 27 TLC-named slots of that vtable puts CopyGameState(CThing const &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?CopyGameState@CThingGameObject@@UAEXABVCThing@@@Z; no `ret` to check (NORET).
// Was: CThingGameObject::vfunc_7 (rebuild/src/compiled/00/83/global_StubReturnVoid004c72a0_00838940.cpp).
#define FABLE_RETAIL_CTHINGPHYSICAL_VSLOTS2
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
void CThingGameObject::CopyGameState(const CThing& thing)
{
    CThingPhysical::CopyGameState(thing);
}
