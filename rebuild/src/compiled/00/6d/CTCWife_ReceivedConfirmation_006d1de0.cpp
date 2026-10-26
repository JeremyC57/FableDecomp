// CTCWife::ReceivedConfirmation(bool) -- on a confirmed prompt: an unmarried wife willing to marry marries the hero; a married wife offering sex has it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D1DE0, length 73 bytes (+ int3 padding).
// Callees: GetWorldFrame() @ 0x0049D870 (twice, through the inline IsMarriedToThePlayer; manifest
// "GFGetPreMainMemoryUsed", propagated), CTCWife::IsWillingToMarry() const @ 0x006CFEC0, CTCWife::Marry(bool) @ 0x006D18A0,
// CTCWife::IsOfferingSex() const @ 0x006CEEA0, CTCWife::HaveSex() @ 0x006CF920 (manifest labels). Called by 0x0092F188.
// Ego_r.pdb: ?ReceivedConfirmation@CTCWife@@QAEX_N@Z. Layout: engine/CTCWife.h FrameGotMarriedToThePlayer +0x20.
// Shape: the marriage test is re-evaluated after the Marry branch (the frame fetched again), each step a nested if.
#define CTCWife CTCWifeLayout
#include "engine/CTCWife.h"
#undef CTCWife

class CTCWife : public CTCWifeLayout
{
public:
    bool IsOfferingSex() const;   // 0x006CEEA0
    bool IsWillingToMarry() const;   // 0x006CFEC0
};

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                                       // 0x0049D870

struct CTCWife_Confirm : CTCWife {
    bool IsMarriedToThePlayer() const { return FrameGotMarriedToThePlayer <= CWorld::GetFrame(); }
    void Marry(bool fromScript);                            // 0x006D18A0
    void HaveSex();                                         // 0x006CF920
    void ReceivedConfirmation(bool confirmed);
};

void CTCWife_Confirm::ReceivedConfirmation(bool confirmed)
{
    if (confirmed) {
        if (!IsMarriedToThePlayer()) {
            if (IsWillingToMarry())
                Marry(false);
        }
        if (IsMarriedToThePlayer()) {
            if (IsOfferingSex())
                HaveSex();
        }
    }
}