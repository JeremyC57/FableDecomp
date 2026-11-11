// Behaviour test for CAtmosProcess::SetAtmosSoundsAsMuted (0x006B1D30): the same control flow over a host model.
#include <cstdio>
#include <cstring>

struct Mix { char from[16]; char to[8]; float mix; float time; };
static Mix g_mixes[32];
static int g_mixCount;
static long g_themeCount;          // GetNoInstantiatedDefsOfType("SOUND_THEME")
static long g_releases;
static int g_dayNightUpdates;
static bool g_soundManager = true; // PSoundManager present
static const char* const kThemeNames[] = { "", "FOREST", "TOWN", "CAVE" };

struct Theme { long refs; const char* name; };
static Theme g_themes[4];

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
        if (!g_soundManager) return;
        Mix& m = g_mixes[g_mixCount++];
        std::strcpy(m.from, from); std::strcpy(m.to, to); m.mix = mix; m.time = time;
    }
    void UpdateDayNightAtmosSounds() { ++g_dayNightUpdates; }
    void SetAtmosSoundsAsMuted(bool muted, float time)
    {
        if (muted) {
            for (long i = 1; i < g_themeCount; ++i) {
                Theme* theme = 0;
                GetDef(i, &theme);
                MixAtmos(theme->name, "DAY", 0.0f, time);
                MixAtmos(theme->name, "NIGHT", 0.0f, time);
                if (--theme->refs == 0) ++g_releases;
            }
            DayNightState = 0;
        } else {
            UpdateDayNightAtmosSounds();
        }
        AtmosSoundsMuted = muted;
    }
};

int main()
{
    int fails = 0;
    AtmosProcess a; a.DayNightState = 1; a.AtmosSoundsMuted = false;
    g_themeCount = 4;                                   // themes 1..3
    a.SetAtmosSoundsAsMuted(true, 500.0f);
    if (!a.AtmosSoundsMuted || a.DayNightState != 0 || g_mixCount != 6 || g_releases != 3 || g_dayNightUpdates != 0) ++fails;
    if (std::strcmp(g_mixes[0].from, "FOREST") || std::strcmp(g_mixes[0].to, "DAY") || g_mixes[0].mix != 0.0f || g_mixes[0].time != 500.0f) ++fails;
    if (std::strcmp(g_mixes[1].to, "NIGHT") || std::strcmp(g_mixes[5].from, "CAVE")) ++fails;
    a.SetAtmosSoundsAsMuted(false, 500.0f);            // unmute: the day/night update runs, the state is untouched
    if (a.AtmosSoundsMuted || g_dayNightUpdates != 1 || g_mixCount != 6) ++fails;
    g_soundManager = false; a.DayNightState = 2;        // no sound manager: nothing mixed, the state still resets
    a.SetAtmosSoundsAsMuted(true, 0.0f);
    if (!a.AtmosSoundsMuted || a.DayNightState != 0 || g_mixCount != 6 || g_releases != 6) ++fails;
    if (fails) { std::printf("006b1d30_TEST FAIL %d\n", fails); return 1; }
    std::printf("006b1d30_TEST PASS\n");
    return 0;
}