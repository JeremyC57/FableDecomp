// CTCWife::OnInitialActivate() -- on activation re-initialise the non-persistent wife state (InitWifeNonpersistentState inlined)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D0800, length 38 bytes (+ int3 padding).
// Label correction: the manifest repeats "CTCWife::InitWifeNonpersistentState" (that body is 0x006D03E0); this copy is
// CTCWife vtable 0x0125ED5C slot 6 (rtti-vtables.tsv), the CTCBase OnInitialActivate slot (Ego_r
// ?OnInitialActivate@CTCWife@@UAEXXZ), with InitWifeNonpersistentState inlined byte-for-byte.
// Callees: GetWorldFrame() @ 0x0049D870, CTCWife::SetAttractionIcon(bool, float, unsigned char) @ 0x006CFC90.
// Layout: engine/CTCWife.h FrameGotMarriedToThePlayer +0x20, NeedsToChangeBrain +0x26.
// Shape: as 0x006D03E0 -- `wedding frame <= now`, the three pushes, the flag store last.
// Name fix (naming agent NF21, 2026-11-11): was the derived stand-in CTCWife_State, which mangled the private
//   ?SetAttractionIcon@CTCWife@@AAEX_NME@Z (Ego_r.pdb + FableWin.pdb, 0x006CFC90) as CTCWife_State::SetAttractionIcon; now the
//   retail engine/CTCWife.h (FABLE_RETAIL_CTCWIFE) member, with IsMarriedToThePlayer written inline as retail inlines it here.
// Leaf: ?OnInitialActivate@CTCWife@@UAEXXZ
#define FABLE_RETAIL_CTCWIFE
#include "engine/CTCWife.h"

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                                       // 0x0049D870

void CTCWife::OnInitialActivate()
{
    // InitWifeNonpersistentState() inlined
    if (FrameGotMarriedToThePlayer <= CWorld::GetFrame()) {   // the inline IsMarriedToThePlayer
        SetAttractionIcon(true, 0.5f, 255);
        NeedsToChangeBrain = true;
    }
}
