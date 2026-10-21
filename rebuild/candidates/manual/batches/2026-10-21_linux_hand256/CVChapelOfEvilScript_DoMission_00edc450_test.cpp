// Behaviour test for 0x00EDC450 (DoMission, the Witchwood3 copy): wait for the region, spawn the watcher, end.
#include <cstdio>
#include <cstring>
#define REGION "Witchwood3"
// Host model: logs 'F' frame, 'W' WatchForLeaving spawned (thread name + "" region), 'q' quest info hidden,
// 'E' EndMission run; the region loads at frame 'loadAt'; terminating is a countdown of checks.
struct Game {
    char log[256]; int frames; int loadAt;
    Game() : frames(0), loadAt(0) { log[0] = 0; }
    void Add(char c) { size_t n = std::strlen(log); if (n < 250) { log[n] = c; log[n + 1] = 0; } }
    void NewScriptFrame() { Add('F'); ++frames; }
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, REGION) == 0 && frames >= loadAt; }
    void DisplayQuestInfo(bool show) { Add(show ? '?' : 'q'); }
};
struct Script;
struct Spawned { const char* name; Script* cls; void (Script::*func)(); };
struct Script {
    Game* GSI; int termAfter; int checks; Spawned* last;
    Script(Game* g, int after) : GSI(g), termAfter(after), checks(0), last(0) {}
    ~Script() { delete last; }
    bool IsActiveThreadTerminating() { return ++checks > termAfter; }
    void AddSpawnedFunction(Spawned* f, const char* region)
    {
        GSI->Add(std::strcmp(f->name, "WatchForLeaving") == 0 && region[0] == 0 && f->cls == this ? 'W' : '?');
        delete last; last = f;
    }
    void WatchForLeaving() {}
    void EndMission() { GSI->Add('E'); }
    void DoMission()
    {
        while (!GSI->IsRegionLoaded(REGION)) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Spawned* s = new Spawned; s->name = "WatchForLeaving"; s->cls = this; s->func = &Script::WatchForLeaving;
        AddSpawnedFunction(s, "");
        GSI->DisplayQuestInfo(false);
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        EndMission();
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    { Game g; Script s(&g, 5); s.DoMission(); CHECK(std::strcmp(g.log, "WqFE") == 0); CHECK(s.last && s.last->func == &Script::WatchForLeaving); }
    { Game g; g.loadAt = 2; Script s(&g, 5); s.DoMission(); CHECK(std::strcmp(g.log, "FFWqFE") == 0); }
    { Game g; g.loadAt = 9; Script s(&g, 2); s.DoMission(); CHECK(std::strcmp(g.log, "FFF") == 0 && !s.last); }
    { Game g; Script s(&g, 1); s.DoMission(); CHECK(std::strcmp(g.log, "WqF") == 0); }
    if (g_fail) { std::printf("00edc450_TEST FAIL\n"); return 1; }
    std::printf("00edc450_TEST PASS\n");
    return 0;
}
