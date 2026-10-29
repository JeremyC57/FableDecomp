#include "engine/CAIStateGroup_MinionReactToNoise.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MinionReactToNoise;
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
void __fastcall OnReplace(CAIStateGroup_MinionReactToNoise* stateGroup)
{
    stateGroup->FrameOfLastInteraction = CWorld::GetFrame();
}