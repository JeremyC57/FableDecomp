// CThingPhysical::CopyGameState(CThing const&) -- copies the CThing game state (tail call to CThing::CopyGameState 0x004C72A0)
// Leaf: ?CopyGameState@CThingPhysical@@UAEXABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A4B70, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 7 of CThingPhysical's vtable 0x01244E84 (the slot holds this body in 1 vtable(s), owner CThingPhysical); the Xbox 360 Test PDB virtual order (third source) aligned on 26 TLC-named slots of that vtable puts CopyGameState(CThing const &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?CopyGameState@CThingPhysical@@UAEXABVCThing@@@Z; no `ret` to check (NORET).
// Was: CThingPhysical::vfunc_7 (rebuild/src/compiled/00/6a/global_StubReturnVoid004c72a0_006a4b70.cpp).
#define FABLE_RETAIL_CTHINGPHYSICAL_VSLOTS2
#define FABLE_RETAIL_CTHING_LOADSINGLE
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
void CThingPhysical::CopyGameState(const CThing& thing)
{
    CThing::CopyGameState(thing);
}
