// Leaf: ?IsToFaceTarget@CCreatureAction_NymphShot@@UBE_NXZ
// CCreatureAction_NymphShot::IsToFaceTarget() const -- returns true.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   ?CCreatureAction_NymphShot_IsToFaceTarget@@YI_NXZ becomes the virtual member on engine/CCreatureAction_NymphShot.h
//   (FABLE_RETAIL_CCREATUREACTION_NYMPHSHOT_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G1 (CONST shape).
#define FABLE_RETAIL_CCREATUREACTION_NYMPHSHOT_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_NymphShot.h"
bool CCreatureAction_NymphShot::IsToFaceTarget() const
{
    return true;
}
