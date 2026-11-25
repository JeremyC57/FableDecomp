// Leaf: ?IsRunnable@CAIStateGroup_MinionHasInformation@@UAE_NXZ
// CAIStateGroup_MinionHasInformation::IsRunnable() -- forwards to CAIStateGroup_MinionHasInformation::CreatureWantsPlayersAttention (0x0090F290; retail tail-jumps to it).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + Ego_r/FableWin name set; the landed stand-in
//   _CAIStateGroup_MinionHasInformation_IsRunnable becomes the virtual member of the polymorphic class in
//   engine/CAIStateGroup_MinionHasInformation.h (model FABLE_RETAIL_CAISTATEGROUP_MINIONHASINFORMATION_VIRTUAL_MODEL (generated here);
//   defines FABLE_RETAIL_CAISTATEGROUP_MINIONHASINFORMATION_VIRTUAL_MODEL, FABLE_RETAIL_CAISTATEGROUPBASE,
//   FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS). NF429 G2b (JMP to an own non-virtual member); callee: the target's landed self, in the
//   Ego_r and FableWin name sets, declared by the model.
#define FABLE_RETAIL_CAISTATEGROUP_MINIONHASINFORMATION_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_MinionHasInformation.h"
bool CAIStateGroup_MinionHasInformation::IsRunnable()
{
    return CreatureWantsPlayersAttention();
}
