// NUISystem::CIME::FinaliseString(bool finalise) -- tell the IME UI to commit or cancel the composition, honouring CancelOnOKClick
// Leaf: ?FinaliseString@CIME@NUISystem@@QAEX_N@Z
// Identity (naming wave 11): Ego_r.pdb + FableWin.pdb ?FinaliseString@CIME@NUISystem@@QAEX_N@Z (NUISystem::CIME members in retail = PDB order: Get 0x00404A80, FinaliseString 0x00404B50, SetEditBoxCaretPos 0x00404B80, ShouldEditBoxShowCaret 0x00404BA0, SetEditBoxRegion 0x00404BB0, Render 0x00404C00, IsIMEActive 0x00404C50).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404B50, 37 bytes. Callees: IME UI helper (bool).
//
// CIME wraps the Windows Input Method Editor so players using Japanese/Chinese/Korean keyboards can
// compose text in our edit boxes (hero name, save names). Nothing happens unless the IME was set up at
// start-up. The argument asks for a "finalise" (true) or plain close (false); when the profile option
// CancelOnOKClick is set we turn a finalise into a cancel, because on those systems pressing OK should
// discard the half-composed string rather than commit it. The resulting flag is passed to the UI
// helper that actually ends the composition.
//
// Matching: the argument is copied into a local that the inner branch overwrites with 0, which is what
// gives the `mov al,param; test; jz; cmp flag50; jz; xor al,al` shape; the helper takes the bool in cl.
namespace NUISystem {
class CIME {
public:
    unsigned char _pad00[0x8];
    bool          IsInitialised;                 // +0x08 the IME was set up at start-up
    unsigned char _pad09[0x47];                  // WindowHandle, Appearance, PEditBox
    bool          CancelOnOKClick;               // +0x50 profile option
    void FinaliseString(bool finalise);
};
}

void __fastcall ImeUi_FinalizeString(bool send);   // 0x00CB2C60 ?ImeUi_FinalizeString@@YIX_N@Z (Ego_r)

void NUISystem::CIME::FinaliseString(bool finalise)
{
    if (IsInitialised) {
        char v = finalise;
        // Finalise becomes cancel when the player asked for OK to cancel.
        if (finalise) {
            if (CancelOnOKClick)
                v = 0;
        }
        ImeUi_FinalizeString(v != 0);
    }
}
