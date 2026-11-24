// Leaf: ?IsToReplaceItemIfRemoved@CCreatureAction_BlowHorn@@UBE_NXZ
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_BLOWHORN
#include "engine/CCreatureAction_BlowHorn.h"
bool CCreatureAction_BlowHorn::IsToReplaceItemIfRemoved() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_BlowHorn_IsToReplaceItemIfRemoved -> CCreatureAction_BlowHorn::IsToReplaceItemIfRemoved, the RTTI-placed PDB member; class model: engine/CCreatureAction_BlowHorn.h under FABLE_RETAIL_CCREATUREACTION_BLOWHORN (preamble of CTCActionUseBlowHorn_OnActionUse_007eb9b0.cpp).
