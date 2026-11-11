// CThingCreatureBase::CopyGameState(CThing const&) -- copies the CThingGameObject game state (tail call to CThingGameObject::CopyGameState 0x00838940)
// Leaf: ?CopyGameState@CThingCreatureBase@@UAEXABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006619D0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 7 of CThingAICreature's vtable 0x0127293C (the slot holds this body in 2 vtable(s), owner CThingCreatureBase); the Xbox 360 Test PDB virtual order (third source) aligned on 37 TLC-named slots of that vtable puts CopyGameState(CThing const &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?CopyGameState@CThingCreatureBase@@UAEXABVCThing@@@Z; no `ret` to check (NORET).
// Was: CThingCreatureBase::vfunc_7 (rebuild/src/compiled/00/66/global_StubReturnVoid004c72a0_006619d0.cpp).
#define FABLE_RETAIL_CTHINGGAMEOBJECT_VSLOTS2
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
void CThingCreatureBase::CopyGameState(const CThing& thing)
{
    CThingGameObject::CopyGameState(thing);
}
