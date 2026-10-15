#include <cstdio>
// Behaviour test for CTCTavernGameCoinBase::LeaveGame (0x008EF110).
static int g_kills = 0;
class CThing {
public:
    int killed;
    void Kill(bool immediate) { if (immediate) ++killed; ++g_kills; }
};
static int g_base = 0;
struct CTCTavernGame { unsigned char _pad[0xf0]; void LeaveGame() { ++g_base; } };
struct CTCTavernGameCoinBase : CTCTavernGame {
    CThing* m_pCoin0; CThing* m_pCoin1; CThing* m_pCoin2;
    void LeaveGame();
};
void CTCTavernGameCoinBase::LeaveGame()
{
    CTCTavernGame::LeaveGame();
    if (m_pCoin0)
        m_pCoin0->Kill(true);
    if (m_pCoin1)
        m_pCoin1->Kill(true);
    if (m_pCoin2)
        m_pCoin2->Kill(true);
    m_pCoin2 = 0;
    m_pCoin1 = 0;
    m_pCoin0 = 0;
}
int main()
{
    static CTCTavernGameCoinBase g;
    CThing a, c; a.killed = 0; c.killed = 0;
    g.m_pCoin0 = &a; g.m_pCoin1 = 0; g.m_pCoin2 = &c;
    g.LeaveGame();
    if (g_base != 1 || g_kills != 2 || a.killed != 1 || c.killed != 1 || g.m_pCoin0 || g.m_pCoin1 || g.m_pCoin2)
    { std::printf("008ef110_TEST FAIL\n"); return 1; }
    std::printf("008ef110_TEST PASS\n");
    return 0;
}
