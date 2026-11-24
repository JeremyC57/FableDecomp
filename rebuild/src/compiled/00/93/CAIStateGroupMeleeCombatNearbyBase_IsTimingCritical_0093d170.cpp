// Leaf: ?IsTimingCritical@CAIStateGroup_MeleeCombatNearbyBase@@UBE_NXZ
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBY_MOVE
#define FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBYATTACKWHENVERYCLOSE
#define FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBYATTACKWHENVERYCLOSE_RTTI_DECLS
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_MeleeCombatNearbyBase.h"
bool CAIStateGroup_MeleeCombatNearbyBase::IsTimingCritical() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CAIStateGroup_MeleeCombatNearbyBase_IsTimingCritical -> CAIStateGroup_MeleeCombatNearbyBase::IsTimingCritical, the RTTI-placed PDB member; class model: engine/CAIStateGroup_MeleeCombatNearbyBase.h under FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBY_MOVE (preamble of CAIStateGroupMeleeCombatNearbyAttackWhenVeryClose_GetStateGroupType_008ff9d2.cpp).
