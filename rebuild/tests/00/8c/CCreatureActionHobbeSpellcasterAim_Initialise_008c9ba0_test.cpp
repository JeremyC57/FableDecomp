// Behaviour test: CCreatureAction_HobbeSpellcasterAim::Initialise (0x008C9BA0) -- base Initialise, kill a live previous
// charge effect and clear it, then attach a fresh "CHARGE_SPELL" effect to the staff.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CThing { public: bool Dead; void Kill(bool) { Dead = true; Log("kill"); } };
static CThing g_newEffect;
static const char* g_effectName;
static CThing* AttachEffectToStaff(const char* name) { g_effectName = name; Log("attach"); return &g_newEffect; }
struct CAim {
    CThing* PEffect;
    void Initialise()
    {
        Log("base");
        if (PEffect != 0 && !PEffect->Dead) { PEffect->Kill(true); PEffect = 0; }
        PEffect = AttachEffectToStaff("CHARGE_SPELL");
    }
};
int main()
{
    CThing old = { false };
    CAim a = { &old };
    a.Initialise();
    bool ok = std::strcmp(g_log, "base;kill;attach;") == 0 && old.Dead && a.PEffect == &g_newEffect
        && std::strcmp(g_effectName, "CHARGE_SPELL") == 0;
    g_log[0] = 0; CThing dead = { true }; CAim b = { &dead }; b.Initialise();
    ok = ok && std::strcmp(g_log, "base;attach;") == 0;
    g_log[0] = 0; CAim c = { 0 }; c.Initialise();
    ok = ok && std::strcmp(g_log, "base;attach;") == 0 && c.PEffect == &g_newEffect;
    if (!ok) { std::printf("008c9ba0_TEST FAIL\n"); return 1; }
    std::printf("008c9ba0_TEST PASS\n");
    return 0;
}