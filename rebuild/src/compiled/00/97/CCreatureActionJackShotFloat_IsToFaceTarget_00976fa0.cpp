// Leaf: ?IsToFaceTarget@CCreatureAction_JackShotFloat@@UBE_NXZ
// CCreatureAction_JackShotFloat::IsToFaceTarget() const -- returns true.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   @CCreatureAction_JackShotFloat_IsToFaceTarget@4 becomes the virtual member on engine/CCreatureAction_JackShotFloat.h
//   (FABLE_RETAIL_CCREATUREACTION_JACKSHOTFLOAT_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G1 (CONST shape).
#define FABLE_RETAIL_CCREATUREACTION_JACKSHOTFLOAT_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_JackShotFloat.h"
bool CCreatureAction_JackShotFloat::IsToFaceTarget() const
{
    return true;
}
