// CThingAICreature::GetMaxFlyingSpeed(C3DVector const&) const -- returns MaxFlyingSpeed (the direction is unused)
// Leaf: ?GetMaxFlyingSpeed@CThingAICreature@@UBEMABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830090, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 83 of CThingAICreature's vtable 0x0127293C (the slot holds this body in 1 vtable(s), owner CThingAICreature); the Xbox 360 Test PDB virtual order (third source) aligned on 37 TLC-named slots of that vtable puts GetMaxFlyingSpeed(C3DVector const &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetMaxFlyingSpeed@CThingAICreature@@UBEMABVC3DVector@@@Z; `ret` matches the signature.
// Was: CThingAICreature::vfunc_83 (rebuild/src/compiled/00/83/gapscan_sub00830090_00830090.cpp).
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_VSLOTS2
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGAICREATURE_INITFROMDEF
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
float CThingAICreature::GetMaxFlyingSpeed(const C3DVector&) const
{
    return MaxFlyingSpeed;
}
