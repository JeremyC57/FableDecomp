// Behaviour test for CTCKrakenTentacle::OnInitialActivate (0x007F08B0): tentacle def fetch, ripple kill, the DRIP effect
// attached at TARGET_CURSOR_POINT (mode 0xc) when alive, the MEETS_WATER effect stored as the ripple and attached at
// EFFECT_WATER (mode 0x4a) when alive, then the base activate.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

struct Thing { bool dead; };
static Thing g_things[4];
static int g_nextThing;
static bool g_hasEffects;

static bool GetEffectFromString(const char* name, long* effect)
{
    if (std::strcmp(name, "DRIP") == 0) { *effect = 7; return true; }
    if (std::strcmp(name, "MEETS_WATER") == 0) { *effect = 9; return true; }
    return false;
}
static Thing* Create(long effect)
{
    char b[32]; std::sprintf(b, "create%ld", effect); Log(b);
    return &g_things[g_nextThing++];
}
static void AttachToThing(long mode, const char* bone)
{
    char b[64]; std::sprintf(b, "attach%lx:%s", mode, bone); Log(b);
}

struct Tentacle {
    Thing* ripple;
    void OnInitialActivate()
    {
        Log("subdef");
        Log("kill");
        long effect = 0;
        if (g_hasEffects) GetEffectFromString("DRIP", &effect);
        Thing* drip = Create(effect);
        if (drip && !drip->dead) AttachToThing(0xc, "TARGET_CURSOR_POINT");
        if (g_hasEffects) GetEffectFromString("MEETS_WATER", &effect);
        ripple = Create(effect);
        if (ripple && !ripple->dead) AttachToThing(0x4a, "EFFECT_WATER");
        Log("base");
    }
};

int main()
{
    Tentacle t;
    g_hasEffects = true; g_nextThing = 0; g_log[0] = 0;
    g_things[0].dead = false; g_things[1].dead = false;
    t.OnInitialActivate();
    if (std::strcmp(g_log, "subdef;kill;create7;attachc:TARGET_CURSOR_POINT;create9;attach4a:EFFECT_WATER;base;") != 0 ||
        t.ripple != &g_things[1]) { std::printf("007f08b0_TEST FAIL\n"); return 1; }
    g_hasEffects = false; g_nextThing = 0; g_log[0] = 0;
    g_things[0].dead = true; g_things[1].dead = true;
    t.OnInitialActivate();
    if (std::strcmp(g_log, "subdef;kill;create0;create0;base;") != 0) { std::printf("007f08b0_TEST FAIL\n"); return 1; }
    std::printf("007f08b0_TEST PASS\n");
    return 0;
}
