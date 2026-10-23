#include <cstdio>
#include <cstring>
#include <vector>
// behaviour test: CQ_OakValeBanditRaidScript::PopulateLevel -- restart path tops bandits up to the restart-point count
// (recording the old count unless NoPersistence), first-setup path cleans up and spawns at every spawn point; both
// attach each bandit to the village and refresh the follower list
static int g_created = 0, g_attached = 0, g_cleaned = 0;
struct Thing { int id; };
struct Game {
    int restarts, bandits, spawns, followers;
    void GetAll(const char* n, std::vector<Thing>& v) const {
        int c = std::strcmp(n, "OakValeRestart") == 0 ? restarts : std::strcmp(n, "OakValeBanditRaidBandit") == 0 ? bandits : spawns;
        for (int i = 0; i < c; ++i) { Thing t = { i }; v.push_back(t); }
    }
    Thing Create(int at) const { ++g_created; Thing t = { 100 + at }; return t; }
    void Attach(const Thing& b, const Thing& v) const { if (b.id >= 100 && v.id == 7) ++g_attached; }
    void Followers(std::vector<Thing>& v) const { for (int i = 0; i < followers; ++i) { Thing t = { i }; v.push_back(t); } }
};
struct Script {
    Game* G; bool Aware; std::vector<Thing> HerosFollowers; bool OpenGates, FirstSetup; long StopChasing; bool NoPersistence;
    bool PopulateLevel()
    {
        bool spawned = false;
        Aware = false;
        if (!FirstSetup && OpenGates) {
            std::vector<Thing> restarts; G->GetAll("OakValeRestart", restarts);
            Thing village = { 7 };
            std::vector<Thing> bandits; G->GetAll("OakValeBanditRaidBandit", bandits);
            if (!NoPersistence) StopChasing = (long)bandits.size();
            NoPersistence = false;
            for (unsigned int i = 0; i < restarts.size() - bandits.size(); ++i) {
                spawned = true;
                Thing b = G->Create(restarts[i].id); G->Attach(b, village);
            }
        } else {
            ++g_cleaned;
            std::vector<Thing> spawns; G->GetAll("M_BanditSpawn", spawns);
            Thing village = { 7 };
            FirstSetup = false;
            for (unsigned int i = 0; i < spawns.size(); ++i) { Thing b = G->Create(spawns[i].id); G->Attach(b, village); }
        }
        HerosFollowers.clear();
        G->Followers(HerosFollowers);
        return spawned;
    }
};
int main()
{
    Game g; g.restarts = 5; g.bandits = 2; g.spawns = 4; g.followers = 3;
    Script s; s.G = &g; s.Aware = true; s.OpenGates = true; s.FirstSetup = true; s.StopChasing = -1; s.NoPersistence = false;
    bool ok = !s.PopulateLevel() && g_cleaned == 1 && g_created == 4 && g_attached == 4 && !s.FirstSetup && !s.Aware && s.HerosFollowers.size() == 3;
    g_created = g_attached = 0;
    ok = ok && s.PopulateLevel() && g_created == 3 && g_attached == 3 && s.StopChasing == 2 && !s.NoPersistence && s.HerosFollowers.size() == 3;
    g_created = 0; s.NoPersistence = true; s.StopChasing = 9; g.bandits = 5;
    ok = ok && !s.PopulateLevel() && g_created == 0 && s.StopChasing == 9 && !s.NoPersistence;
    if (!ok) { std::printf("00dc0300_TEST FAIL\n"); return 1; }
    std::printf("00dc0300_TEST PASS\n");
    return 0;
}