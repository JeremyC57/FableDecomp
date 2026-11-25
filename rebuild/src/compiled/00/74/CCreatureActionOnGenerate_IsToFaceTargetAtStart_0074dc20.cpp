// Leaf: ?IsToFaceTargetAtStart@CCreatureAction_OnGenerate@@UBE_NXZ
// CCreatureAction_OnGenerate::IsToFaceTargetAtStart() const -- returns true.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   ?CCreatureAction_OnGenerate_IsToFaceTargetAtStart@@YI_NXZ becomes the virtual member on engine/CCreatureAction_OnGenerate.h
//   (FABLE_RETAIL_CCREATUREACTION_ONGENERATE_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G1 (CONST shape).
#define FABLE_RETAIL_CCREATUREACTION_ONGENERATE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_OnGenerate.h"
bool CCreatureAction_OnGenerate::IsToFaceTargetAtStart() const
{
    return true;
}
