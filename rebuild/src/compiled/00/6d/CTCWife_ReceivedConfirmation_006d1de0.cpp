// CTCWife::ReceivedConfirmation(bool) -- on a confirmed prompt: an unmarried wife willing to marry marries the hero; a married wife offering sex has it
// Leaf: ?ReceivedConfirmation@CTCWife@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D1DE0, length 73 bytes (+ int3 padding).
// Callees: GetWorldFrame() @ 0x0049D870 (twice, through the inline IsMarriedToThePlayer; manifest
// "GFGetPreMainMemoryUsed", propagated), CTCWife::IsWillingToMarry() const @ 0x006CFEC0, CTCWife::Marry(bool) @ 0x006D18A0,
// CTCWife::IsOfferingSex() const @ 0x006CEEA0, CTCWife::HaveSex() @ 0x006CF920 (manifest labels). Called by 0x0092F188.
// Ego_r.pdb: ?ReceivedConfirmation@CTCWife@@QAEX_N@Z. Layout: engine/CTCWife.h FrameGotMarriedToThePlayer +0x20.
// Shape: the marriage test is re-evaluated after the Marry branch (the frame fetched again), each step a nested if.
#define CTCWife CTCWifeLayout
#include "engine/CTCWife.h"
#undef CTCWife

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x006D18A0 ?Marry@CTCWife@@QAEX_N@Z (was ?Marry@CTCWife_Confirm@@QAEX_N@Z)
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                                       // 0x0049D870
class CTCWife : public CTCWifeLayout
{
public:
    bool IsOfferingSex() const;   // 0x006CEEA0
    bool IsWillingToMarry() const;   void HaveSex();   // 0x006CFEC0; HaveSex 0x006CF920 (Ego_r + FableWin ?HaveSex@CTCWife@@QAEXXZ; NF334: moved here from CTCWife_Confirm)
 public: void Marry(bool);
    // NF479 (Codex pass 29 name fixes): the self is ?ReceivedConfirmation@CTCWife@@QAEX_N@Z (Ego_r 0x00707290 / FableWin 0x0248DBE0 publics),
    //   a member of CTCWife itself; was the stand-in CTCWife_Confirm. IsMarriedToThePlayer is inlined here (out of line at 0x006CEC80);
    //   its text is the landed 0x006CEC80 / 0x006D1E30 `? true : false` form, so the pick-any copy this TU emits is the 16-byte retail body (EQ).
    bool IsMarriedToThePlayer() const { return FrameGotMarriedToThePlayer <= CWorld::GetFrame() ? true : false; }
    void ReceivedConfirmation(bool confirmed);
};



void CTCWife::ReceivedConfirmation(bool confirmed)
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