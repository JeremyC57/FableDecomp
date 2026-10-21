// CPlayerGui::ForceOpen() -- clears the forced-closed state (and the flag at +0x2bd) and puts the GUI into its opening mode
// Leaf: ForceOpen@CPlayerGui
// Retail: Fable.exe @ 0x004374D0, 25 bytes; Ego_r.pdb ?ForceOpen@CPlayerGui@@QAEXXZ. Only caller 0x0089205E (CGameScriptInterface).
// Layout: as in CPlayerGui::Open 0x004374B0: GuiMode +0x294, ForcedClosed +0x29c (Ego_r.pdb +0x2a8/+0x2b0). The bool at +0x2bd
// has no confident Ego_r.pdb match (the -0x14 shift lands inside the Illustrators vector), so it keeps a placeholder name.
enum EPlayerGuiMode { PLAYER_GUI_MODE_OPENING = 2, PLAYER_GUI_MODE_FORCE_DWORD = 0x7fffffff };
class CPlayerGui {
public:
    unsigned char  _pad00[0x294];
    EPlayerGuiMode GuiMode;                              // +0x294
    float          FadeInTimer;                          // +0x298
    bool           ForcedClosed;                         // +0x29c
    unsigned char  _pad29d[0x2bd - 0x29d];
    bool           Flag2BD;                              // +0x2bd
    void ForceOpen();
};

void CPlayerGui::ForceOpen()
{
    ForcedClosed = false;
    Flag2BD = false;
    GuiMode = PLAYER_GUI_MODE_OPENING;
}