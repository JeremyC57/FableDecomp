// CThingPlayerCreature::DrawUpdate(CInterpolationInfo const*) -- the CThingCreatureBase draw update (tail call to CThingCreatureBase::DrawUpdate 0x00661A40)
// Leaf: ?DrawUpdate@CThingPlayerCreature@@UAEXPBVCInterpolationInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A9550, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 26 of CThingPlayerCreature's vtable 0x012457FC (the slot holds this body in 1 vtable(s), owner CThingPlayerCreature); the Xbox 360 Test PDB virtual order (third source) aligned on 33 TLC-named slots of that vtable puts DrawUpdate(CInterpolationInfo const *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?DrawUpdate@CThingPlayerCreature@@UAEXPBVCInterpolationInfo@@@Z; no `ret` to check (NORET).
// Was: CThingPlayerCreature::vfunc_26 (rebuild/src/compiled/00/6a/gapscan_sub006a9550_006a9550.cpp).
class CInterpolationInfo;                                                // the FABLE_RETAIL_CTHINGCREATUREBASE_VSLOTS DrawUpdate parameter
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_VSLOTS2
#define FABLE_RETAIL_CTHINGCREATUREBASE_VSLOTS
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
void CThingPlayerCreature::DrawUpdate(const CInterpolationInfo* info)
{
    CThingCreatureBase::DrawUpdate(info);
}
