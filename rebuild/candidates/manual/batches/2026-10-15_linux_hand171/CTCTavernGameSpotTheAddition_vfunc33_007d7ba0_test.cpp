// Behaviour test for CTCTavernGameSpotTheAddition::vfunc_33 (0x007D7BA0): score = time taken; a won game sets NewBestScore
// when there is no best time yet (0) or the time beats it; state 8.
#include <cstdio>
struct Game { bool GameWon; float Score; bool NewBestScore; float CurrentTime; int state; };
static void End(Game& g, float best)
{
    g.Score = g.CurrentTime;
    if (g.GameWon == true) {
        if (best == 0.0f || g.CurrentTime < best) g.NewBestScore = true;
    }
    g.state = 8;
}
int main()
{
    Game a = { true, 0, false, 10, 0 }; End(a, 0);
    Game b = { true, 0, false, 10, 0 }; End(b, 12);
    Game c = { true, 0, false, 10, 0 }; End(c, 8);
    Game d = { false, 0, false, 10, 0 }; End(d, 0);
    if (!a.NewBestScore || !b.NewBestScore || c.NewBestScore || d.NewBestScore || a.Score != 10 || a.state != 8) {
        std::printf("007d7ba0_TEST FAIL\n"); return 1;
    }
    std::printf("007d7ba0_TEST PASS\n");
    return 0;
}
