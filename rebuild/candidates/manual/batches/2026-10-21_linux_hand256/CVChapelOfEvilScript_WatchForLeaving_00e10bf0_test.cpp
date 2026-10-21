// Behaviour test for 0x00E10BF0 (entity Main following WaspChaseWoman): scripting, follow, release.
#include <cstdio>
#include <cstring>
// Host model: logs 'F' frame, 'S' scripting attempt, 'f' follow (WaspChaseWoman at 1.0, run), 'R' release; the handle
// is scripting after a successful start; the woman dies at frame 'dieAt'; IsDone is a countdown of checks.
struct Game;
struct Thing { Game* g; bool woman; bool IsAlive() const; };
struct Game {
    char log[256]; int frames; int startOkFrom; int dieAt; int filters;
    Game() : frames(0), startOkFrom(0), dieAt(1000), filters(0) { log[0] = 0; }
    void Add(char c) { size_t n = std::strlen(log); if (n < 250) { log[n] = c; log[n + 1] = 0; } }
    void NewScriptFrame() { Add('F'); ++frames; }
    Thing GetThingWithScriptName(const char* n) { Thing t; t.g = this; t.woman = std::strcmp(n, "WaspChaseWoman") == 0; return t; }
};
bool Thing::IsAlive() const { return woman && g->frames < g->dieAt; }
struct Handle {
    Game* g; bool scripting;
    Handle(Game* game) : g(game), scripting(false) {}
    bool IsScripting() { return scripting; }
    void Release() { g->Add('R'); scripting = false; }
    void FollowThing(const Thing& t, float d, bool run) { g->Add(t.woman && d == 1.0f && run ? 'f' : '?'); }
    bool Start(int frames, int okFrom) { g->Add('S'); if (frames >= okFrom) scripting = true; return scripting; }
};
struct Script {
    Game* Game_; int doneAfter; int checks;
    Script(Game* g, int after) : Game_(g), doneAfter(after), checks(0) {}
    bool IsDone() { return ++checks > doneAfter; }
    void Main()
    {
        ++Game_->filters;
        Game_->NewScriptFrame();
        if (IsDone()) return;
        Handle handle(Game_);
        if (handle.IsScripting()) handle.Release();
        while (!handle.Start(Game_->frames, Game_->startOkFrom)) { Game_->NewScriptFrame(); if (IsDone()) return; }
        if (IsDone()) return;
        const Thing& woman = Game_->GetThingWithScriptName("WaspChaseWoman");
        handle.FollowThing(woman, 1.0f, true);
        while (!IsDone()) {
            if (!woman.IsAlive()) {
                if (IsDone()) return;
                if (handle.IsScripting()) handle.Release();
                return;
            }
            Game_->NewScriptFrame();
        }
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    { Game g; Script s(&g, 0); s.Main(); CHECK(std::strcmp(g.log, "F") == 0 && g.filters == 1); }
    // start succeeds at frame 2, woman dies at frame 4: follow, two frames, release
    { Game g; g.startOkFrom = 2; g.dieAt = 4; Script s(&g, 20); s.Main(); CHECK(std::strcmp(g.log, "FSFSfFFR") == 0); }
    // done while following: no release
    { Game g; Script s(&g, 4); s.Main(); CHECK(std::strcmp(g.log, "FSfFF") == 0); }
    if (g_fail) { std::printf("00e10bf0_TEST FAIL\n"); return 1; }
    std::printf("00e10bf0_TEST PASS\n");
    return 0;
}
