// behaviour test for 0x004374B0
#include <stdio.h>
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
int main()
{
    CPlayerGui g; g.GuiMode = (EPlayerGuiMode)0; g.ForcedClosed = false; int ok = 1;
    g.Open(); ok &= g.GuiMode == PLAYER_GUI_MODE_OPENING;
    g.GuiMode = (EPlayerGuiMode)0; g.ForcedClosed = true; g.Open(); ok &= g.GuiMode == (EPlayerGuiMode)0;
    printf(ok ? "004374b0_TEST PASS\n" : "004374b0_TEST FAIL\n");
    return ok ? 0 : 1;
}