// Leaf: ?GetStateGroupType@CAIStateGroup_MinionFollowThing@@MBE?AW4EAIStateGroupType@@XZ
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_MINIONFOLLOWTHING
#include "engine/CAIStateGroup_MinionFollowThing.h"
#pragma optimize("s",on)
EAIStateGroupType CAIStateGroup_MinionFollowThing::GetStateGroupType() const
{
    return (EAIStateGroupType)2;
}
// Flatten route (flatten_pilot v4, NF324): CAIStateGroup_MinionFollowThing_GetStateGroupType -> CAIStateGroup_MinionFollowThing::GetStateGroupType, the RTTI-placed PDB member; class model: engine/CAIStateGroup_MinionFollowThing.h under FABLE_RETAIL_CAISTATEGROUP_MINIONFOLLOWTHING (preamble of CAIStateGroupMinionFollowThing_IsExpressionPerfomedTo_00921440.cpp).
