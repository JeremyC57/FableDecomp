// Leaf: ?IsCombatStateGroup@CAIStateGroup_HoverAttack@@UBE_NXZ
#define FABLE_RETAIL_CAISTATEGROUP_HOVERATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS
#include "engine/CAIStateGroup_HoverAttack.h"
bool CAIStateGroup_HoverAttack::IsCombatStateGroup() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CAIStateGroup_HoverAttack_IsCombatStateGroup -> CAIStateGroup_HoverAttack::IsCombatStateGroup, the RTTI-placed PDB member; class model: engine/CAIStateGroup_HoverAttack.h under FABLE_RETAIL_CAISTATEGROUP_HOVERATTACK_VIRTUAL_MODEL (preamble of CAIStateGroupHoverAttack_GetStateGroupType_00900c50.cpp).
