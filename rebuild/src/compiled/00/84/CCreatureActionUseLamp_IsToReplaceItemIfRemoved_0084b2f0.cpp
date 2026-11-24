// Leaf: ?IsToReplaceItemIfRemoved@CCreatureAction_UseLamp@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_USELAMP_RTTI_DECLS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_USELAMP
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CCreatureAction_UseLamp.h"
bool CCreatureAction_UseLamp::IsToReplaceItemIfRemoved() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_UseLamp_IsToReplaceItemIfRemoved -> CCreatureAction_UseLamp::IsToReplaceItemIfRemoved, the RTTI-placed PDB member; class model: engine/CCreatureAction_UseLamp.h opt-in FABLE_RETAIL_CCREATUREACTION_USELAMP, FABLE_RETAIL_CCREATUREACTION_USELAMP_RTTI_DECLS (preamble of CCreatureActionUseLamp_Initialise_00847df0.cpp).
