#include <cstdio>
#include <cstring>

static int g_strings, g_frames, g_doneCalls, g_doneAfter = 1000, g_regionAt = -1, g_sealAt = -1;
static bool g_active, g_activated;
struct CCharString {
    const char* p;
    CCharString(const char* text, long = -1) : p(text) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
struct CScriptThing { int id; };
static CScriptThing g_hero = { 1 };
struct CGameScriptInterface_Calls {
    void NewScriptFrame() const { ++g_frames; }
    bool IsRegionLoaded(const CCharString& region) const { return std::strcmp(region.p, "LookoutPoint") == 0 && g_regionAt >= 0 && g_frames >= g_regionAt; }
    CScriptThing& GetHero() { return g_hero; }
    bool IsObjectInThingsPossession(const CCharString& object, const CScriptThing& thing) const
    { return std::strcmp(object.p, "OBJECT_GUILD_SEAL_1") == 0 && thing.id == 1 && g_sealAt >= 0 && g_frames >= g_sealAt; }
    void SetTeleporterAsActive(const CScriptThing& t, bool active) const { if (t.id == 9 && active) g_activated = true; }
    bool IsTeleporterActive(const CScriptThing& t) const { return t.id == 9 && g_active; }
};
struct CMarker {
    CGameScriptInterface_Calls* Game; CScriptThing Me;
    bool IsDone() { return ++g_doneCalls >= g_doneAfter; }
    void Main()
    {
        if (Game->IsTeleporterActive(Me)) return;
        if (IsDone()) return;
        for (;;) {
            bool waiting = !(Game->IsRegionLoaded("LookoutPoint") && Game->IsObjectInThingsPossession("OBJECT_GUILD_SEAL_1", Game->GetHero()));
            if (!waiting) break;
            Game->NewScriptFrame();
            if (IsDone()) return;
        }
        if (IsDone()) return;
        Game->SetTeleporterAsActive(Me, true);
    }
};
static void Reset() { g_frames = g_doneCalls = 0; g_doneAfter = 1000; g_regionAt = g_sealAt = -1; g_active = g_activated = false; }
int main()
{
    CGameScriptInterface_Calls game; CMarker m; m.Game = &game; m.Me.id = 9;
    Reset(); g_regionAt = 1; g_sealAt = 3;
    m.Main();
    if (!g_activated || g_frames != 3 || g_strings != 0) { std::printf("00cdf620_TEST FAIL\n"); return 1; }
    Reset(); g_active = true;
    m.Main();
    if (g_activated || g_doneCalls != 0) { std::printf("00cdf620_TEST FAIL\n"); return 1; }
    Reset(); g_doneAfter = 4;                          // never satisfied: ends when the script stops
    m.Main();
    if (g_activated || g_frames != 3) { std::printf("00cdf620_TEST FAIL\n"); return 1; }
    std::printf("00cdf620_TEST PASS\n");
    return 0;
}