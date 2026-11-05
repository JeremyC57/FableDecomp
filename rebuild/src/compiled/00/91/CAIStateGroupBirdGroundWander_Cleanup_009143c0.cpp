// Leaf: ?CAIStateGroup_BirdGroundWander_Cleanup@@YIXPAUCAIStateGroup_BirdGroundWander@@@Z
#include "engine/CAIStateGroup_BirdGroundWander.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall h1_9143c0(CAIStateGroup_BirdGroundWander* stateGroup);
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};   // 0x0049D870
void __fastcall CAIStateGroup_BirdGroundWander_Cleanup(CAIStateGroup_BirdGroundWander* stateGroup){ h1_9143c0(stateGroup); stateGroup->FrameLastFinishedWalking = CWorld::GetFrame(); }
// Stand-in (owner rule 2026-11-05): CAIStateGroup_BirdGroundWander_Cleanup for CAIStateGroup_BirdGroundWander::Cleanup; RTTI vtable slot ?Cleanup@CAIStateGroup_BirdGroundWander@@UAEXXZ + PDB name set (egor/fablewin).
