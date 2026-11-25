// Leaf: ?OnReplace@CAIStateGroup_MinionListenToStory@@MAEXXZ
// CAIStateGroup_MinionListenToStory::OnReplace() -- forwards to CAIStateGroup_MinionListenToStory::StandUp (0x00910570; retail tail-jumps to it).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + Ego_r/FableWin name set; the landed stand-in
//   _CAIStateGroup_MinionListenToStory_OnReplace becomes the virtual member of the polymorphic class in
//   engine/CAIStateGroup_MinionListenToStory.h (model FABLE_RETAIL_CAISTATEGROUP_MINIONLISTENTOSTORY_VIRTUAL_MODEL (generated here);
//   defines FABLE_RETAIL_CAISTATEGROUP_MINIONLISTENTOSTORY_VIRTUAL_MODEL, FABLE_RETAIL_CAISTATEGROUPBASE,
//   FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS). NF429 G2b (JMP to an own non-virtual member); callee: the target's landed self, in the
//   Ego_r and FableWin name sets, declared by the model.
#define FABLE_RETAIL_CAISTATEGROUP_MINIONLISTENTOSTORY_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_MinionListenToStory.h"
void CAIStateGroup_MinionListenToStory::OnReplace()
{
    StandUp();
}
