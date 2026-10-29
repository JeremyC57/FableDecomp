#include "engine/CAIStateGroup_MinionKidGroupTag.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MinionKidGroupTag;
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
void __fastcall OnReplaceKidGroup(CAIStateGroup_MinionKidGroupTag* stateGroup)
{
    stateGroup->FrameLastPlayedIn = CWorld::GetFrame();
}