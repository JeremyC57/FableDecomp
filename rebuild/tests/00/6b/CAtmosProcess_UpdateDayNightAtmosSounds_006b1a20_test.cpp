// Behaviour test for CAtmosProcess::UpdateDayNightAtmosSounds (0x006B1A20): the same control flow over a host model.
#include <cstdio>
#include <cstring>

struct Mix { char from[16]; char to[8]; float mix; float time; };
static Mix g_mixes[32];
static int g_mixCount;
static bool g_daySounds;
static long g_themeCount;          // GetNoInstantiatedDefsOfType("SOUND_THEME")
static long g_releases;
static const char* const kThemeNames[] = { "", "FOREST", "TOWN", "CAVE", "SEA" };

struct Theme { long refs; const char* name; };
static Theme g_themes[5];

static void GetDef(long index, Theme** out)
{
    if (index <= 0) return;
    g_themes[index].refs = 1;
    g_themes[index].name = kThemeNames[index];
    *out = &g_themes[index];
}

struct AtmosProcess {
    long DayNightState;
    bool AtmosSoundsMuted;
    void MixAtmos(const char* from, const char* to, float mix, float time)
    {
        Mix& m = g_mixes[g_mixCount++];
        std::strcpy(m.from, from); std::strcpy(m.to, to); m.mix = mix; m.time = time;
    }
    void Loop(const char* on, const char* off)
    {
        for (long i = 1; i < g_themeCount; ++i) {
            Theme* theme = 0;
            GetDef(i, &theme);
            MixAtmos(theme->name, on, 1.0f, 2000.0f);
            MixAtmos(theme->name, off, 0.0f, 2000.0f);
            if (--theme->refs == 0) ++g_releases;
        }
    }
    void UpdateDayNightAtmosSounds()
    {
        if (!AtmosSoundsMuted) {
            if (DayNightState != 1 && g_daySounds) {
                Loop("DAY", "NIGHT");
                DayNightState = 1;
            } else if (DayNightState != 2 && !g_daySounds) {
                Loop("NIGHT", "DAY");
                DayNightState = 2;
            }
        }
    }
};

int main()
{
    int fails = 0;
    AtmosProcess a; a.DayNightState = 0; a.AtmosSoundsMuted = false;
    g_themeCount = 4;                                   // themes 1..3
    g_daySounds = true;
    a.UpdateDayNightAtmosSounds();
    if (a.DayNightState != 1 || g_mixCount != 6 || g_releases != 3) ++fails;
    if (std::strcmp(g_mixes[0].from, "FOREST") || std::strcmp(g_mixes[0].to, "DAY") || g_mixes[0].mix != 1.0f) ++fails;
    if (std::strcmp(g_mixes[1].to, "NIGHT") || g_mixes[1].mix != 0.0f || g_mixes[1].time != 2000.0f) ++fails;
    a.UpdateDayNightAtmosSounds();                      // already day: nothing
    if (g_mixCount != 6) ++fails;
    g_daySounds = false;
    a.UpdateDayNightAtmosSounds();                      // night: NIGHT up, DAY down
    if (a.DayNightState != 2 || g_mixCount != 12) ++fails;
    if (std::strcmp(g_mixes[6].to, "NIGHT") || g_mixes[6].mix != 1.0f || std::strcmp(g_mixes[7].to, "DAY")) ++fails;
    a.AtmosSoundsMuted = true; g_daySounds = true;
    a.UpdateDayNightAtmosSounds();                      // muted: state kept
    if (a.DayNightState != 2 || g_mixCount != 12) ++fails;
    a.AtmosSoundsMuted = false; g_themeCount = 1;      // no themes: the state still flips
    a.UpdateDayNightAtmosSounds();
    if (a.DayNightState != 1 || g_mixCount != 12) ++fails;
    if (fails) { std::printf("006b1a20_TEST FAIL %d\n", fails); return 1; }
    std::printf("006b1a20_TEST PASS\n");
    return 0;
}