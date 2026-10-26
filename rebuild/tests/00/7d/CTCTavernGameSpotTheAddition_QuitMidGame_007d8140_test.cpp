#include <cstdio>
// Behaviour test for CTCTavernGameSpotTheAddition::QuitMidGame (0x007D8140).
static long g_state = -1; static float g_cover = -1.0f;
class CTCTavernGame { public:
    unsigned char _pad[0x21];
    bool GameWon;
    unsigned char _pad_0x22[0x6];
    bool GameFinished;
    void SetState(long state) { g_state = state; }
};
struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    void PositionCover(float amount) { g_cover = amount; }
    void QuitMidGame();
};
void CTCTavernGameSpotTheAddition::QuitMidGame()
{
    GameFinished = true;
    GameWon = false;
    PositionCover(1.0f);
    SetState(6);
}
int main()
{
    static CTCTavernGameSpotTheAddition g;
    g.GameWon = true; g.GameFinished = false;
    g.QuitMidGame();
    if (!g.GameFinished || g.GameWon || g_cover != 1.0f || g_state != 6) { std::printf("007d8140_TEST FAIL\n"); return 1; }
    std::printf("007d8140_TEST PASS\n");
    return 0;
}