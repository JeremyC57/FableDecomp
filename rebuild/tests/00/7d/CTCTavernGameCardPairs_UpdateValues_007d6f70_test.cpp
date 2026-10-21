#include <cstdio>
// Behaviour test for CTCTavernGameCardPairs::UpdateValues (0x007D6F70): the body of src/ over stand-in collaborators -- base
// UpdateValues result, hero stats, the def's first prize score (8), the prize grant, the scoreboard calls.
static int g_prizes = 0, g_calls = 0, g_releases = 0;
static long g_act[4]; static float g_val[4]; static bool g_flag[4];
static bool g_baseResult = false;
bool g_bTavernPrizesEnabled = true, g_bTavernScoreReported = false;
struct CTCHeroOnlineScoreboard {
    void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter)
    { g_act[g_calls] = activity; g_val[g_calls] = score; g_flag[g_calls] = onlyIfBetter; ++g_calls; }
};
static CTCHeroOnlineScoreboard g_board;
struct CThing {
    template<class T> bool GetInterface(T** ppOut, long interfaceType) { if (interfaceType != 0x10d) return false; *ppOut = &g_board; return true; }
};
static CThing g_hero;
struct CPlayer { CThing* GetHeroThing() { return &g_hero; } };
static CPlayer g_player;
struct CPlayerManager { CPlayer* GetMainPlayer() const { return &g_player; } };
struct CMainGameComponent { CPlayerManager* PlayerManager; };
static CPlayerManager g_pm;
static CMainGameComponent g_mgc = { &g_pm };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
struct CPrizeScoreDef { float Score; float Mult; };
struct CTavernGameDef {
    long m_count;
    CPrizeScoreDef* PrizeScores;
    void Release() { ++g_releases; --m_count; }
};
static CPrizeScoreDef g_prizeScores[1] = { { 8.0f, 1.0f } };
static CTavernGameDef g_def = { 1, g_prizeScores };
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameDef* operator->() const { return Object; }
};
struct CTCHeroStats { float BestTimePairs; bool PrizeGivenPairs; };
static CTCHeroStats g_stats;
struct CTCTavernGame {
    bool  GameWon;
    float Score;
    long  NumConsecutiveRoundsWon;
    float Counter;
    bool UpdateValues() { return g_baseResult; }
    void GetHeroStats(CTCHeroStats** stats) { *stats = &g_stats; }
    void GetDef(CDefPointer* out) { out->Object = &g_def; ++g_def.m_count; }
    void GivePrize() { ++g_prizes; }
};
struct CTCTavernGameCardPairs : CTCTavernGame {
    bool UpdateValues();
};

bool CTCTavernGameCardPairs::UpdateValues()
{
    bool result = CTCTavernGame::UpdateValues();
    CTCHeroStats* stats = 0;
    GetHeroStats(&stats);
    CDefPointer def;
    GetDef(&def);
    if (GameWon) {
        if (!stats->PrizeGivenPairs && Score <= def->PrizeScores[0].Score) {
            stats->PrizeGivenPairs = true;
            GivePrize();
        }
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
        CTCHeroOnlineScoreboard* scoreboard;
        hero->GetInterface(&scoreboard, 0x10d);
        scoreboard->UpdateScoreForActivity(0x12, Counter, true);
        scoreboard->UpdateScoreForActivity(0x1a, (float)NumConsecutiveRoundsWon, false);
        if (stats->BestTimePairs == 0.0f || Score < stats->BestTimePairs) {
            stats->BestTimePairs = Counter;
            return true;
        }
    }
    return result;
}

static int fail(int line) { printf("007d6f70_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CTCTavernGameCardPairs game;
    game.GameWon = false; game.Score = 5.0f; game.Counter = 5.0f; game.NumConsecutiveRoundsWon = 3;
    g_stats.BestTimePairs = 0.0f; g_stats.PrizeGivenPairs = false;
    g_baseResult = true;
    CHECK(game.UpdateValues() == true);                 // lost round: base result, nothing else
    CHECK(g_prizes == 0 && g_calls == 0 && g_releases == 1 && g_def.m_count == 1);
    g_baseResult = false;
    CHECK(game.UpdateValues() == false);

    game.GameWon = true;                                 // first win: prize, both scores, new best
    CHECK(game.UpdateValues() == true);
    CHECK(g_prizes == 1 && g_stats.PrizeGivenPairs);
    CHECK(g_calls == 2 && g_act[0] == 0x12 && g_val[0] == 5.0f && g_flag[0] == true);
    CHECK(g_act[1] == 0x1a && g_val[1] == 3.0f && g_flag[1] == false);
    CHECK(g_stats.BestTimePairs == 5.0f);
    CHECK(g_def.m_count == 1);

    g_calls = 0;                                         // prize already given, best not beaten
    g_stats.BestTimePairs = 1.0f;
    CHECK(game.UpdateValues() == false);
    CHECK(g_prizes == 1 && g_calls == 2 && g_stats.BestTimePairs == 1.0f);

    g_calls = 0;                                         // best beaten, score outside the prize
    g_stats.PrizeGivenPairs = false; g_stats.BestTimePairs = 100.0f;
    game.Score = 10.0f; game.Counter = 10.0f;
    CHECK(game.UpdateValues() == true);
    CHECK(g_prizes == 1 && !g_stats.PrizeGivenPairs && g_stats.BestTimePairs == 10.0f);
    printf("007d6f70_TEST PASS\n");
    return 0;
}