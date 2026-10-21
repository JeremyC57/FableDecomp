// CPlayerGui::ForceClose() -- sets the forced-closed state (and the flag at +0x2bd), puts the GUI into mode 3, then clears it
// Leaf: ForceClose@CPlayerGui
// Retail: Fable.exe @ 0x00438440, 29 bytes; Ego_r.pdb ?ForceClose@CPlayerGui@@QAEXXZ; mirror of ForceOpen 0x004374D0.
// Only caller 0x00892066 (CGameScriptInterface). Tail-jumps to CPlayerGui::ClearGui (0x00438230).
// Layout: as ForceOpen 0x004374D0: GuiMode +0x294, ForcedClosed +0x29c (Ego_r.pdb +0x2a8/+0x2b0); +0x2bd placeholder name.
enum EPlayerGuiMode { PLAYER_GUI_MODE_OPENING = 2, PLAYER_GUI_MODE_CLOSED = 3, PLAYER_GUI_MODE_FORCE_DWORD = 0x7fffffff };
class CPlayerGui {
public:
    unsigned char  _pad00[0x294];
    EPlayerGuiMode GuiMode;                              // +0x294
    float          FadeInTimer;                          // +0x298
    bool           ForcedClosed;                         // +0x29c
    unsigned char  _pad29d[0x2bd - 0x29d];
    bool           Flag2BD;                              // +0x2bd
    void ClearGui();                                     // 0x00438230
    void ForceClose();
};

void CPlayerGui::ForceClose()
{
    ForcedClosed = true;
    Flag2BD = true;
    GuiMode = PLAYER_GUI_MODE_CLOSED;
    ClearGui();
}