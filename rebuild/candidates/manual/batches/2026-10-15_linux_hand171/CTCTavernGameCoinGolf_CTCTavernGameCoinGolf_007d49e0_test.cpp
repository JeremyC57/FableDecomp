// Behaviour test for the coin-game constructor (007d49e0): coin vector, held coin and arrow zeroed, TargetRound = the last prize
// score truncated, the def reference released (destroyed when last).
#include <cstdio>
struct Prize { float Score; long Mult; };
struct Def { long count; int destroyed; Prize prizes[2]; };
struct Game {
    void* Arrow; void* CoinsFirst; void* CoinsLast; void* CoinsEnd; void* Held; long TargetRound;
    Game(Def& d) : CoinsFirst(0), CoinsLast(0), CoinsEnd(0), Held(0)
    {
        Arrow = 0;
        ++d.count;
        TargetRound = (long)(d.prizes + 2 - 1)->Score;
        if (--d.count == 0) d.destroyed = 1;
    }
};
int main()
{
    Def d; d.count = 0; d.destroyed = 0; d.prizes[0].Score = 3.5f; d.prizes[1].Score = 7.9f;
    Game g(d);
    if (g.TargetRound != 7 || g.Arrow || g.CoinsFirst || g.Held || !d.destroyed) { std::printf("007d49e0_TEST FAIL\n"); return 1; }
    std::printf("007d49e0_TEST PASS\n");
    return 0;
}
