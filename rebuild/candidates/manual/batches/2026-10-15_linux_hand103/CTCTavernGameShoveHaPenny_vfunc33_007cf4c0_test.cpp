#include <cstdio>
// Behaviour test for CTCTavernGameShoveHaPenny::vfunc_33 (0x007CF4C0).
struct CHeroStats { unsigned char _pad[0x18c]; float m_bestShoveHaPennyScore; };
static CHeroStats g_stats; static int g_base = 0; static long g_state = -1;
struct CTCTavernGameCoinBase {
    unsigned char _pad[0x21];
    bool GameWon;
    unsigned char _pad_0x22[0xa];
    float Score;
    unsigned char _pad_0x30[0x70];
    bool NewBestScore;
    void vfunc_33() { ++g_base; }
    void GetHeroStats(CHeroStats** stats) { *stats = &g_stats; }
    void SetState(long state) { g_state = state; }
};
struct CTCTavernGameShoveHaPenny : CTCTavernGameCoinBase { void vfunc_33(); };
void CTCTavernGameShoveHaPenny::vfunc_33()
{
    CTCTavernGameCoinBase::vfunc_33();
    CHeroStats* stats = 0;
    GetHeroStats(&stats);
    if (GameWon) {
        if (stats->m_bestShoveHaPennyScore == 0.0f || Score > stats->m_bestShoveHaPennyScore)
            NewBestScore = true;
    }
    SetState(8);
}
static int Run(CTCTavernGameShoveHaPenny& g, bool won, float score, float best)
{
    g.GameWon = won; g.Score = score; g_stats.m_bestShoveHaPennyScore = best; g.NewBestScore = false; g_state = -1;
    g.vfunc_33();
    if (g_state != 8) return -1;
    return g.NewBestScore ? 1 : 0;
}
int main()
{
    static CTCTavernGameShoveHaPenny g;
    if (Run(g, false, 10.0f, 0.0f) != 0 || Run(g, true, 3.0f, 0.0f) != 1 || Run(g, true, 5.0f, 4.0f) != 1
        || Run(g, true, 4.0f, 4.0f) != 0 || Run(g, true, 2.0f, 4.0f) != 0 || g_base != 5)
    { std::printf("007cf4c0_TEST FAIL\n"); return 1; }
    std::printf("007cf4c0_TEST PASS\n");
    return 0;
}
