// CThingAICreature::IsAffectedByDamage() const -- returns true
// Leaf: ?IsAffectedByDamage@CThingAICreature@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00835910, thiscall, `ret`.
// Identity: slot 47 of CThingAICreature's vtable 0x0127293C (defined here: the slot holds this body in 1 vtable(s), all of CThingAICreature or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 37 TLC-named slots of that vtable puts IsAffectedByDamage(void) at this slot; Ego_r/FableWin name set: ?IsAffectedByDamage@CThingAICreature@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CThingAICreature.h (opt-ins as in the landed row CMessageEventFilterCanBeHeard_operator_006af270.cpp).
#define FABLE_RETAIL_CTHINGAICREATURE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_MESSAGERANGE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThingAICreature.h"
#include "engine/CMessageEventFilter_CanBeHeard.h"
bool CThingAICreature::IsAffectedByDamage() const
{
    return true;
}
