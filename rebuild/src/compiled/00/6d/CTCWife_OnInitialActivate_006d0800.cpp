// CTCWife::OnInitialActivate() -- on activation re-initialise the non-persistent wife state (InitWifeNonpersistentState inlined)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D0800, length 38 bytes (+ int3 padding).
// Label correction: the manifest repeats "CTCWife::InitWifeNonpersistentState" (that body is 0x006D03E0); this copy is
// CTCWife vtable 0x0125ED5C slot 6 (rtti-vtables.tsv), the CTCBase OnInitialActivate slot (Ego_r
// ?OnInitialActivate@CTCWife@@UAEXXZ), with InitWifeNonpersistentState inlined byte-for-byte.
// Callees: GetWorldFrame() @ 0x0049D870, CTCWife::SetAttractionIcon(bool, float, unsigned char) @ 0x006CFC90.
// Layout: engine/CTCWife.h FrameGotMarriedToThePlayer +0x20, NeedsToChangeBrain +0x26.
// Shape: as 0x006D03E0 -- `wedding frame <= now`, the three pushes, the flag store last.
#include "engine/CTCWife.h"

long GetWorldFrame();                                       // 0x0049D870

struct CTCWife_State : CTCWife {
    bool IsMarriedToThePlayer() const { return FrameGotMarriedToThePlayer <= GetWorldFrame(); }
    void SetAttractionIcon(bool show, float scale, unsigned char alpha);   // 0x006CFC90
    void InitWifeNonpersistentState()
    {
        if (IsMarriedToThePlayer()) {
            SetAttractionIcon(true, 0.5f, 255);
            NeedsToChangeBrain = true;
        }
    }
    void OnInitialActivate();
};

void CTCWife_State::OnInitialActivate()
{
    InitWifeNonpersistentState();
}