// Leaf: ?IsToFaceTarget@CCreatureAction_JackShotNear@@UBE_NXZ
// CCreatureAction_JackShotNear::IsToFaceTarget() const -- returns true.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   @CCreatureAction_JackShotNear_IsToFaceTarget@4 becomes the virtual member on engine/CCreatureAction_JackShotNear.h
//   (FABLE_RETAIL_CCREATUREACTION_JACKSHOTNEAR_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G1 (CONST shape).
#define FABLE_RETAIL_CCREATUREACTION_JACKSHOTNEAR_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_JackShotNear.h"
bool CCreatureAction_JackShotNear::IsToFaceTarget() const
{
    return true;
}
