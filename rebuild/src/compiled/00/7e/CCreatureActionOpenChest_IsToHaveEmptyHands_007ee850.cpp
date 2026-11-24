// Leaf: ?IsToHaveEmptyHands@CCreatureAction_OpenChest@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_OPENCHEST_RTTI_DECLS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_OPENCHEST
#include "engine/CThing.h"
#include "engine/CCreatureAction_OpenChest.h"
bool CCreatureAction_OpenChest::IsToHaveEmptyHands() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_OpenChest_IsToHaveEmptyHands -> CCreatureAction_OpenChest::IsToHaveEmptyHands, the RTTI-placed PDB member; class model: engine/CCreatureAction_OpenChest.h opt-in FABLE_RETAIL_CCREATUREACTION_OPENCHEST, FABLE_RETAIL_CCREATUREACTION_OPENCHEST_RTTI_DECLS (preamble of CGameScriptInterface_OpenChest_00896390.cpp).
