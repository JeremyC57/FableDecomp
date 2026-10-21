// behaviour test for 0x00438440
#include <stdio.h>
#include <string.h>
static int g_clears = 0;
enum EPlayerGuiMode { PLAYER_GUI_MODE_OPENING = 2, PLAYER_GUI_MODE_CLOSED = 3, PLAYER_GUI_MODE_FORCE_DWORD = 0x7fffffff };
class CPlayerGui {
public:
    unsigned char  _pad00[0x294];
    EPlayerGuiMode GuiMode;                              // +0x294
    float          FadeInTimer;                          // +0x298
    bool           ForcedClosed;                         // +0x29c
    unsigned char  _pad29d[0x2bd - 0x29d];
    bool           Flag2BD;                              // +0x2bd
    void ClearGui() { ++g_clears; }
    void ForceClose();
};

void CPlayerGui::ForceClose()
{
    ForcedClosed = true;
    Flag2BD = true;
    GuiMode = PLAYER_GUI_MODE_CLOSED;
    ClearGui();
}

int main()
{
    CPlayerGui g; memset(&g, 0, sizeof(g)); g.ForceClose();
    int ok = g.ForcedClosed && g.Flag2BD && g.GuiMode == PLAYER_GUI_MODE_CLOSED && g_clears == 1;
    printf(ok ? "00438440_TEST PASS\n" : "00438440_TEST FAIL\n");
    return ok ? 0 : 1;
}