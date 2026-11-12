// CTCPhysicalShieldSpell::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCPhysicalShieldSpell@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079F500, thiscall, `ret`.
// Identity: slot 8 of CTCPhysicalShieldSpell's vtable 0x0126C53C (defined here: the slot holds this body in 1 vtable(s), all of CTCPhysicalShieldSpell or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCPhysicalShieldSpell@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCPhysicalShieldSpell.h (opt-ins as in the landed row CCreatureActionCastPhysicalShieldSpell_Initialise_008b9dd0.cpp).
#define FABLE_RETAIL_CTCPHYSICALSHIELDSPELL_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTION_CASTPHYSICALSHIELD
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureAction_CastPhysicalShieldSpell.h"
#include "engine/CTCPhysicalShieldSpell.h"
bool CTCPhysicalShieldSpell::HasFrameUpdate() const
{
    return true;
}
