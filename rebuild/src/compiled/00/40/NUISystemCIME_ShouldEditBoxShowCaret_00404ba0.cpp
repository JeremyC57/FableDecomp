// NUISystem::CIME::ShouldEditBoxShowCaret() -- with the IME up, ask the IME UI whether the caret shows; otherwise answer true
// Leaf: ?ShouldEditBoxShowCaret@CIME@NUISystem@@QAE_NXZ
// Identity (naming wave 11): Ego_r.pdb + FableWin.pdb ?ShouldEditBoxShowCaret@CIME@NUISystem@@QAE_NXZ (the CNetworkClient label was propagated) (NUISystem::CIME members in retail = PDB order: Get 0x00404A80, FinaliseString 0x00404B50, SetEditBoxCaretPos 0x00404B80, ShouldEditBoxShowCaret 0x00404BA0, SetEditBoxRegion 0x00404BB0, Render 0x00404C00, IsIMEActive 0x00404C50).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404BA0, 15 bytes. Callees: the delegate query (same arguments).
//
// A guarded forwarder: IsInitialised (+8) gates the ImeUi query; without the IME the edit box always shows its caret.
// Caller: CEditBox::UpdateDisplayText 0x00540980 (draws the caret and sets SetEditBoxCaretPos).
//
// Matching: `cmp byte ptr [ecx+8],0; je` then a tail `jmp` to the delegate with ecx/edx untouched;
// the true path is `mov al,1; ret`.
namespace NUISystem {
class CIME {
public:
    unsigned char _pad00[0x8];
    bool          IsInitialised;                 // +0x08
    bool ShouldEditBoxShowCaret();
};
}

bool __fastcall ImeUi_GetCaretStatus();          // 0x00CB2EC0 ?ImeUi_GetCaretStatus@@YI_NXZ (Ego_r)

bool NUISystem::CIME::ShouldEditBoxShowCaret()
{
    if (IsInitialised)
        return ImeUi_GetCaretStatus();
    return true;
}
