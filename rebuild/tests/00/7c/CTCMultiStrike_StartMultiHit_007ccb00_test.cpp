// Behaviour test for CTCMultiStrike::StartMultiHit (0x007CCB00): hit state copied from the hit parameters, the delay in
// frames from the def's per-level seconds, and the conditional effect / weapon / rumble / combo steps.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static long g_FramesPerSecond = 15;

struct Vec { float X, Y, Z; };
struct Hit { Vec Position, Direction; int location; int weapon; };
struct MultiStrike {
    float delays[4];
    long SpellLevel, DelayTimer, NumHits; bool StrikeStarted;
    int target, location, weapon; Vec point, dir;
    bool hasEffect, hasHolder, holderWeapon, mainPlayer, hasCombo;
    void StartMultiHit(const Hit& hit, int t)
    {
        target = t; location = hit.location; weapon = hit.weapon; point = hit.Position; dir = hit.Direction;
        StrikeStarted = true; NumHits = 1;
        DelayTimer = (long)(g_FramesPerSecond * delays[SpellLevel]);
        if (hasEffect) Log("effect");
        if (hasHolder && holderWeapon) Log("weapon");
        if (mainPlayer) Log("rumble");
        if (hasCombo) Log("combo");
    }
};

int main()
{
    MultiStrike m; std::memset(&m, 0, sizeof(m));
    m.delays[1] = 0.4f; m.SpellLevel = 1; m.hasEffect = true; m.hasHolder = true; m.mainPlayer = true; m.hasCombo = true;
    Hit h = { { 1, 2, 3 }, { 0, 1, 0 }, 7, 9 };
    g_log[0] = 0; m.StartMultiHit(h, 5);
    if (m.DelayTimer != 6 || m.NumHits != 1 || !m.StrikeStarted || m.location != 7 || m.weapon != 9 || m.point.Z != 3 ||
        m.dir.Y != 1 || m.target != 5 || std::strcmp(g_log, "effect;rumble;combo;") != 0) { std::printf("007ccb00_TEST FAIL\n"); return 1; }
    m.holderWeapon = true; m.mainPlayer = false; g_log[0] = 0; m.StartMultiHit(h, 5);
    if (std::strcmp(g_log, "effect;weapon;combo;") != 0) { std::printf("007ccb00_TEST FAIL\n"); return 1; }
    std::printf("007ccb00_TEST PASS\n");
    return 0;
}