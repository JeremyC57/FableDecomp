#include <cstdio>
// Behaviour test for CTCTavernGameShoveHaPenny::NewGame (0x007CF4A0).
static int g_base = 0; static long g_state = -1;
struct CTCTavernGameCoinBase {
    unsigned char _pad[0x2c];
    float Score;
    unsigned char _pad_0x30[0x118];
    long m_numShoves;
    void NewGame() { ++g_base; }
    void SetState(long state) { g_state = state; }
};
struct CTCTavernGameShoveHaPenny : CTCTavernGameCoinBase { void NewGame(); };
void CTCTavernGameShoveHaPenny::NewGame()
{
    CTCTavernGameCoinBase::NewGame();
    Score = 0.0f;
    m_numShoves = 0;
    SetState(7);
}
int main()
{
    static CTCTavernGameShoveHaPenny g;
    g.Score = 12.5f; g.m_numShoves = 4;
    g.NewGame();
    if (g_base != 1 || g.Score != 0.0f || g.m_numShoves != 0 || g_state != 7) { std::printf("007cf4a0_TEST FAIL\n"); return 1; }
    std::printf("007cf4a0_TEST PASS\n");
    return 0;
}