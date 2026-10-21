// behaviour test for 0x004374D0
#include <stdio.h>
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
int main()
{
    static CPlayerGui g; g.ForcedClosed = true; g.Flag2BD = true; g.GuiMode = (EPlayerGuiMode)0;
    g.ForceOpen();
    int ok = !g.ForcedClosed && !g.Flag2BD && g.GuiMode == PLAYER_GUI_MODE_OPENING;
    printf(ok ? "004374d0_TEST PASS\n" : "004374d0_TEST FAIL\n");
    return ok ? 0 : 1;
}