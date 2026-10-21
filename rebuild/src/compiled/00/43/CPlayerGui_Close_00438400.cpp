// CPlayerGui::Close(bool fade) -- start the GUI closing (fade from zero) or mark it closed outright, then clear it
// Leaf: ?Close@CPlayerGui@@QAEX_N@Z
// Retail: Fable.exe @ 0x00438400, 52 bytes, `ret 4`; Ego_r.pdb ?Close@CPlayerGui@@QAEX_N@Z; 7 callers (0x00483176,
// 0x0055D4B6, ...). fade: GuiMode = 1, FadeInTimer = 0.0f; else GuiMode = 3; both arms end in CPlayerGui::ClearGui
// (0x00438230, tail-duplicated). The EPlayerGuiMode enumerator names are not recovered.
// Layout: retail CPlayerGui -- GuiMode +0x294, FadeInTimer +0x298 (engine/CPlayerGui.h carries Ego_r.pdb +0x2a8/+0x2ac:
// 8 higher past SubtitleRenderer plus 12 for the three 16-byte vectors AbilityButtons..QuickAccessButtonsAlpha).
// Shape: /O2.
enum EPlayerGuiMode { PLAYER_GUI_MODE_1 = 1, PLAYER_GUI_MODE_3 = 3 };
class CPlayerGui {
public:
    unsigned char  _pad000[0x294];
    EPlayerGuiMode GuiMode;                              // +0x294
    float          FadeInTimer;                          // +0x298
    void Close(bool fade);
    void ClearGui();                                     // 0x00438230
};
void CPlayerGui::Close(bool fade)
{
    if (fade) {
        GuiMode = PLAYER_GUI_MODE_1;
        FadeInTimer = 0.0f;
    }
    else
        GuiMode = PLAYER_GUI_MODE_3;
    ClearGui();
}