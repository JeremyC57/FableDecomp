// CTCWife::InitWifeNonpersistentState() -- for a wife married to the player: show the attraction icon (half scale, opaque) and ask for the wife brain
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D03E0, length 38 bytes (+ int3 padding).
// Callees: GetWorldFrame() @ 0x0049D870 (manifest "GFGetPreMainMemoryUsed", propagated), CTCWife::SetAttractionIcon(bool,
// float, unsigned char) @ 0x006CFC90 (manifest label; Ego_r ?SetAttractionIcon@CTCWife@@AAEX_NME@Z). Called by 0x006D090F.
// Ego_r.pdb: ?InitWifeNonpersistentState@CTCWife@@QAEXXZ. Layout: engine/CTCWife.h FrameGotMarriedToThePlayer +0x20,
// NeedsToChangeBrain +0x26.
// Shape: the inline IsMarriedToThePlayer (`wedding frame <= now`, `cmp [esi+20h],eax ; jg`), the three pushes
// (`push 0FFh ; push 3F000000h ; push 1`), the flag store last.
// Name fix (naming agent NF21, 2026-11-11): was the derived stand-in CTCWife_State, which mangled the private
//   ?SetAttractionIcon@CTCWife@@AAEX_NME@Z (Ego_r.pdb + FableWin.pdb, 0x006CFC90) as CTCWife_State::SetAttractionIcon; now the
//   retail engine/CTCWife.h (FABLE_RETAIL_CTCWIFE) member, with IsMarriedToThePlayer written inline as retail inlines it here.
// Leaf: ?InitWifeNonpersistentState@CTCWife@@QAEXXZ
#define FABLE_RETAIL_CTCWIFE
#include "engine/CTCWife.h"

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                                       // 0x0049D870

void CTCWife::InitWifeNonpersistentState()
{
    if (FrameGotMarriedToThePlayer <= CWorld::GetFrame()) {   // the inline IsMarriedToThePlayer
        SetAttractionIcon(true, 0.5f, 255);
        NeedsToChangeBrain = true;
    }
}
