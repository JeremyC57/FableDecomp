// Leaf: ?IsToPerformActionOnStrike@CCreatureAction_BanditKingThrowWeapons@@UBE_NXZ
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCREATUREACTION_BANDITKINGTHROWWEAPONS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CThingAICreature.h"
#include "engine/CCreatureAction_BanditKingThrowWeapons.h"
bool CCreatureAction_BanditKingThrowWeapons::IsToPerformActionOnStrike() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_BanditKingThrowWeapons_IsToPerformActionOnStrike -> CCreatureAction_BanditKingThrowWeapons::IsToPerformActionOnStrike, the RTTI-placed PDB member; class model: engine/CCreatureAction_BanditKingThrowWeapons.h under FABLE_RETAIL_CCREATUREACTION_BANDITKINGTHROWWEAPONS (preamble of CAIStateGroupBanditKingThrowWeapons_GetStateGroupType_009012e6.cpp).
