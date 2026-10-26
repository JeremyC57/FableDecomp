// Behaviour test for 00ed2a50 (CV_SingingStonesScript::WatchForRegionLeaving): every visit to Witchwood2 followed by
// leaving it kills the effects and empties the effect list once; frames pass until the thread terminates.
#include <cstdio>
#include <cstring>

struct Game { int frames, termAt, kills; bool loaded[64]; int effects; };
struct Script {
    Game* g;
    bool Term() { return g->frames >= g->termAt; }
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "Witchwood2") == 0 && g->loaded[g->frames]; }
    void Frame() { ++g->frames; if (IsRegionLoaded("Witchwood2")) ++g->effects; }
    void Run()
    {
        while (!Term()) {
            while (!IsRegionLoaded("Witchwood2")) { Frame(); if (Term()) return; }
            if (Term()) return;
            while (!(!IsRegionLoaded("Witchwood2"))) { Frame(); if (Term()) return; }
            if (Term()) return;
            ++g->kills; g->effects = 0;
            Frame();
        }
    }
};
int main()
{
    Game g; std::memset(&g, 0, sizeof g); g.termAt = 40;
    for (int i = 4; i < 9; ++i) g.loaded[i] = true;
    for (int i = 20; i < 22; ++i) g.loaded[i] = true;
    Script s; s.g = &g; s.Run();
    int ok = g.kills == 2 && g.effects == 0 && g.frames == 40;
    if (!ok) { std::printf("00ed2a50_TEST FAIL\n"); return 1; }
    std::printf("00ed2a50_TEST PASS\n");
    return 0;
}