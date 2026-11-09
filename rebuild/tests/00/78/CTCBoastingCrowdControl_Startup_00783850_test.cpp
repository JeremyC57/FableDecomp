// Behaviour test for 0x00783850 CTCBoastingCrowdControl::Startup (C++03 model): disabled boasting, no boasts, no podium, a podium
// in another region, no marker or no village -> nothing; otherwise the renown-level crowd size (halved once, at least 1, while
// suppressed; the flag is then cleared) spawn attempts at the NEAREST village, and Active set if any spawn succeeded.
#include <cstdio>

struct V3 { float X, Y, Z; V3(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct Thing { V3 Pos; long Map; Thing(float x, long map) : Pos(x, 0, 0), Map(map) { } };
static bool g_boastingEnabled = true;
static int g_boasts = 1;
static const Thing* g_podium = 0;
static const Thing* g_marker = 0;
static Thing* g_villages[3] = { 0, 0, 0 };
static long g_maxCrowd = 4;
static int g_spawnCalls = 0, g_spawnOk = 0;
static Thing* g_lastVillage = 0;
static long Region(long map) { return map / 10; }
static Thing* Nearest(const V3& pos)   // CThingCompare_Nearest over the villages (the landed 0x0048B160 rule)
{
    Thing* best = 0;
    for (int i = 0; i < 3; ++i) {
        Thing* c = g_villages[i];
        if (!c) continue;
        if (!best) { best = c; continue; }
        float da = (best->Pos.X - pos.X) * (best->Pos.X - pos.X), db = (c->Pos.X - pos.X) * (c->Pos.X - pos.X);
        best = da < db ? best : c;
    }
    return best;
}
struct Control {
    bool Active; bool CrowdSizesSuppressed; long HeroMap;
    bool Spawn(Thing& village) { ++g_spawnCalls; g_lastVillage = &village; return g_spawnCalls <= g_spawnOk; }
    void Startup()
    {
        if (!g_boastingEnabled) return;
        if (g_boasts == 0) return;
        if (!g_podium) return;
        if (Region(g_podium->Map) != Region(HeroMap)) return;
        if (!g_marker) return;
        Thing* village = Nearest(g_marker->Pos);
        if (!village) return;
        long max = g_maxCrowd;
        if (max <= 0) return;
        if (CrowdSizesSuppressed) { max = (long)(max < 2 ? 1.0f : (float)max * 0.5f); CrowdSizesSuppressed = false; }
        bool spawned = false;
        for (long i = 0; i < max; ++i) if (Spawn(*village)) spawned = true;
        if (spawned) Active = true;
    }
};
int main()
{
    bool ok = true;
    Thing podium(0, 21), marker(10, 0), v1(30, 0), v2(12, 0), v3(-5, 0);
    Control c = { false, false, 25 };
    c.Startup(); if (g_spawnCalls != 0 || c.Active) ok = false;                     // no podium
    g_podium = &podium; g_marker = &marker; g_villages[0] = &v1; g_villages[1] = &v2; g_villages[2] = &v3;
    g_boastingEnabled = false; c.Startup(); if (g_spawnCalls != 0) ok = false;      // disabled
    g_boastingEnabled = true; c.HeroMap = 35; c.Startup(); if (g_spawnCalls != 0) ok = false;   // other region
    c.HeroMap = 25; g_spawnOk = 0; c.Startup();
    if (g_spawnCalls != 4 || c.Active || g_lastVillage != &v2) ok = false;         // 4 tries at the nearest, none spawned
    g_spawnCalls = 0; g_spawnOk = 1; c.CrowdSizesSuppressed = true; c.Startup();
    if (g_spawnCalls != 2 || !c.Active || c.CrowdSizesSuppressed) ok = false;       // halved to 2, one spawned -> Active
    g_spawnCalls = 0; g_maxCrowd = 1; c.CrowdSizesSuppressed = true; c.Startup();
    if (g_spawnCalls != 1) ok = false;                                               // max < 2 -> 1
    if (!ok) { std::printf("00783850_TEST FAIL\n"); return 1; }
    std::printf("00783850_TEST PASS\n");
    return 0;
}