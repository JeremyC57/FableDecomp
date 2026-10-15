// CTCWife::InitWifeNonpersistentState() -- for a wife married to the player: show the attraction icon (half scale, opaque) and ask for the wife brain
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D03E0, length 38 bytes (+ int3 padding).
// Callees: GetWorldFrame() @ 0x0049D870 (manifest "GFGetPreMainMemoryUsed", propagated), CTCWife::SetAttractionIcon(bool,
// float, unsigned char) @ 0x006CFC90 (manifest label; Ego_r ?SetAttractionIcon@CTCWife@@AAEX_NME@Z). Called by 0x006D090F.
// Ego_r.pdb: ?InitWifeNonpersistentState@CTCWife@@QAEXXZ. Layout: engine/CTCWife.h FrameGotMarriedToThePlayer +0x20,
// NeedsToChangeBrain +0x26.
// Shape: the inline IsMarriedToThePlayer (`wedding frame <= now`, `cmp [esi+20h],eax ; jg`), the three pushes
// (`push 0FFh ; push 3F000000h ; push 1`), the flag store last.
#include "engine/CTCWife.h"

long GetWorldFrame();                                       // 0x0049D870

struct CTCWife_State : CTCWife {
    bool IsMarriedToThePlayer() const { return FrameGotMarriedToThePlayer <= GetWorldFrame(); }
    void SetAttractionIcon(bool show, float scale, unsigned char alpha);   // 0x006CFC90
    void InitWifeNonpersistentState();
};

void CTCWife_State::InitWifeNonpersistentState()
{
    if (IsMarriedToThePlayer()) {
        SetAttractionIcon(true, 0.5f, 255);
        NeedsToChangeBrain = true;
    }
}
