// Leaf: ?IsToEvadeAllHits@CCreatureAction_UndeadReturnToEarth@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_UNDEADRETURNTOEARTH_RTTI_DECLS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_UNDEADRETURNTOEARTH_CLASS
#include "engine/CThingAICreature.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_UndeadReturnToEarth.h"
bool CCreatureAction_UndeadReturnToEarth::IsToEvadeAllHits() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_UndeadReturnToEarth_IsToEvadeAllHits -> CCreatureAction_UndeadReturnToEarth::IsToEvadeAllHits, the RTTI-placed PDB member; class model: engine/CCreatureAction_UndeadReturnToEarth.h opt-in FABLE_RETAIL_CCREATUREACTION_UNDEADRETURNTOEARTH_CLASS, FABLE_RETAIL_CCREATUREACTION_UNDEADRETURNTOEARTH_RTTI_DECLS (preamble of CAIStateGroupCreatureGoUnderground_Update_00920730.cpp).
