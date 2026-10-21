#include <cstdio>
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

static int g_clear; static EPlayerGuiMode g_mode_at_clear;
void CPlayerGui::ClearGui() { ++g_clear; g_mode_at_clear = GuiMode; }
int main()
{
    static double mem[0x300 / 8];
    CPlayerGui* g = (CPlayerGui*)mem;
    g->GuiMode = (EPlayerGuiMode)0; g->FadeInTimer = 5.0f;
    g->Close(true);
    bool ok = g->GuiMode == 1 && g->FadeInTimer == 0.0f && g_clear == 1 && g_mode_at_clear == 1;
    g->FadeInTimer = 5.0f;
    g->Close(false);
    ok = ok && g->GuiMode == 3 && g->FadeInTimer == 5.0f && g_clear == 2 && g_mode_at_clear == 3;
    std::printf(ok ? "00438400_TEST PASS\n" : "FAIL\n");
    return 0;
}