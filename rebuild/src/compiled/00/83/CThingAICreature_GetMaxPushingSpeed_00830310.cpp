// CThingAICreature::GetMaxPushingSpeed(C3DVector const&) const -- returns the definition's MaxPushingSpeed (the direction is unused)
// Leaf: ?GetMaxPushingSpeed@CThingAICreature@@UBEMABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830310, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 78 of CThingAICreature's vtable 0x0127293C (the slot holds this body in 1 vtable(s), owner CThingAICreature); the Xbox 360 Test PDB virtual order (third source) aligned on 37 TLC-named slots of that vtable puts GetMaxPushingSpeed(C3DVector const &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetMaxPushingSpeed@CThingAICreature@@UBEMABVC3DVector@@@Z; `ret` matches the signature.
// Was: CThingAICreature::vfunc_78 (rebuild/src/compiled/00/83/CThingAICreature_vfunc78_00830310.cpp).
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_VSLOTS2
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
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
#include "engine/CThingAICreature.h"
float CThingAICreature::GetMaxPushingSpeed(const C3DVector&) const
{
    return PDef->MaxPushingSpeed;
}
