// Leaf: ?GetIsToFaceTargetCone@CCreatureAction_BanditKingBackOff@@UBEMXZ
// CCreatureAction_BanditKingBackOff::GetIsToFaceTargetCone() const -- returns 0.125f (retail loads the pooled float literal at 0x0123095C).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + Ego_r/FableWin name set; the landed stand-in
//   _CCreatureAction_BanditKingBackOff_GetIsToFaceTargetCone becomes the virtual member on engine/CCreatureAction_BanditKingBackOff.h
//   (FABLE_RETAIL_CCREATUREACTION_BANDITKINGBACKOFF_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR, FABLE_RETAIL_CCREATUREACTION_STRAFEBACKWARD_PROTOTYPE,
//   FABLE_RETAIL_CCREATUREACTION_STRAFEBACKWARD_RTTI_DECLS). NF427 G1 (CONST shape).
#define FABLE_RETAIL_CCREATUREACTION_BANDITKINGBACKOFF_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCREATUREACTION_STRAFEBACKWARD_PROTOTYPE
#define FABLE_RETAIL_CCREATUREACTION_STRAFEBACKWARD_RTTI_DECLS
#include "engine/CCreatureAction_BanditKingBackOff.h"
float CCreatureAction_BanditKingBackOff::GetIsToFaceTargetCone() const
{
    return 0.125f;
}
