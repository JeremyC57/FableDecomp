// CThingCreatureBase::IsHidden() const -- a creature is not hidden: returns false
// Leaf: ?IsHidden@CThingCreatureBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052AB80, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 50 of CThingAICreature's vtable 0x0127293C (the slot holds this body in 3 vtable(s), owner CThingCreatureBase); the Xbox 360 Test PDB virtual order (third source) aligned on 37 TLC-named slots of that vtable puts IsHidden(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsHidden@CThingCreatureBase@@UBE_NXZ; `ret` matches the signature.
// Was: CThingPlayerCreature::vfunc_50 (rebuild/src/compiled/00/52/gapscan_sub0052ab80_0052ab80.cpp).
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
bool CThingCreatureBase::IsHidden() const
{
    return false;
}
