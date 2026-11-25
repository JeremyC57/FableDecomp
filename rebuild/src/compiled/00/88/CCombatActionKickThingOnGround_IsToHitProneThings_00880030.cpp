// Leaf: ?IsToHitProneThings@CCombatAction_KickThingOnGround@@UBE_NXZ
// CCombatAction_KickThingOnGround::IsToHitProneThings() const -- returns true.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   ?CCombatAction_KickThingOnGround_IsToHitProneThings@@YI_NXZ becomes the virtual member on engine/CCombatAction_KickThingOnGround.h
//   (FABLE_RETAIL_CCOMBATACTION_KICKTHINGONGROUND_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G1 (CONST shape).
#define FABLE_RETAIL_CCOMBATACTION_KICKTHINGONGROUND_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCombatAction_KickThingOnGround.h"
bool CCombatAction_KickThingOnGround::IsToHitProneThings() const
{
    return true;
}
