// NUISystem::CIME::SetEditBoxCaretPos(long x, long y) -- forward the edit box caret position to the IME UI when the IME is initialised
// Leaf: ?SetEditBoxCaretPos@CIME@NUISystem@@QAEXJJ@Z
// Identity (naming wave 11): Ego_r.pdb + FableWin.pdb ?SetEditBoxCaretPos@CIME@NUISystem@@QAEXJJ@Z (NUISystem::CIME members in retail = PDB order: Get 0x00404A80, FinaliseString 0x00404B50, SetEditBoxCaretPos 0x00404B80, ShouldEditBoxShowCaret 0x00404BA0, SetEditBoxRegion 0x00404BB0, Render 0x00404C00, IsIMEActive 0x00404C50).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404B80, 23 bytes. Callees: IME UI helper (a, b).
//
// Guarded pass-through: every CIME entry point checks IsInitialised first so the whole IME layer is a
// no-op on machines where it was never set up (no IME installed, or the option off). When it is, the two
// arguments go straight to ImeUi_SetCaretPosition (the propagated FinaliseString label was 0x00404B50's).
//
// Matching: `this` in ecx, the two arguments on the stack are reloaded into ecx/edx for the __fastcall
// callee; `ret 8`.
namespace NUISystem {
class CIME {
public:
    unsigned char _pad00[0x8];
    bool          IsInitialised;                 // +0x08
    void SetEditBoxCaretPos(long x, long y);
};
}

void __fastcall ImeUi_SetCaretPosition(unsigned int x, unsigned int y);   // 0x00CB2860 ?ImeUi_SetCaretPosition@@YIXII@Z (Ego_r)

void NUISystem::CIME::SetEditBoxCaretPos(long x, long y)
{
    if (IsInitialised)
        ImeUi_SetCaretPosition(x, y);
}
