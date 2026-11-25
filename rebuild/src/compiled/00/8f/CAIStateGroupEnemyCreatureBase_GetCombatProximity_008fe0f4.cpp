// Leaf: ?GetCombatProximity@CAIStateGroup_EnemyCreatureBase@@MBEMXZ
// CAIStateGroup_EnemyCreatureBase::GetCombatProximity() const -- returns 0.0f (retail loads the pooled float literal at 0x0122DEDC).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   ?CAIStateGroup_EnemyCreatureBase_GetCombatProximity@@YAMXZ becomes the virtual member of the polymorphic class in
//   engine/CAIStateGroup_EnemyCreatureBase.h (model FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL (generated here);
//   defines FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL, FABLE_RETAIL_CAISTATEGROUPBASE,
//   FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS). NF429 G1 (CONST shape).
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_EnemyCreatureBase.h"
float CAIStateGroup_EnemyCreatureBase::GetCombatProximity() const
{
    return 0.0f;
}
