// Behaviour test for CTCTavernGameCardBase::NewRound @ 0x008F0960: base NewRound, round flag and table bounds cleared, state 18.
#include <cstdio>
struct CTCTavernGame { bool GameInSession; long state; void NewRoundBase() { GameInSession = true; } void SetState(long s) { state = s; } };
struct CTCTavernGameCardBase : CTCTavernGame {
    float MinX, MaxX, MinY, MaxY; bool RoundFinished;
    void NewRound() { NewRoundBase(); RoundFinished = false; MaxY = 0.0f; MinY = 0.0f; MaxX = 0.0f; MinX = 0.0f; SetState(18); }
};
int main()
{
    CTCTavernGameCardBase g; g.GameInSession = false; g.state = 0; g.MinX = 1; g.MaxX = 2; g.MinY = 3; g.MaxY = 4; g.RoundFinished = true;
    g.NewRound();
    if (!g.GameInSession || g.state != 18 || g.RoundFinished || g.MinX != 0.0f || g.MaxX != 0.0f || g.MinY != 0.0f || g.MaxY != 0.0f) { std::printf("008f0960_TEST FAIL\n"); return 1; }
    std::printf("008f0960_TEST PASS\n");
    return 0;
}