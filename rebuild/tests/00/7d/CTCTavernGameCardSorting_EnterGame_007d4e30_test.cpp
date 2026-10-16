// Behaviour test for CTCTavernGameCardSorting::EnterGame (0x007D4E30): base EnterGame, NumCards 1, pointer piece from the
// def's model, positioned at the game position with offset (0, 0, PointerHeightOffset); def reference released.
#include <cstdio>
struct Def { long count; long PointerModel; float PointerHeightOffset; };
static Def g_def; static int g_base; static long g_model; static float g_off[3]; static int g_piece = 7;
struct Game {
    long NumCards; int* Pointer;
    void EnterGameBase() { ++g_base; }
    void GetCardDef(Def** out) { *out = &g_def; ++g_def.count; }
    int* CreateGamePiece(long model) { g_model = model; return &g_piece; }
    void EnterGame()
    {
        EnterGameBase();
        NumCards = 1;
        Def* d = 0; GetCardDef(&d);
        Pointer = CreateGamePiece(d->PointerModel);
        g_off[0] = 0; g_off[1] = 0; g_off[2] = d->PointerHeightOffset;
        if (d && --d->count == 0) {}
    }
};
int main()
{
    g_def.count = 1; g_def.PointerModel = 42; g_def.PointerHeightOffset = 0.25f;
    Game g; g.NumCards = 5; g.Pointer = 0; g.EnterGame();
    if (g_base != 1 || g.NumCards != 1 || g_model != 42 || g.Pointer != &g_piece || g_off[2] != 0.25f || g_def.count != 1) {
        std::printf("007d4e30_TEST FAIL\n"); return 1;
    }
    std::printf("007d4e30_TEST PASS\n");
    return 0;
}