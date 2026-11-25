// Leaf: ?GetStateGroupType@CAIStateGroup_RockTroll@@MBE?AW4EAIStateGroupType@@XZ
// CAIStateGroup_RockTroll::GetStateGroupType() const -- returns (EAIStateGroupType)1.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + FableWin name set; the landed stand-in
//   ?GetStateGroupType@CAIStateGroup_RockTroll_Methods@@QAEHXZ becomes the virtual member of the polymorphic class in
//   engine/CAIStateGroup_RockTroll.h (model FABLE_RETAIL_CAISTATEGROUP_ROCKTROLL_VIRTUAL_MODEL (generated here); defines
//   FABLE_RETAIL_CAISTATEGROUP_ROCKTROLL_VIRTUAL_MODEL, FABLE_RETAIL_CAISTATEGROUPBASE, FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS,
//   FABLE_RETAIL_CAISTATEGROUP_TROLLBASE_VIRTUAL_MODEL). NF429 G1 (CONST shape).
#pragma optimize("s",on)
#define FABLE_RETAIL_CAISTATEGROUP_ROCKTROLL_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_TROLLBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_RockTroll.h"
EAIStateGroupType CAIStateGroup_RockTroll::GetStateGroupType() const
{
    return (EAIStateGroupType)1;
}
