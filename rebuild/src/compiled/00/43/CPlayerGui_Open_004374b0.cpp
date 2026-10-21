// CPlayerGui::Open() -- puts the player GUI into its opening mode unless it has been forced closed
// Leaf: Open@CPlayerGui
// Retail: Fable.exe @ 0x004374B0, 21 bytes; Ego_r.pdb ?Open@CPlayerGui@@QAEXXZ. Called from many places (0x0047D8F2, 0x0055D4F6, ...).
// Layout: retail CPlayerGui is 0x14 below Ego_r.pdb here: GuiMode +0x294, ForcedClosed +0x29c (PDB +0x2a8/+0x2b0); engine/CPlayerGui.h
// carries the PDB offsets. The mode value is the PDB EPlayerGuiMode enumerator 2 (name ours).
enum EPlayerGuiMode { PLAYER_GUI_MODE_OPENING = 2, PLAYER_GUI_MODE_FORCE_DWORD = 0x7fffffff };
class CPlayerGui {
public:
    unsigned char  _pad00[0x294];
    EPlayerGuiMode GuiMode;                              // +0x294
    float          FadeInTimer;                          // +0x298
    bool           ForcedClosed;                         // +0x29c
    void Open();
};

void CPlayerGui::Open()
{
    if (!ForcedClosed)
        GuiMode = PLAYER_GUI_MODE_OPENING;
}