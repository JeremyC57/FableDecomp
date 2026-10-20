#include <cstdio>
#include <cstring>
struct Game {
    int frames; int opened; bool thunder; bool arena;
    void NewScriptFrame() { ++frames; }
    bool IsQuestCompleted(const char* q) const { return std::strcmp(q, "Q_HeroSoulsThunder") == 0 ? thunder : std::strcmp(q, "Q_HeroSoulsArena") == 0 ? arena : false; }
};
struct Gate {
    Game* G; bool done; int filters;
    void RegisterFilter() { ++filters; }
    bool IsDone() { return done; }
    void Main()
    {
        RegisterFilter();
        G->NewScriptFrame();
        if (IsDone()) return;
        bool open = G->IsQuestCompleted("Q_HeroSoulsThunder") || G->IsQuestCompleted("Q_HeroSoulsArena");
        if (open && !IsDone()) G->opened++;
    }
};
int main()
{
    Game g1 = { 0, 0, false, true }; Gate a = { &g1, false, 0 }; a.Main();
    Game g2 = { 0, 0, false, false }; Gate b = { &g2, false, 0 }; b.Main();
    Game g3 = { 0, 0, true, false }; Gate c = { &g3, true, 0 }; c.Main();
    if (g1.opened != 1 || g2.opened != 0 || g3.opened != 0 || g1.frames != 1 || a.filters != 1) { std::printf("00cdf260_TEST FAIL\n"); return 1; }
    std::printf("00cdf260_TEST PASS\n");
    return 0;
}