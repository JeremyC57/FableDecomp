// CThingCreatureBase::GetMaterialOnHitByMelee() const -- no melee-hit material: returns 0
// Leaf: ?GetMaterialOnHitByMelee@CThingCreatureBase@@UBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00661D00, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 70 of CThingAICreature's vtable 0x0127293C (the slot holds this body in 3 vtable(s), owner CThingCreatureBase); the Xbox 360 Test PDB virtual order (third source) aligned on 37 TLC-named slots of that vtable puts GetMaterialOnHitByMelee(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetMaterialOnHitByMelee@CThingCreatureBase@@UBEJXZ; `ret` matches the signature.
// Was: CThingPlayerCreature::vfunc_70 (rebuild/src/compiled/00/66/gapscan_sub00661d00_00661d00.cpp).
class CInterpolationInfo;                                                // the FABLE_RETAIL_CTHINGCREATUREBASE_VSLOTS DrawUpdate parameter
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
long CThingCreatureBase::GetMaterialOnHitByMelee() const
{
    return 0;
}
