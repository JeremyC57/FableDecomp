#include <cstdio>
#include <cstring>
// behaviour test: CTC_GuardSpawnPoint::Main spawns up to two guards per wave while fewer than 25 have come in, picking the guard type from the remaining-count thresholds
struct CParent { long AliveCount, InitialNumberInRegion, NumberSpawned, NextTimeToSpawnGuards; };
struct CDef { long Blue, Crossbow, Red; };
static const char* g_names[64]; static int g_spawned = 0;
inline long NumLeftToSpawn(const CParent* p) { return 25 - p->InitialNumberInRegion - p->NumberSpawned; }
static void SpawnWave(CParent* parent, const CDef& def, int roll)
{
    if (parent->AliveCount - parent->InitialNumberInRegion - parent->NumberSpawned + 25 < parent->NextTimeToSpawnGuards || parent->AliveCount < 7) {
        for (long i = 0; i < 2 && NumLeftToSpawn(parent) > 0; ++i) {
            const char* name;
            if (NumLeftToSpawn(parent) > def.Blue) name = "CREATURE_BS_GUARD_BLUE";
            else if (NumLeftToSpawn(parent) > def.Crossbow) name = "CREATURE_BS_GUARD_BLUE_CROSSBOW";
            else if (NumLeftToSpawn(parent) > def.Red) name = "CREATURE_BS_GUARD_RED";
            else name = "CREATURE_BS_GUARD_BLACK";
            if (roll % 500 == 0) name = "CREATURE_BS_SHERIFF";
            g_names[g_spawned++] = name;
            ++parent->NumberSpawned;
        }
        parent->NextTimeToSpawnGuards = parent->AliveCount - parent->InitialNumberInRegion - parent->NumberSpawned + 24;
    }
}
int main()
{
    CParent p = { 0, 20, 0, 0 };
    CDef d = { 4, 2, 1 };
    SpawnWave(&p, d, 1);
    bool ok = g_spawned == 2 && std::strcmp(g_names[0], "CREATURE_BS_GUARD_BLUE") == 0 && std::strcmp(g_names[1], "CREATURE_BS_GUARD_BLUE_CROSSBOW") == 0;
    SpawnWave(&p, d, 1);
    ok = ok && g_spawned == 4 && std::strcmp(g_names[2], "CREATURE_BS_GUARD_BLUE_CROSSBOW") == 0 && std::strcmp(g_names[3], "CREATURE_BS_GUARD_RED") == 0;
    SpawnWave(&p, d, 500);
    ok = ok && g_spawned == 5 && std::strcmp(g_names[4], "CREATURE_BS_SHERIFF") == 0 && NumLeftToSpawn(&p) == 0 && p.NextTimeToSpawnGuards == -1;
    if (!ok) { std::printf("00df7bf0_TEST FAIL\n"); return 1; }
    std::printf("00df7bf0_TEST PASS\n");
    return 0;
}