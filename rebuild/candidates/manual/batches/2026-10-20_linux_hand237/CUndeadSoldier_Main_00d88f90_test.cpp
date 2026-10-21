// Behaviour test for CUndeadSoldier::Main (0x00D88F90): invulnerable and hostile until the hero is within 15 units, then
// damageable; frames run until the thread terminates.
#include <cstdio>
#include <cstring>

struct CScriptThing { float X; };
static int g_frames, g_filters, g_terminateAt; static bool g_damageable = true; static char g_faction[32]; static float g_lastDist;
struct CGameScriptInterface {
    CScriptThing Hero;
    void NewScriptFrame() const { ++g_frames; }
    CScriptThing& GetHero() { Hero.X = 20.0f - 3.0f * g_frames; return Hero; }
    void EntitySetAsDamageable(const CScriptThing&, bool d) const { g_damageable = d; }
    void EntitySetInFaction(const CScriptThing&, const char* f) const { std::strcpy(g_faction, f); }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d) { g_lastDist = d; float x = b.X - a.X; return x < d; }
struct CUndeadSoldier {
    CGameScriptInterface* Game; CScriptThing Me;
    void RegisterFilter() { ++g_filters; }
    bool IsActiveThreadTerminating() { return g_frames >= g_terminateAt; }
    void Main()
    {
        RegisterFilter();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->EntitySetAsDamageable(Me, false);
        Game->EntitySetInFaction(Me, "FACTION_GUARDS_ENEMY");
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->EntitySetAsDamageable(Me, true);
        for (;;) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
    }
};

int main()
{
    CGameScriptInterface gsi;
    CUndeadSoldier s; s.Game = &gsi; s.Me.X = 0.0f;
    g_terminateAt = 10;
    s.Main();
    bool ok = g_filters == 1 && g_damageable && std::strcmp(g_faction, "FACTION_GUARDS_ENEMY") == 0 && g_lastDist == 15.0f && g_frames == 10;
    g_frames = 0; g_terminateAt = 1; g_damageable = true; g_faction[0] = 0;
    s.Main();
    ok = ok && g_damageable && g_faction[0] == 0;
    if (!ok) { std::printf("00d88f90_TEST FAIL\n"); return 1; }
    std::printf("00d88f90_TEST PASS\n");
    return 0;
}
