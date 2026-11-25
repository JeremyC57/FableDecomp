// Leaf: ?IsToFaceTargetAtStart@CCreatureAction_Charge@@UBE_NXZ
// CCreatureAction_Charge::IsToFaceTargetAtStart() const -- returns true.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   ?CCreatureAction_Charge_IsToFaceTargetAtStart@@YI_NXZ becomes the virtual member on engine/CCreatureAction_Charge.h
//   (FABLE_RETAIL_CCREATUREACTION_CHARGE_VIRTUAL_MODEL, Needs FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G1 (CONST shape).
#define FABLE_RETAIL_CCREATUREACTION_CHARGE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_Charge.h"
bool CCreatureAction_Charge::IsToFaceTargetAtStart() const
{
    return true;
}
