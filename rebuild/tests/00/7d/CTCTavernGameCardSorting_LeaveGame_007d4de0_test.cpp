#include <cstdio>
// Behaviour test for CTCTavernGameCardSorting::LeaveGame (0x007D4DE0): stubbed callees record the calls.
class CThing { public: int killed; };
static int g_baseLeave = 0;
static CThing* g_killed = 0;

struct CTCTavernGameCardBase {
    unsigned char _pad[0x140];
    void LeaveGame() { ++g_baseLeave; }
    void KillThing(CThing* thing) { g_killed = thing; if (thing) thing->killed = 1; }
};

struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    CThing* m_pCardThing;
    void LeaveGame();
};

void CTCTavernGameCardSorting::LeaveGame()
{
    CTCTavernGameCardBase::LeaveGame();
    KillThing(m_pCardThing);
    m_pCardThing = 0;
}

int main()
{
    static CTCTavernGameCardSorting g;
    CThing t; t.killed = 0;
    g.m_pCardThing = &t;
    g.LeaveGame();
    if (g_baseLeave != 1 || g_killed != &t || t.killed != 1 || g.m_pCardThing != 0) { std::printf("007d4de0_TEST FAIL\n"); return 1; }
    std::printf("007d4de0_TEST PASS\n");
    return 0;
}