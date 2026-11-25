// Leaf: ?GetCombatMinDistance@CAIStateGroup_EnemyCreatureBase@@MBEMXZ
// CAIStateGroup_EnemyCreatureBase::GetCombatMinDistance() const -- returns 1.5f (retail loads the pooled float literal at 0x0125AADC).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + Ego_r/FableWin name set; the landed stand-in
//   _CAIStateGroup_EnemyCreatureBase_GetCombatMinDistance becomes the virtual member of the polymorphic class in
//   engine/CAIStateGroup_EnemyCreatureBase.h (model FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL (generated here);
//   defines FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL, FABLE_RETAIL_CAISTATEGROUPBASE,
//   FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS). NF429 G1 (CONST shape).
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_EnemyCreatureBase.h"
float CAIStateGroup_EnemyCreatureBase::GetCombatMinDistance() const
{
    return 1.5f;
}
