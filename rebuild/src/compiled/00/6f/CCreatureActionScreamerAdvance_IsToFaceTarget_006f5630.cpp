// Leaf: ?IsToFaceTarget@CCreatureAction_ScreamerAdvance@@UBE_NXZ
// CCreatureAction_ScreamerAdvance::IsToFaceTarget() const -- returns true.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   ?CCreatureAction_ScreamerAdvance_IsToFaceTarget@@YI_NXZ becomes the virtual member on engine/CCreatureAction_ScreamerAdvance.h
//   (FABLE_RETAIL_CCREATUREACTION_SCREAMERADVANCE_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR, FABLE_RETAIL_CCREATUREACTION_STRAFEFORWARD_VIRTUAL_MODEL). NF427 G1
//   (CONST shape).
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERADVANCE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCREATUREACTION_STRAFEFORWARD_VIRTUAL_MODEL
#include "engine/CCreatureAction_ScreamerAdvance.h"
bool CCreatureAction_ScreamerAdvance::IsToFaceTarget() const
{
    return true;
}
