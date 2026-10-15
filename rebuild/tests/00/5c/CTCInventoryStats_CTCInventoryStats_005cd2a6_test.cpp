#include <cstdio>

// Model of CTCInventoryStats::CTCInventoryStats(CThing*) @ 0x005CD2A6: both bases built (the inventory base keeps the
// owner), the kill map constructed empty, the five scalar members zeroed.
struct CThing { int id; };
static int g_order[3]; static int g_step = 0;
struct CTCInventoryBase { CThing* Owner; CTCInventoryBase(CThing* t) : Owner(t) { g_order[g_step++] = 1; } virtual ~CTCInventoryBase() { } };
struct CInfoScreenBase { CInfoScreenBase() { g_order[g_step++] = 2; } virtual ~CInfoScreenBase() { } };
struct CCreatureKillMap { long count; CCreatureKillMap() : count(0) { g_order[g_step++] = 3; } };
struct CTCInventoryStats : CTCInventoryBase, CInfoScreenBase {
    CCreatureKillMap CreaturesKilled;
    CThing* pMannequin; bool UseMannequin; long lastCategoryIdx; void* pTCHeroExperience; void* pTCHeroStats;
    CTCInventoryStats(CThing* thing)
        : CTCInventoryBase(thing), pMannequin(0), UseMannequin(false), lastCategoryIdx(0), pTCHeroExperience(0), pTCHeroStats(0) { }
};

int main()
{
    CThing t; t.id = 7;
    CTCInventoryStats* s = new CTCInventoryStats(&t);
    bool ok = s->Owner == &t && s->CreaturesKilled.count == 0 && s->pMannequin == 0 && !s->UseMannequin &&
              s->lastCategoryIdx == 0 && s->pTCHeroExperience == 0 && s->pTCHeroStats == 0 &&
              g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3;
    delete s;
    if (!ok) { std::printf("005cd2a6_TEST FAIL\n"); return 1; }
    std::printf("005cd2a6_TEST PASS\n");
    return 0;
}