// Leaf: ?IsBlockable@CCreatureAction_ScorpionChargeInto@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONCHARGEINTO_RTTI_DECLS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONCHARGEINTO
#include "engine/CCreatureAction_ScorpionChargeInto.h"
bool CCreatureAction_ScorpionChargeInto::IsBlockable() const
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_ScorpionChargeInto_IsBlockable -> CCreatureAction_ScorpionChargeInto::IsBlockable, the RTTI-placed PDB member; class model: engine/CCreatureAction_ScorpionChargeInto.h opt-in FABLE_RETAIL_CCREATUREACTION_SCORPIONCHARGEINTO, FABLE_RETAIL_CCREATUREACTION_SCORPIONCHARGEINTO_RTTI_DECLS (preamble of CAIStateGroupScorpionKing_UpdateCombat_0097bda0.cpp).
