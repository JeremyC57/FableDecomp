// CThingPlayerCreature::GetMaxAcceleration() const -- returns the definition's MaxAcceleration
// Leaf: ?GetMaxAcceleration@CThingPlayerCreature@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A9C10, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 87 of CThingPlayerCreature's vtable 0x012457FC (the slot holds this body in 1 vtable(s), owner CThingPlayerCreature); the Xbox 360 Test PDB virtual order (third source) aligned on 33 TLC-named slots of that vtable puts GetMaxAcceleration(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetMaxAcceleration@CThingPlayerCreature@@UBEMXZ; `ret` matches the signature.
// Was: CThingPlayerCreature::vfunc_87 (rebuild/src/compiled/00/6a/CThingPlayerCreature_vfunc87_006a9c10.cpp).
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_VSLOTS2
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
#include "engine/CThingPlayerCreature.h"
float CThingPlayerCreature::GetMaxAcceleration() const
{
    return PDef->MaxAcceleration;
}
