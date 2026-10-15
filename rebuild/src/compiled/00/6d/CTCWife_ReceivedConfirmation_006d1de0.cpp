// CTCWife::ReceivedConfirmation(bool) -- on a confirmed prompt: an unmarried wife willing to marry marries the hero; a married wife offering sex has it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D1DE0, length 73 bytes (+ int3 padding).
// Callees: GetWorldFrame() @ 0x0049D870 (twice, through the inline IsMarriedToThePlayer; manifest
// "GFGetPreMainMemoryUsed", propagated), CTCWife::IsWillingToMarry() const @ 0x006CFEC0, CTCWife::Marry(bool) @ 0x006D18A0,
// CTCWife::IsOfferingSex() const @ 0x006CEEA0, CTCWife::HaveSex() @ 0x006CF920 (manifest labels). Called by 0x0092F188.
// Ego_r.pdb: ?ReceivedConfirmation@CTCWife@@QAEX_N@Z. Layout: engine/CTCWife.h FrameGotMarriedToThePlayer +0x20.
// Shape: the marriage test is re-evaluated after the Marry branch (the frame fetched again), each step a nested if.
#include "engine/CTCWife.h"

long GetWorldFrame();                                       // 0x0049D870

struct CTCWife_Confirm : CTCWife {
    bool IsMarriedToThePlayer() const { return FrameGotMarriedToThePlayer <= GetWorldFrame(); }
    bool IsWillingToMarry() const;                          // 0x006CFEC0
    void Marry(bool fromScript);                            // 0x006D18A0
    bool IsOfferingSex() const;                             // 0x006CEEA0
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