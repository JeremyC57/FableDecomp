// CAIStateGroup_OpinionReactionRoot::FollowFarUpdate() -- the far follow reaction: FollowUpdate(7.0f, false)
// Leaf: ?FollowFarUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00985430, oracle length 13 bytes.
// Retail bytes:  push 0 ; push 40E00000h (7.0f) ; call 0x009852C0 ; ret
// Name fix (agentNF36 2026-11-16): was the generated stand-in CClass_00985430::CallWith2_00985430 calling Method(long, long).
// Ego_r.pdb + FableWin.pdb name sets hold ?FollowFarUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ (private bool); the Behaviours
// initializer 0x01221D20 stores 0x00985430 as Behaviours[36].UpdateFunc (OPINION_REACTION_TYPE_FOLLOW_FAR) and binds this name; Ego_r donor
// order FollowCloseUpdate 0x00985420 then FollowFarUpdate. The callee is the landed ?FollowUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NM_N@Z
// 0x009852C0. The second argument's name is descriptive.
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_BEHAVIOURS
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_FOLLOWUPDATE
#include "engine/CAIStateGroup_OpinionReactionRoot.h"

bool CAIStateGroup_OpinionReactionRoot::FollowFarUpdate()
{
    return FollowUpdate(7.0f, false);
}
