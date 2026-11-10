// Behaviour test for CAtmosProcess::Update (0x006B2260): the same control flow over a host model.
#include <cstdio>
#include <cstring>

struct Vec { float X, Y, Z; };
struct AtmosPlayer { long defIndex; Vec pos; int posSets; };
struct Marker { long defIndex; Vec created; bool hasPlayer; AtmosPlayer player; };

static bool g_soundManager;
static float g_rain;
static Vec g_camera;
static Marker g_markers[4];
static int g_markerCount;
static int g_mixCount; static float g_lastMix; static char g_lastFrom[24]; static char g_lastTo[8];
static int g_insideUpdates, g_dayNightUpdates;

static long IndexOf(const char* name) { return std::strcmp(name, "MARKER_POSITIONAL_ATMOS") == 0 ? 17 : 42; }
static Marker* CreateMarker(long index, const Vec& pos)
{
    Marker* m = &g_markers[g_markerCount++];
    m->defIndex = index; m->created = pos; m->hasPlayer = true; m->player.defIndex = 0; m->player.posSets = 0;
    return m;
}

struct AtmosProcess {
    bool RainCurrentlyPlaying;
    Marker* PRainPlayer;
    void MixAtmos(const char* from, const char* to, float mix, float)
    {
        ++g_mixCount; g_lastMix = mix; std::strcpy(g_lastFrom, from); std::strcpy(g_lastTo, to);
    }
    void Update()
    {
        if (g_soundManager) {
            if (g_rain > 0.0001f) {
                if (PRainPlayer == 0) {
                    long index = IndexOf("MARKER_POSITIONAL_ATMOS");
                    PRainPlayer = CreateMarker(index, g_camera);
                    if (PRainPlayer->hasPlayer) {
                        long theme = IndexOf("SOUND_THEME_RAIN");
                        PRainPlayer->player.defIndex = theme;
                    }
                }
                if (!RainCurrentlyPlaying) {
                    MixAtmos("SOUND_THEME_RAIN", "RAIN", 1.0f, 2000.0f);
                    RainCurrentlyPlaying = true;
                }
                if (PRainPlayer->hasPlayer) {
                    Vec p = g_camera; p.Z += 1.0f;
                    PRainPlayer->player.pos = p; ++PRainPlayer->player.posSets;
                }
            } else if (RainCurrentlyPlaying) {
                MixAtmos("SOUND_THEME_RAIN", "RAIN", 0.0f, 2000.0f);
                RainCurrentlyPlaying = false;
            }
            ++g_insideUpdates; ++g_dayNightUpdates;
        }
    }
};

int main()
{
    int fails = 0;
    AtmosProcess a; a.RainCurrentlyPlaying = false; a.PRainPlayer = 0;
    a.Update();                                         // no sound manager: nothing
    if (g_insideUpdates || g_markerCount) ++fails;
    g_soundManager = true; g_rain = 0.0f;
    a.Update();                                         // dry: only the sub-updates
    if (g_insideUpdates != 1 || g_dayNightUpdates != 1 || g_mixCount || g_markerCount) ++fails;
    g_rain = 0.5f; g_camera.X = 1; g_camera.Y = 2; g_camera.Z = 3;
    a.Update();                                         // rain starts: marker, theme, fade in, position
    if (g_markerCount != 1 || a.PRainPlayer->defIndex != 17 || a.PRainPlayer->player.defIndex != 42) ++fails;
    if (g_mixCount != 1 || g_lastMix != 1.0f || std::strcmp(g_lastFrom, "SOUND_THEME_RAIN") || std::strcmp(g_lastTo, "RAIN")) ++fails;
    if (a.PRainPlayer->player.pos.Z != 4.0f || a.PRainPlayer->player.pos.X != 1.0f || !a.RainCurrentlyPlaying) ++fails;
    g_camera.Z = 10;
    a.Update();                                         // still raining: no new marker or fade, position follows
    if (g_markerCount != 1 || g_mixCount != 1 || a.PRainPlayer->player.pos.Z != 11.0f || a.PRainPlayer->player.posSets != 2) ++fails;
    g_rain = 0.00001f;
    a.Update();                                         // rain stops: fade out once
    if (g_mixCount != 2 || g_lastMix != 0.0f || a.RainCurrentlyPlaying) ++fails;
    a.Update();
    if (g_mixCount != 2 || g_insideUpdates != 5) ++fails;
    if (fails) { std::printf("006b2260_TEST FAIL %d\n", fails); return 1; }
    std::printf("006b2260_TEST PASS\n");
    return 0;
}