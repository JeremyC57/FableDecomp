// Leaf: ?IsCombatStateGroup@CAIStateGroup_HoverAttackArea@@UBE_NXZ
#define FABLE_RETAIL_CAISTATEGROUP_HOVERATTACKAREA_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_HOVERATTACK_VIRTUAL_MODEL
#include "engine/CAIStateGroup_HoverAttackArea.h"
bool CAIStateGroup_HoverAttackArea::IsCombatStateGroup() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CAIStateGroup_HoverAttackArea_IsCombatStateGroup -> CAIStateGroup_HoverAttackArea::IsCombatStateGroup, the RTTI-placed PDB member; class model: engine/CAIStateGroup_HoverAttackArea.h under FABLE_RETAIL_CAISTATEGROUP_HOVERATTACKAREA_VIRTUAL_MODEL (preamble of CAIStateGroupHoverAttackArea_GetStateGroupType_00900d3d.cpp).
