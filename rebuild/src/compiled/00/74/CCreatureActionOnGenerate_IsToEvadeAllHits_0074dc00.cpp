// Leaf: ?IsToEvadeAllHits@CCreatureAction_OnGenerate@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_ONGENERATE_RTTI_DECLS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCREATUREACTION_ONGENERATE
#include "fable_string.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_OnGenerate.h"
bool CCreatureAction_OnGenerate::IsToEvadeAllHits() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_OnGenerate_IsToEvadeAllHits -> CCreatureAction_OnGenerate::IsToEvadeAllHits, the RTTI-placed PDB member; class model: engine/CCreatureAction_OnGenerate.h opt-in FABLE_RETAIL_CCREATUREACTION_ONGENERATE, FABLE_RETAIL_CCREATUREACTION_ONGENERATE_RTTI_DECLS (preamble of CGameScriptInterface_CreateCreature_008a9100.cpp).
