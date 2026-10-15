#include <cstdio>
// Behaviour test for CTCTavernGameCoinGolf::vfunc_11 (0x007D4A60).
struct CRenderContext { int id; };
static int g_prepare = 0, g_fade = 0, g_fadeArg = -1;
struct CTCTavernGame {
    unsigned char _pad[0x20];
    bool IsBeingPlayed;
    void DrawPreparePrimitivesForRendering(CRenderContext* context) { if (context) ++g_prepare; }
};
struct CTCTavernGameCoinGolf : CTCTavernGame {
    void FadeOutTableOccupants(bool fadeOut) { ++g_fade; g_fadeArg = fadeOut ? 1 : 0; }
    void vfunc_11(CRenderContext* context);
};
void CTCTavernGameCoinGolf::vfunc_11(CRenderContext* context)
{
    CTCTavernGame::DrawPreparePrimitivesForRendering(context);
    if (IsBeingPlayed)
        FadeOutTableOccupants(false);
}
int main()
{
    static CTCTavernGameCoinGolf g;
    CRenderContext rc; rc.id = 0;
    g.IsBeingPlayed = false;
    g.vfunc_11(&rc);
    if (g_prepare != 1 || g_fade != 0) { std::printf("007d4a60_TEST FAIL\n"); return 1; }
    g.IsBeingPlayed = true;
    g.vfunc_11(&rc);
    if (g_prepare != 2 || g_fade != 1 || g_fadeArg != 0) { std::printf("007d4a60_TEST FAIL\n"); return 1; }
    std::printf("007d4a60_TEST PASS\n");
    return 0;
}
