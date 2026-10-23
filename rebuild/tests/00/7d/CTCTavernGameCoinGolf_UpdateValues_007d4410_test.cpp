#include <cstdio>
#include <cstring>
// behaviour test: CTCTavernGameCoinGolf::UpdateValues gives the prize once on a winning score, posts scoreboard entries and keeps the best (lowest) score per table
static int g_prizes = 0; static long g_lastActivity = -1;
struct CStats { bool PrizeGolf, PrizeSnow; float BestGolf, BestSnow; };
struct CGolf {
    const char* DefName; bool GameWon; float Score; float PrizeScore; long Rounds; CStats* Stats;
    bool BaseUpdateValues() { return false; }
    void GivePrize() { ++g_prizes; }
    void Post(long activity, float) { g_lastActivity = activity; }
    bool UpdateValues()
    {
        bool result = BaseUpdateValues();
        CStats* stats = Stats;
        if (std::strcmp(DefName, "OBJECT_TAVERN_TABLE_COIN_GOLF") == 0) {
            if (GameWon) {
                if (!stats->PrizeGolf && Score <= PrizeScore) { stats->PrizeGolf = true; GivePrize(); }
                Post(0x16, Score); Post(0x1e, (float)Rounds);
                if (stats->BestGolf == 0.0f || Score < stats->BestGolf) { stats->BestGolf = Score; return true; }
            }
        } else if (std::strcmp(DefName, "OBJECT_TAVERN_TABLE_COIN_GOLF_SNOWSPIRE") == 0 && GameWon) {
            if (!stats->PrizeSnow && Score <= PrizeScore) { stats->PrizeSnow = true; GivePrize(); }
            Post(0x17, Score); Post(0x1f, (float)Rounds);
            if (stats->BestSnow == 0.0f || Score < stats->BestSnow) { stats->BestSnow = Score; return true; }
        }
        return result;
    }
};
int main()
{
    CStats s = { false, false, 0.0f, 0.0f };
    CGolf g; g.DefName = "OBJECT_TAVERN_TABLE_COIN_GOLF"; g.GameWon = true; g.Score = 5.0f; g.PrizeScore = 6.0f; g.Rounds = 2; g.Stats = &s;
    bool ok = g.UpdateValues() && g_prizes == 1 && s.BestGolf == 5.0f && g_lastActivity == 0x1e;
    g.Score = 7.0f; ok = ok && !g.UpdateValues() && g_prizes == 1 && s.BestGolf == 5.0f;
    g.DefName = "OBJECT_TAVERN_TABLE_COIN_GOLF_SNOWSPIRE"; g.Score = 4.0f;
    ok = ok && g.UpdateValues() && g_prizes == 2 && s.BestSnow == 4.0f && g_lastActivity == 0x1f;
    g.GameWon = false; g.Score = 1.0f; ok = ok && !g.UpdateValues() && s.BestSnow == 4.0f;
    if (!ok) { std::printf("007d4410_TEST FAIL\n"); return 1; }
    std::printf("007d4410_TEST PASS\n");
    return 0;
}