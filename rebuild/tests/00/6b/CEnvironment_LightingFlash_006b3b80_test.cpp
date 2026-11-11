// Behaviour test for 0x006B3B80 CEnvironment::LightingFlash: the row's body over stubs (scripted clock and random rolls, recorded
// theme transitions and thunder), covering the no-storm exit, a flash with thunder, the indoor hero, the fade-out and the decay.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static void check(bool b) { if (!b) { std::puts("006b3b80_TEST FAIL"); std::exit(1); } }
static bool near(double a, double b) { double d = a - b; return d < 1e-4 && d > -1e-4; }
static double g_time; static float g_roll[4]; static int g_rollPos; static unsigned long* g_seedSeen[4];
static double GFGetTime() { return g_time; }
static float GFFloatRandom(float range, unsigned long* seed) { g_seedSeen[g_rollPos] = seed; float r = g_roll[g_rollPos++]; check(r <= range || range <= 0.0f); return r; }
class C3DVector { public: float X, Y, Z; };
static int g_inCount, g_outCount, g_soundCount; static long g_inTheme; static float g_inDur, g_outDur; static C3DVector g_soundPos;
static char g_soundName[64]; static unsigned long g_soundFader;
class CSoundInit { public: unsigned long GainFaderBits; CSoundInit() : GainFaderBits(0) { } };
class CCharString { public: const char* s; CCharString(const char* t) : s(t) { } };
class CASoundBank { public: };
class CThingSoundEmitter { public: bool dead; bool IsDead() const { return dead; }
    unsigned long PlaySoundEvent(CASoundBank&, const CCharString& n, bool loop, CSoundInit& init, bool attached)
    { check(!loop && !attached); ++g_soundCount; std::strcpy(g_soundName, n.s); g_soundFader = init.GainFaderBits; return 1; } };
static CThingSoundEmitter g_soundEmitterThing;
static CThingSoundEmitter* CreateEmitter(const C3DVector& p) { g_soundPos = p; return &g_soundEmitterThing; }
template<class T> bool IS_THING_ALIVE(const T& t) { return t != 0 && !t->IsDead(); }
class CThing { public: bool dead; bool inside; bool hasTracker; C3DVector pos; bool IsDead() const { return dead; } const C3DVector& GetPos() const { return pos; } };
static CThing* g_hero; static bool g_soundManager; static unsigned long g_worldSeed; static CASoundBank g_bank;
class CEnvironment { public:
    double LightningStartTime; float LightningFadeInDuration, LightningFadeOutDuration, LightningFlashDuration, LightningRainThreshold;
    long LightningTheme; float LightningCurrentFlashChance; unsigned long LightningRandomSeed; float RainStrength, LightningFrequency;
    float GetRainStrength() const { return RainStrength; }
    void TransitionToThemeAllChannels(long p, long t, float d) { check(p == 4); ++g_inCount; g_inTheme = t; g_inDur = d; }
    void TransitionToDefaultThemeAllChannels(long p, float d) { check(p == 4); ++g_outCount; g_outDur = d; }
    void LightingFlash();
};
void CEnvironment::LightingFlash()
{
    if (!(LightningFrequency > 0.0f || LightningStartTime >= 0.0))
        return;
    float rain_strength = GetRainStrength();
    double time = GFGetTime();
    static double last_flash_update_time = time;
    if (rain_strength > LightningRainThreshold && LightningStartTime < 0.0
        && GFFloatRandom(LightningCurrentFlashChance, &LightningRandomSeed) < 1.0f) {
        LightningCurrentFlashChance += LightningFrequency;
        TransitionToThemeAllChannels(4, LightningTheme, LightningFadeInDuration);
        LightningStartTime = time;
        if (g_soundManager) {
            CThing* hero = g_hero;
            if (hero && !hero->IsDead() && hero->hasTracker && !hero->inside) {
                CSoundInit init;
                init.GainFaderBits = 0x8080;
                C3DVector pos = hero->GetPos();
                pos.X += GFFloatRandom(20.0f, &g_worldSeed) - 10.0f;
                pos.Y += GFFloatRandom(20.0f, &g_worldSeed) - 10.0f;
                pos.Z += 30.0f;
                CThingSoundEmitter* emitter = CreateEmitter(pos);
                if (IS_THING_ALIVE(emitter)) {
                    CCharString sound_name("CS_LIGHTNING_THUNDER");
                    emitter->PlaySoundEvent(g_bank, sound_name, false, init, false);
                }
            }
        }
    } else if (LightningStartTime >= 0.0 && time - LightningStartTime > LightningFlashDuration) {
        TransitionToDefaultThemeAllChannels(4, LightningFadeOutDuration);
        LightningStartTime = -1.0;
    } else {
        double chance = LightningCurrentFlashChance - (time - last_flash_update_time);
        if (0.0 > chance)
            chance = 0.0;
        LightningCurrentFlashChance = (float)chance;
    }
    last_flash_update_time = time;
}
int main()
{
    CEnvironment e; std::memset(&e, 0, sizeof e);
    e.LightningStartTime = -1.0; e.LightningFadeInDuration = 0.25f; e.LightningFadeOutDuration = 0.5f; e.LightningFlashDuration = 0.2f;
    e.LightningRainThreshold = 0.5f; e.LightningTheme = 77; e.LightningCurrentFlashChance = 8.0f; e.RainStrength = 0.75f;
    CThing hero; hero.dead = false; hero.inside = false; hero.hasTracker = true; hero.pos.X = 100.0f; hero.pos.Y = 200.0f; hero.pos.Z = 5.0f;
    g_hero = &hero; g_soundManager = true; g_soundEmitterThing.dead = false;
    // no lightning frequency and no flash running: nothing happens (not even the clock)
    e.LightningFrequency = 0.0f; g_time = 1.0; g_rollPos = 0;
    e.LightingFlash(); check(g_rollPos == 0 && g_inCount == 0 && e.LightningCurrentFlashChance == 8.0f);
    // first call: the roll (2.0) misses -> chance decays by 0 (static set to now)
    e.LightningFrequency = 1.5f; g_time = 10.0; g_roll[0] = 2.0f; g_rollPos = 0;
    e.LightingFlash(); check(g_rollPos == 1 && g_seedSeen[0] == &e.LightningRandomSeed && g_inCount == 0 && near(e.LightningCurrentFlashChance, 8.0));
    // 3 seconds later the roll misses again -> chance 8 - 3 = 5
    g_time = 13.0; g_roll[0] = 4.0f; g_rollPos = 0;
    e.LightingFlash(); check(near(e.LightningCurrentFlashChance, 5.0));
    // a flash: chance grows by the frequency, theme in, thunder 30 above the hero, offset by roll - 10
    g_time = 14.0; g_roll[0] = 0.5f; g_roll[1] = 13.0f; g_roll[2] = 4.0f; g_rollPos = 0;
    e.LightingFlash();
    check(g_rollPos == 3 && g_seedSeen[1] == &g_worldSeed && near(e.LightningCurrentFlashChance, 6.5) && g_inCount == 1);
    check(g_inTheme == 77 && g_inDur == 0.25f && e.LightningStartTime == 14.0 && g_soundCount == 1);
    check(near(g_soundPos.X, 103.0) && near(g_soundPos.Y, 194.0) && near(g_soundPos.Z, 35.0));
    check(std::strcmp(g_soundName, "CS_LIGHTNING_THUNDER") == 0 && g_soundFader == 0x8080);
    // still flashing, inside the duration: decay branch (14.1 - 14.0)
    g_time = 14.1; g_rollPos = 0;
    e.LightingFlash(); check(g_rollPos == 0 && g_outCount == 0 && near(e.LightningCurrentFlashChance, 6.4));
    // past the duration: fade out, start time reset
    g_time = 14.5;
    e.LightingFlash(); check(g_outCount == 1 && g_outDur == 0.5f && e.LightningStartTime == -1.0);
    // big gap: the chance clamps at zero
    g_time = 100.0; g_roll[0] = 3.0f; g_rollPos = 0;
    e.LightingFlash(); check(e.LightningCurrentFlashChance == 0.0f);
    // flash with the hero inside a volume: no thunder; dry weather: no roll
    hero.inside = true; g_time = 101.0; g_roll[0] = 0.0f; g_rollPos = 0;
    e.LightingFlash(); check(g_inCount == 2 && g_soundCount == 1 && g_rollPos == 1);
    e.LightningStartTime = -1.0; e.RainStrength = 0.1f; g_rollPos = 0; g_time = 102.0;
    e.LightingFlash(); check(g_rollPos == 0 && g_inCount == 2);
    std::puts("006b3b80_TEST PASS");
    return 0;
}