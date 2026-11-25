// Leaf: ?IsCombatStateGroup@CAIStateGroup_CreatureFindCover@@UBE_NXZ
// CAIStateGroup_CreatureFindCover::IsCombatStateGroup() const -- returns true.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   ?CAIStateGroup_CreatureFindCover_IsCombatStateGroup@@YI_NXZ becomes the virtual member of the polymorphic class in
//   engine/CAIStateGroup_CreatureFindCover.h (model FABLE_RETAIL_CAISTATEGROUP_CREATUREFINDCOVER_VIRTUAL_MODEL (generated here);
//   defines FABLE_RETAIL_CAISTATEGROUP_CREATUREFINDCOVER_VIRTUAL_MODEL, FABLE_RETAIL_CAISTATEGROUPBASE,
//   FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS, FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL,
//   FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_VIRTUAL_MODEL). NF429 G1 (CONST shape).
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREFINDCOVER_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_CreatureFindCover.h"
bool CAIStateGroup_CreatureFindCover::IsCombatStateGroup() const
{
    return true;
}
