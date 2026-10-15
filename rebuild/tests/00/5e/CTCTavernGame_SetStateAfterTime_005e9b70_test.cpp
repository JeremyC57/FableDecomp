// Behaviour test for CTCTavernGame::SetStateAfterTime @ 0x005E9B70: quit request leaves; else wait state 14 then `state` after `time`.
#include <cstdio>
static bool g_quit;
struct CTCTavernGame {
    long TavernGameState, PreviousTavernGameState, NextTavernGameState; float StateTimer; int left;
    void LeaveGame() { ++left; }
    void SetStateAfterTime(long state, float time)
    {
        if (g_quit) { g_quit = false; LeaveGame(); return; }
        PreviousTavernGameState = TavernGameState; TavernGameState = 14; NextTavernGameState = state; StateTimer = time;
    }
};
int main()
{
    CTCTavernGame g; g.TavernGameState = 3; g.PreviousTavernGameState = 0; g.NextTavernGameState = 0; g.StateTimer = 0.0f; g.left = 0; g_quit = false;
    g.SetStateAfterTime(9, 2.5f);
    if (g.PreviousTavernGameState != 3 || g.TavernGameState != 14 || g.NextTavernGameState != 9 || g.StateTimer != 2.5f || g.left) { std::printf("005e9b70_TEST FAIL\n"); return 1; }
    g_quit = true; g.SetStateAfterTime(4, 1.0f);
    if (g_quit || g.left != 1 || g.TavernGameState != 14 || g.NextTavernGameState != 9) { std::printf("005e9b70_TEST FAIL\n"); return 1; }
    std::printf("005e9b70_TEST PASS\n");
    return 0;
}