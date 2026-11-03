// Behaviour test for 0x005E9B20 CTCTavernGame::SetState: a stand-alone model of the three members and the LeaveGame virtual.
#include <stdio.h>
static bool g_bTavernGameQuitRequested = false;
static int leaveCalls = 0;
struct TavernGame {
    int TavernGameState, PreviousTavernGameState; bool GamePlayed;
    TavernGame() : TavernGameState(1), PreviousTavernGameState(0), GamePlayed(false) {}
    virtual void LeaveGame() { ++leaveCalls; }
    void SetState(int state)
    {
        if (g_bTavernGameQuitRequested) { g_bTavernGameQuitRequested = false; state = 47; }
        if (state == 5) GamePlayed = true;
        else if (state == 47) { LeaveGame(); return; }
        PreviousTavernGameState = TavernGameState;
        TavernGameState = state;
    }
};
int main()
{
    TavernGame g; int ok = 1;
    g.SetState(5);  ok &= g.GamePlayed && g.TavernGameState == 5 && g.PreviousTavernGameState == 1;
    g.SetState(7);  ok &= g.TavernGameState == 7 && g.PreviousTavernGameState == 5;
    g.SetState(47); ok &= leaveCalls == 1 && g.TavernGameState == 7;
    g_bTavernGameQuitRequested = true;
    g.SetState(3);  ok &= leaveCalls == 2 && !g_bTavernGameQuitRequested && g.TavernGameState == 7;
    if (ok) printf("005e9b20_TEST PASS\n");
    return ok ? 0 : 1;
}