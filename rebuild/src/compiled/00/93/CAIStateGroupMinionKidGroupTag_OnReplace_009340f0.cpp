#include "engine/CAIStateGroup_MinionKidGroupTag.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MinionKidGroupTag;
extern int __fastcall re_call(CAIStateGroup_MinionKidGroupTag*);
void __fastcall OnReplaceKidGroup(CAIStateGroup_MinionKidGroupTag* stateGroup)
{
    stateGroup->FrameLastPlayedIn = re_call(stateGroup);
}