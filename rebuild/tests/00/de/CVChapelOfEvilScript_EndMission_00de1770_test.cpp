// Behaviour test for 0x00DE1770 (EndMission, Knothole Glade house step).
#include <cstdio>
#include <cstring>
// Host model: logs 'F' frame, 'S' house made scripted, 'L' house locking overridden (only for ChiefsHut with true);
// region-loaded messages arrive on the frames in 'msgMask'; KnotholeGlade is loaded when 'glade' is set.
struct Thing { bool hut; };
struct Game {
    char log[256]; int frames; unsigned msgMask; bool glade; int live;
    Game() : frames(0), msgMask(0), glade(false), live(0) { log[0] = 0; }
    void Add(char c) { size_t n = std::strlen(log); if (n < 250) { log[n] = c; log[n + 1] = 0; } }
    void NewScriptFrame() { Add('F'); ++frames; }
    bool MsgOnRegionLoaded(char* region) { if (msgMask & (1u << frames)) { std::strcpy(region, "X"); return true; } return false; }
    bool IsRegionLoaded(const char* r) { return glade && std::strcmp(r, "KnotholeGlade") == 0; }
    Thing GetThingWithScriptName(const char* n) { Thing t; t.hut = std::strcmp(n, "ChiefsHut") == 0; return t; }
    void SetBuyableHouseAsScripted(const Thing& t, bool b) { Add(t.hut && b ? 'S' : '?'); }
    void OverrideAutomaticHouseLocking(const Thing& t, bool b) { Add(t.hut && b ? 'L' : '?'); }
};
struct Script {
    Game* GSI; bool Go; bool RegionSeen; int termAfter; int checks;
    Script(Game* g, bool go, int after) : GSI(g), Go(go), RegionSeen(false), termAfter(after), checks(0) {}
    bool IsActiveThreadTerminating() { return ++checks > termAfter; }
    void EndMission()
    {
        while (!Go) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        char region[16] = "";
        while (!IsActiveThreadTerminating()) {
            if (GSI->MsgOnRegionLoaded(region)) {
                if (IsActiveThreadTerminating()) return;
                RegionSeen = true;
                if (GSI->IsRegionLoaded("KnotholeGlade")) {
                    if (IsActiveThreadTerminating()) return;
                    GSI->SetBuyableHouseAsScripted(GSI->GetThingWithScriptName("ChiefsHut"), true);
                    GSI->OverrideAutomaticHouseLocking(GSI->GetThingWithScriptName("ChiefsHut"), true);
                }
            }
            GSI->NewScriptFrame();
        }
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    { Game g; Script s(&g, false, 3); s.EndMission(); CHECK(std::strcmp(g.log, "FFFF") == 0 && !s.RegionSeen); }
    { Game g; g.msgMask = 1u << 1; Script s(&g, true, 4); s.EndMission(); CHECK(std::strcmp(g.log, "FF") == 0 && s.RegionSeen); }
    { Game g; g.msgMask = 1u << 0; g.glade = true; Script s(&g, true, 6); s.EndMission(); CHECK(std::strcmp(g.log, "SLFFF") == 0 && s.RegionSeen); }
    if (g_fail) { std::printf("00de1770_TEST FAIL\n"); return 1; }
    std::printf("00de1770_TEST PASS\n");
    return 0;
}