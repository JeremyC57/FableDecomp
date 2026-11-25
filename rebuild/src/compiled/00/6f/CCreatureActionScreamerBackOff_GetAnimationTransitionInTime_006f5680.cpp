// Leaf: ?GetAnimationTransitionInTime@CCreatureAction_ScreamerBackOff@@UBEJXZ
// CCreatureAction_ScreamerBackOff::GetAnimationTransitionInTime() const -- returns 20.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   ?CCreatureAction_ScreamerBackOff_GetAnimationTransitionInTime@@YIHXZ becomes the virtual member on
//   engine/CCreatureAction_ScreamerBackOff.h (FABLE_RETAIL_CCREATUREACTION_SCREAMERBACKOFF_VIRTUAL_MODEL, Needs
//   FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR,
//   FABLE_RETAIL_CCREATUREACTION_STRAFEBACKWARD_VIRTUAL_MODEL). NF427 G1 (CONST shape).
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERBACKOFF_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCREATUREACTION_STRAFEBACKWARD_VIRTUAL_MODEL
#include "engine/CCreatureAction_ScreamerBackOff.h"
long CCreatureAction_ScreamerBackOff::GetAnimationTransitionInTime() const
{
    return 20;
}
