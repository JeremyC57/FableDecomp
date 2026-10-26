#include <cstdio>
#include <cstring>
#include <vector>
// behaviour test: CQ_AmbushTradersScript::WatchForRegionClear -- the region is clear once the spy and every collected group
// (bandit allies, convoy members, phase 1/3 guards and archers) are dead; termination stops the wait without setting it
static int g_frames = 0, g_endAt = 1000, g_spyDiesAt = 2, g_groupDiesAt = 4;
class CScriptThing { public: const char* name; bool alive; bool IsAlive() const { return alive && g_frames < g_spyDiesAt; } };
static bool AreAllThingsInVectorDead(std::vector<CScriptThing>* things)
{
    for (size_t i = 0; i < things->size(); ++i)
        if (std::strcmp((*things)[i].name, "AT_Phase1Archer") == 0 ? g_frames < g_groupDiesAt : false) return false;
    return true;
}
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    long GetAllThingsWithScriptName(const char* name, std::vector<CScriptThing>& things) const
    { CScriptThing t; t.name = name; t.alive = true; things.push_back(t); return 1; }
    CScriptThing GetThingWithScriptName(const char* name) const { CScriptThing t; t.name = name; t.alive = !std::strcmp(name, "AT_ConvoySpy"); return t; }
};
struct CQ_AmbushTradersScript {
    CGameScriptInterface* Game;
    bool RegionClear;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void WatchForRegionClear();
};
void CQ_AmbushTradersScript::WatchForRegionClear()
{
    std::vector<CScriptThing> banditAllies, convoyMembers, phase1Guards, phase1Archers, phase3Guards, phase3Archers;
    Game->GetAllThingsWithScriptName("AT_BanditAlly", banditAllies);
    Game->GetAllThingsWithScriptName("AT_ConvoyMember", convoyMembers);
    Game->GetAllThingsWithScriptName("AT_Phase3Guard", phase3Guards);
    Game->GetAllThingsWithScriptName("AT_Phase3Archer", phase3Archers);
    Game->GetAllThingsWithScriptName("AT_Phase1Guard", phase1Guards);
    Game->GetAllThingsWithScriptName("AT_Phase1Archer", phase1Archers);
    CScriptThing spy = Game->GetThingWithScriptName("AT_ConvoySpy");
    for (;;) {
        if (IsActiveThreadTerminating()) return;
        RegionClear = !spy.IsAlive() && AreAllThingsInVectorDead(&banditAllies) && AreAllThingsInVectorDead(&convoyMembers)
            && AreAllThingsInVectorDead(&phase3Guards) && AreAllThingsInVectorDead(&phase3Archers)
            && AreAllThingsInVectorDead(&phase1Guards) && AreAllThingsInVectorDead(&phase1Archers);
        if (RegionClear) break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating()) return;
}
int main()
{
    CGameScriptInterface game; CQ_AmbushTradersScript s; s.Game = &game; s.RegionClear = false;
    s.WatchForRegionClear();
    bool ok = s.RegionClear && g_frames == 4;              // spy dead at frame 2, the phase 1 archers at frame 4
    g_frames = 0; g_endAt = 3; s.RegionClear = false;
    s.WatchForRegionClear();
    ok = ok && !s.RegionClear && g_frames == 3;
    if (!ok) { std::printf("00f09cd0_TEST FAIL\n"); return 1; }
    std::printf("00f09cd0_TEST PASS\n");
    return 0;
}