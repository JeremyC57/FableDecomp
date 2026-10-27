#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (interface calls are logged, not executed).
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void LogI(const char* s, int v) { char b[128]; std::sprintf(b, "%s%d", s, v); Log(b); }
static void LogS(const char* s, const char* v) { char b[256]; std::sprintf(b, "%s %s", s, v); Log(b); }
static int g_termAfter = 1000;   // IsActiveThreadTerminating() turns true after this many calls
static int g_termCalls = 0;
static bool Term() { return ++g_termCalls > g_termAfter; }
static void Reset(int termAfter) { g_log[0] = 0; g_termCalls = 0; g_termAfter = termAfter; }
static int g_loadAt = 2, g_polls = 0;
struct Str { char t[32]; Str() { t[0] = 0; } };
struct Game {
    void NewScriptFrame() { Log("frame"); }
    bool MsgOnRegionLoaded(Str& r) { ++g_polls; if (g_polls % 3 == g_loadAt) { std::sprintf(r.t, "R%d", g_polls); return true; } return false; }
};
struct Script { Game* G; Str PreviousLoadedRegion, CurrentLoadedRegion; bool RegionChangeDetected;
    bool IsActiveThreadTerminating() { return Term(); }
    void WatchForRegionLoads()
    {
        Str region;
        while (!IsActiveThreadTerminating()) {
            while (!G->MsgOnRegionLoaded(region)) {
                G->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            PreviousLoadedRegion = CurrentLoadedRegion;
            CurrentLoadedRegion = region;
            RegionChangeDetected = true;
            G->NewScriptFrame();
        }
    }
};
int main()
{
    bool ok = true;
    Game g; Script s; s.G = &g; s.RegionChangeDetected = false; std::strcpy(s.CurrentLoadedRegion.t, "Start");
    Reset(9); s.WatchForRegionLoads();
    ok = s.RegionChangeDetected && std::strcmp(s.PreviousLoadedRegion.t, "R2") == 0 && std::strcmp(s.CurrentLoadedRegion.t, "R5") == 0;
    Script t; t.G = &g; t.RegionChangeDetected = false;
    Reset(0); t.WatchForRegionLoads();
    ok = ok && !t.RegionChangeDetected && g_log[0] == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00e1e470_TEST FAIL\n"); return 1; }
    std::printf("00e1e470_TEST PASS\n");
    return 0;
}