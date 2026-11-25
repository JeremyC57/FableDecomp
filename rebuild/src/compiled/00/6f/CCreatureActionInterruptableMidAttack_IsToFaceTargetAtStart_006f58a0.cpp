// Leaf: ?IsToFaceTargetAtStart@CCreatureAction_InterruptableMidAttack@@UBE_NXZ
// CCreatureAction_InterruptableMidAttack::IsToFaceTargetAtStart() const -- returns true.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   ?CCreatureAction_InterruptableMidAttack_IsToFaceTargetAtStart@@YI_NXZ becomes the virtual member on
//   engine/CCreatureAction_InterruptableMidAttack.h (FABLE_RETAIL_CCREATUREACTION_INTERRUPTABLEMIDATTACK_VIRTUAL_MODEL, Needs
//   FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR). NF427 G1 (CONST
//   shape).
#define FABLE_RETAIL_CCREATUREACTION_INTERRUPTABLEMIDATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_InterruptableMidAttack.h"
bool CCreatureAction_InterruptableMidAttack::IsToFaceTargetAtStart() const
{
    return true;
}
