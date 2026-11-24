// Leaf: ?IsToEvadeAllHits@CCreatureAction_GoUnderground@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_GOUNDERGROUND_RTTI_DECLS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_GOUNDERGROUND_CLASS
#include "engine/CThingAICreature.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_GoUnderground.h"
bool CCreatureAction_GoUnderground::IsToEvadeAllHits() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_GoUnderground_IsToEvadeAllHits -> CCreatureAction_GoUnderground::IsToEvadeAllHits, the RTTI-placed PDB member; class model: engine/CCreatureAction_GoUnderground.h opt-in FABLE_RETAIL_CCREATUREACTION_GOUNDERGROUND_CLASS, FABLE_RETAIL_CCREATUREACTION_GOUNDERGROUND_RTTI_DECLS (preamble of CAIStateGroupCreatureIdleUnderground_OnActivate_009208e0.cpp).
