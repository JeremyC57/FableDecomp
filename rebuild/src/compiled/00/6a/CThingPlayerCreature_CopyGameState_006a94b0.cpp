// CThingPlayerCreature::CopyGameState(CThing const&) -- copies the CThingCreatureBase game state (tail call to CThingCreatureBase::CopyGameState 0x006619D0)
// Leaf: ?CopyGameState@CThingPlayerCreature@@UAEXABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A94B0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 7 of CThingPlayerCreature's vtable 0x012457FC (the slot holds this body in 1 vtable(s), owner CThingPlayerCreature); the Xbox 360 Test PDB virtual order (third source) aligned on 33 TLC-named slots of that vtable puts CopyGameState(CThing const &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?CopyGameState@CThingPlayerCreature@@UAEXABVCThing@@@Z; no `ret` to check (NORET).
// Was: CThingPlayerCreature::vfunc_7 (rebuild/src/compiled/00/6a/gapscan_sub006a94b0_006a94b0.cpp).
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_VSLOTS2
#define FABLE_RETAIL_CTHINGCREATUREBASE_VSLOTS2
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/EMovementType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingPlayerCreature.h"
void CThingPlayerCreature::CopyGameState(const CThing& thing)
{
    CThingCreatureBase::CopyGameState(thing);
}
