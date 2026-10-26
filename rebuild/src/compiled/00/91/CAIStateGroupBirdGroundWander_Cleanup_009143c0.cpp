#include "engine/CAIStateGroup_BirdGroundWander.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall h1_9143c0(CAIStateGroup_BirdGroundWander* stateGroup);
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};   // 0x0049D870
void __fastcall fn_009143c0(CAIStateGroup_BirdGroundWander* stateGroup){ h1_9143c0(stateGroup); stateGroup->FrameLastFinishedWalking = CWorld::GetFrame(); }