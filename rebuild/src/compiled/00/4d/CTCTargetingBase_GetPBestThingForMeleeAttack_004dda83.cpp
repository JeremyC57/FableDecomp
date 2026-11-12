// CTCTargetingBase::GetPBestThingForMeleeAttack() const -- returns 0
// Leaf: ?GetPBestThingForMeleeAttack@CTCTargetingBase@@UBEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DDA83, thiscall, `ret`.
// Identity: slot 28 of CTCTargetingBase's vtable 0x01271E74 (defined here: the slot holds this body in 2 vtable(s), all of CTCTargetingBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts GetPBestThingForMeleeAttack(void) at this slot; Ego_r/FableWin name set: ?GetPBestThingForMeleeAttack@CTCTargetingBase@@UBEPAVCThing@@XZ; `ret` matches the signature.
// Header: engine/CTCTargetingBase.h (opt-ins as in the landed row CTCSpecialAbilities_SetupDrainLifeSpell_0072b260.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_PLAYER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CPlayer.h"
CThing* CTCTargetingBase::GetPBestThingForMeleeAttack() const
{
    return 0;
}
