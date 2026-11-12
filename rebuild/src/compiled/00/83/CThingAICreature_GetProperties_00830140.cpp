// CThingAICreature::GetProperties() const -- returns the definition's Property (EThingCreatureProperty, opaque in the shared header)
// Leaf: ?GetProperties@CThingAICreature@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830140, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 48 of CThingAICreature's vtable 0x0127293C (the slot holds this body in 1 vtable(s), owner CThingAICreature); the Xbox 360 Test PDB virtual order (third source) aligned on 37 TLC-named slots of that vtable puts GetProperties(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetProperties@CThingAICreature@@UBEKXZ; `ret` matches the signature.
// Was: CThingAICreature::vfunc_48 (rebuild/src/compiled/00/83/CThingAICreature_vfunc48_00830140.cpp).
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
unsigned long CThingAICreature::GetProperties() const
{
    return *reinterpret_cast<const unsigned long*>(PDef->Property);
}
