// Leaf: ?GetIsToFaceTargetCone@CCreatureAction_GenericFireProjectileWeapon@@UBEMXZ
// CCreatureAction_GenericFireProjectileWeapon::GetIsToFaceTargetCone() const -- returns 1.0f (retail loads the pooled float literal at 0x0122DED8).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + Ego_r/FableWin name set; the landed stand-in
//   ?CCreatureAction_GenericFireProjectileWeapon__GetIsToFaceTargetCone@@YIMPAX@Z becomes the virtual member on
//   engine/CCreatureAction_GenericFireProjectileWeapon.h (FABLE_RETAIL_CCREATUREACTION_GENERICFIREPROJECTILEWEAPON_VIRTUAL_MODEL, Needs
//   FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G1 (CONST
//   shape).
#define FABLE_RETAIL_CCREATUREACTION_GENERICFIREPROJECTILEWEAPON_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_GenericFireProjectileWeapon.h"
float CCreatureAction_GenericFireProjectileWeapon::GetIsToFaceTargetCone() const
{
    return 1.0f;
}
