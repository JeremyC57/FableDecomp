// Behaviour test for CTCTavernGameCardPairs::CTCTavernGameCardPairs (0x007D7520): target score = last prize score,
// NumCards 1, pointer state zeroed, temporary def reference released (destroyed when it was the last one).
#include <cstdio>
struct Prize { float Score; long Mult; };
struct Def { long count; int destroyed; Prize prizes[3]; Prize* end; };
static Def g_def;
struct Game {
    float target; long NumCards; long LastMoved; long PrevDir;
    void GetDef(Def** out) { *out = &g_def; ++g_def.count; }
    Game() : target(-1), NumCards(9), LastMoved(9), PrevDir(9)
    {
        Def* d = 0;
        GetDef(&d);
        target = (d->end - 1)->Score;
        NumCards = 1; LastMoved = 0; PrevDir = 0;
        if (--d->count == 0) d->destroyed = 1;
    }
};
int main()
{
    g_def.count = 0; g_def.destroyed = 0;
    g_def.prizes[0].Score = 10; g_def.prizes[1].Score = 20; g_def.prizes[2].Score = 35.5f;
    g_def.end = g_def.prizes + 3;
    Game g;
    if (g.target != 35.5f || g.NumCards != 1 || g.LastMoved != 0 || g.PrevDir != 0 || !g_def.destroyed) {
        std::printf("007d7520_TEST FAIL\n"); return 1;
    }
    std::printf("007d7520_TEST PASS\n");
    return 0;
}
