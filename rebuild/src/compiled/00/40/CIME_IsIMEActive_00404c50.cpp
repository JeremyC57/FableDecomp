// NUISystem::CIME::IsIMEActive() const -- is the IME initialised and currently switched on by the player?
// Leaf: ?IsIMEActive@CIME@NUISystem@@QBE_NXZ
// Identity (naming wave 11): Ego_r.pdb + FableWin.pdb ?IsIMEActive@CIME@NUISystem@@QBE_NXZ (NUISystem::CIME members in retail = PDB order: Get 0x00404A80, FinaliseString 0x00404B50, SetEditBoxCaretPos 0x00404B80, ShouldEditBoxShowCaret 0x00404BA0, SetEditBoxRegion 0x00404BB0, Render 0x00404C00, IsIMEActive 0x00404C50).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00404C50, length 22 bytes.
//
// The edit boxes ask this to decide whether keystrokes go through composition or straight into the
// text. Two conditions: we set the IME up at start-up (IsInitialised), and the IME UI library reports
// that the player has it enabled right now (they can toggle it with the usual IME hot keys).
//
// Matching: the callee 0x00CB28E0 is ImeUi_IsEnabled (the label "CAIStateGroup_Freeze::IsRunnable" was propagated);
// `this` in ecx; the result is `mov al,1` / `xor al,al` arms.
namespace NUISystem {
class CIME {
public:
    unsigned char _pad00[0x8];
    bool          IsInitialised;                   // +0x08
    bool IsIMEActive() const;
};
}

bool __fastcall ImeUi_IsEnabled();                 // 0x00CB28E0 ?ImeUi_IsEnabled@@YI_NXZ (Ego_r; `mov al,[0x0143E890] ; ret`)

bool NUISystem::CIME::IsIMEActive() const
{
    if (IsInitialised && ImeUi_IsEnabled())
        return true;
    return false;
}
