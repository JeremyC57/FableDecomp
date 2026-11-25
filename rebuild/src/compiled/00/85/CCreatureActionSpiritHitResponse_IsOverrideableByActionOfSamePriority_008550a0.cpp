// Leaf: ?IsOverrideableByActionOfSamePriority@CCreatureAction_SpiritHitResponse@@UBE_NXZ
// CCreatureAction_SpiritHitResponse::IsOverrideableByActionOfSamePriority() const -- returns true.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   ?CCreatureAction_SpiritHitResponse_IsOverrideableByActionOfSamePriority@@YI_NXZ becomes the virtual member on
//   engine/CCreatureAction_SpiritHitResponse.h (FABLE_RETAIL_CCREATUREACTION_SPIRITHITRESPONSE_VIRTUAL_MODEL, Needs
//   FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_CCREATUREACTIONBASE). NF427 G1 (CONST shape).
#define FABLE_RETAIL_CCREATUREACTION_SPIRITHITRESPONSE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureAction_SpiritHitResponse.h"
bool CCreatureAction_SpiritHitResponse::IsOverrideableByActionOfSamePriority() const
{
    return true;
}
