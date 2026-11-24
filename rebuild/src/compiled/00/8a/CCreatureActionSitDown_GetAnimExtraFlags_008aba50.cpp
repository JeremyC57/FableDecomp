// Leaf: ?GetAnimExtraFlags@CCreatureAction_SitDown@@UBEKXZ
#define FABLE_RETAIL_CCREATUREACTION_SITDOWN_RTTI_DECLS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SITDOWN
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureAction_SitDown.h"
unsigned long CCreatureAction_SitDown::GetAnimExtraFlags() const
{
    return 8;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_SitDown_GetAnimExtraFlags -> CCreatureAction_SitDown::GetAnimExtraFlags, the RTTI-placed PDB member; class model: engine/CCreatureAction_SitDown.h opt-in FABLE_RETAIL_CCREATUREACTION_SITDOWN, FABLE_RETAIL_CCREATUREACTION_SITDOWN_RTTI_DECLS (preamble of CCreatureActionSitDown_PerformAction_00848fd0.cpp).
