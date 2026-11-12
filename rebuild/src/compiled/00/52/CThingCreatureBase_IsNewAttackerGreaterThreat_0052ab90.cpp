// CThingCreatureBase::IsNewAttackerGreaterThreat(CThingCreatureBase const&, CThingCreatureBase const&) const -- the new attacker is the greater threat: returns true
// Leaf: ?IsNewAttackerGreaterThreat@CThingCreatureBase@@UBE_NABV1@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052AB90, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 74 of CThingCreatureBase's vtable 0x0125AC0C (the slot holds this body in 2 vtable(s), owner CThingCreatureBase); the Xbox 360 Test PDB virtual order (third source) aligned on 33 TLC-named slots of that vtable puts IsNewAttackerGreaterThreat(CThingCreatureBase const &, CThingCreatureBase const &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsNewAttackerGreaterThreat@CThingCreatureBase@@UBE_NABV1@0@Z; `ret` matches the signature.
// Was: CThingPlayerCreature::vfunc_74 (rebuild/src/compiled/00/52/gapscan_sub0052ab90_0052ab90.cpp).
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
bool CThingCreatureBase::IsNewAttackerGreaterThreat(const CThingCreatureBase&, const CThingCreatureBase&) const
{
    return true;
}
