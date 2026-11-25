// Leaf: ?GetIsToFaceTargetCone@CCreatureAction_TrollThrowProjectile@@UBEMXZ
// CCreatureAction_TrollThrowProjectile::GetIsToFaceTargetCone() const -- returns 1.0f (retail loads the pooled float literal at 0x0122DED8).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   ?CCreatureAction_TrollThrowProjectile__GetIsToFaceTargetCone@@YIMPAX@Z becomes the virtual member on
//   engine/CCreatureAction_TrollThrowProjectile.h (FABLE_RETAIL_CCREATUREACTION_TROLLTHROWPROJECTILE_VIRTUAL_MODEL, Needs
//   FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G1 (CONST
//   shape).
#define FABLE_RETAIL_CCREATUREACTION_TROLLTHROWPROJECTILE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_TrollThrowProjectile.h"
float CCreatureAction_TrollThrowProjectile::GetIsToFaceTargetCone() const
{
    return 1.0f;
}
