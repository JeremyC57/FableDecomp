// Leaf: ?GetIsToFaceTargetCone@CCreatureAction_LeapBack@@UBEMXZ
// CCreatureAction_LeapBack::GetIsToFaceTargetCone() const -- returns 0.25f (retail loads the pooled float literal at 0x0123078C).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + Ego_r/FableWin name set; the landed stand-in
//   _CCreatureAction_LeapBack_GetIsToFaceTargetCone becomes the virtual member on engine/CCreatureAction_LeapBack.h
//   (FABLE_RETAIL_CCREATUREACTION_LEAPBACK_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G1 (CONST shape).
#define FABLE_RETAIL_CCREATUREACTION_LEAPBACK_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_LeapBack.h"
float CCreatureAction_LeapBack::GetIsToFaceTargetCone() const
{
    return 0.25f;
}
