// Behaviour test for 0x006E75C0 (CGameScriptInterface::WorldUpdate: once a second, outside movies/cutscenes, count timers down to 0).
#include <cstdio>
#include <map>

static long g_frame = 0;
static long g_framesPerSecond = 30;
long GFGetCurrentFrame() { return g_frame; }

struct ScriptInterface {
    bool movie, cutscene; int checks;
    std::map<long, long> ScriptTimers;
    bool IsInMovieSequence() { ++checks; return movie; }
    bool IsInCutscene() { ++checks; return cutscene; }
    void WorldUpdate(bool active)
    {
        if (active && !IsInMovieSequence() && !IsInCutscene()) {
            long fps = g_framesPerSecond;
            if (GFGetCurrentFrame() % fps == 0)
                for (std::map<long, long>::iterator it = ScriptTimers.begin(); it != ScriptTimers.end(); ++it)
                    if (it->second > 0) --it->second;
        }
    }
};
#define FAIL { std::printf("006e75c0_TEST FAIL\n"); return 1; }
int main()
{
    ScriptInterface si; si.movie = si.cutscene = false; si.checks = 0;
    si.ScriptTimers[1] = 2; si.ScriptTimers[5] = 0; si.ScriptTimers[9] = 1;
    si.WorldUpdate(false);
    if (si.checks != 0 || si.ScriptTimers[1] != 2) FAIL;
    g_frame = 60; si.WorldUpdate(true);
    if (si.ScriptTimers[1] != 1 || si.ScriptTimers[5] != 0 || si.ScriptTimers[9] != 0) FAIL;
    g_frame = 61; si.WorldUpdate(true);
    if (si.ScriptTimers[1] != 1) FAIL;
    g_frame = 90; si.movie = true; si.WorldUpdate(true);
    if (si.ScriptTimers[1] != 1) FAIL;
    si.movie = false; si.cutscene = true; si.WorldUpdate(true);
    if (si.ScriptTimers[1] != 1) FAIL;
    si.cutscene = false; si.WorldUpdate(true); si.WorldUpdate(true);
    if (si.ScriptTimers[1] != 0 || si.ScriptTimers[9] != 0 || si.ScriptTimers[5] != 0) FAIL;
    std::printf("006e75c0_TEST PASS\n");
    return 0;
}