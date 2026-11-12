// Behaviour test for 0x007F53B0 CTCSoundAtmosphere::FrameUpdate: a copy of the model's logic over plain test doubles.
#include <cstdio>
#include <cstring>

struct Mix { const char* theme; const char* layer; float gain; float time; };
static Mix g_mixes[16];
static int g_mixCount;
static int g_released;

struct Theme {                       // CSoundThemeDef: instantiation name + the atmos-group names of its map
    const char* name;
    const char* groups[4];
    int groupCount;
    int refs;
    bool HasAtmosGroup(const char* layer) const
    {
        for (int i = 0; i < groupCount; ++i)
            if (std::strcmp(groups[i], layer) == 0)
                return true;
        return false;
    }
};

static Theme g_themes[4];            // class index 0 is the non-instantiated def: the loop starts at 1
static long g_themeCount;
static bool g_soundManager;
static bool g_atmosProcessor;
static long g_constantFPS = 30;

struct Atmosphere {                  // CTCSoundAtmosphere
    float TimeUntilNextUpdate;
    const char* layers[4];           // PDef->SoundAtmospheres
    int layerCount;
    float GetGain(const char* layer) // the village override's contract: a gain for STREETLIFE, -1 for anything else
    {
        return std::strcmp(layer, "STREETLIFE") == 0 ? 0.25f : -1.0f;
    }
    void FrameUpdate()
    {
        if (g_soundManager && g_atmosProcessor) {
            for (int l = 0; l != layerCount; ++l) {
                long count = g_themeCount;
                for (long i = 1; i < count; ++i) {
                    Theme* theme = &g_themes[i];
                    ++theme->refs;
                    if (theme->HasAtmosGroup(layers[l])) {
                        float gain = GetGain(layers[l]);
                        if (gain != -1.0f) {
                            Mix& m = g_mixes[g_mixCount++];
                            m.theme = theme->name; m.layer = layers[l]; m.gain = gain; m.time = 0.0f;
                        }
                    }
                    if (--theme->refs == 0)
                        ++g_released;
                }
            }
        }
        TimeUntilNextUpdate -= 1.0f / g_constantFPS;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    bool ok = true;
    g_themes[0].name = "NULL";   g_themes[0].groups[0] = "STREETLIFE"; g_themes[0].groupCount = 1;
    g_themes[1].name = "TOWN";   g_themes[1].groups[0] = "DAY"; g_themes[1].groups[1] = "STREETLIFE"; g_themes[1].groupCount = 2;
    g_themes[2].name = "FOREST"; g_themes[2].groups[0] = "DAY"; g_themes[2].groupCount = 1;
    g_themes[3].name = "DOCKS";  g_themes[3].groups[0] = "STREETLIFE"; g_themes[3].groups[1] = "AMBIENT"; g_themes[3].groupCount = 2;
    g_themeCount = 4;

    Atmosphere a;
    a.TimeUntilNextUpdate = 5.0f;
    a.layers[0] = "AMBIENT"; a.layers[1] = "STREETLIFE"; a.layerCount = 2;

    // No sound manager: nothing mixed, the timer still counts down.
    g_soundManager = false; g_atmosProcessor = true;
    a.FrameUpdate();
    if (g_mixCount != 0 || !Near(a.TimeUntilNextUpdate, 5.0f - 1.0f / 30)) ok = false;

    // No atmos processor: the same.
    g_soundManager = true; g_atmosProcessor = false;
    a.FrameUpdate();
    if (g_mixCount != 0 || !Near(a.TimeUntilNextUpdate, 5.0f - 2.0f / 30)) ok = false;

    // Both: AMBIENT is in DOCKS' map but its gain is -1 (skipped); STREETLIFE mixes TOWN and DOCKS, never index 0 or FOREST.
    g_atmosProcessor = true;
    a.FrameUpdate();
    if (g_mixCount != 2) ok = false;
    else {
        if (std::strcmp(g_mixes[0].theme, "TOWN") != 0 || std::strcmp(g_mixes[0].layer, "STREETLIFE") != 0) ok = false;
        if (std::strcmp(g_mixes[1].theme, "DOCKS") != 0 || std::strcmp(g_mixes[1].layer, "STREETLIFE") != 0) ok = false;
        if (g_mixes[0].gain != 0.25f || g_mixes[1].gain != 0.25f || g_mixes[0].time != 0.0f || g_mixes[1].time != 0.0f) ok = false;
    }
    if (!Near(a.TimeUntilNextUpdate, 5.0f - 3.0f / 30)) ok = false;
    // Every theme handle taken in the loops was released again: 2 layers x 3 themes.
    if (g_released != 6 || g_themes[1].refs != 0 || g_themes[2].refs != 0 || g_themes[3].refs != 0 || g_themes[0].refs != 0) ok = false;

    // An empty layer list mixes nothing more.
    a.layerCount = 0;
    a.FrameUpdate();
    if (g_mixCount != 2 || !Near(a.TimeUntilNextUpdate, 5.0f - 4.0f / 30)) ok = false;

    if (!ok) { std::printf("007f53b0_TEST FAIL\n"); return 1; }
    std::printf("007f53b0_TEST PASS\n");
    return 0;
}