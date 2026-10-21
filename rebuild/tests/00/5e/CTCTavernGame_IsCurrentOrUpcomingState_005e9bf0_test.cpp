// test for 005e9bf0 CTCTavernGame::IsCurrentOrUpcomingState
#include <stdio.h>
#include <string.h>
class CTCTavernGame {
public:
    enum ETavernGameStates { TAVERN_GAME_STATE_NONE };
    bool IsCurrentOrUpcomingState(ETavernGameStates state);
    unsigned char     _pad00[0xa8];
    ETavernGameStates YesState;                          // +0xa8
    ETavernGameStates NoState;                           // +0xac
    ETavernGameStates AltState;                          // +0xb0
    void (*YesFunc)();                                   // +0xb4
    void (*NoFunc)();                                    // +0xb8
    void (*AltFunc)();                                   // +0xbc
    ETavernGameStates TavernGameState;                   // +0xc0
};
bool CTCTavernGame::IsCurrentOrUpcomingState(ETavernGameStates state)
{
    return TavernGameState == state || YesState == state || AltState == state;
}

int main()
{
    typedef CTCTavernGame::ETavernGameStates E;
    CTCTavernGame g; memset(&g, 0, sizeof(g));
    g.TavernGameState = (E)1; g.YesState = (E)2; g.NoState = (E)3; g.AltState = (E)4;
    int ok = g.IsCurrentOrUpcomingState((E)1) && g.IsCurrentOrUpcomingState((E)2) && !g.IsCurrentOrUpcomingState((E)3)
          && g.IsCurrentOrUpcomingState((E)4) && !g.IsCurrentOrUpcomingState((E)5);
    printf(ok ? "005e9bf0_TEST PASS\n" : "005e9bf0_TEST FAIL\n");
    return ok ? 0 : 1;
}