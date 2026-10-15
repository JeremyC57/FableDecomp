// Behaviour test for 0x00D83DC0: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model of the summoner-to-kill script: frame updates through the interface, a parent flag set after a few frames, IsDone.
struct Thing { int id; };
struct Parent { bool QuestStartScreened; };
struct Game {
    int frames, setAt, removed, doneAt; Parent* parent; bool removeA, removeB;
    void Update() { ++frames; if (frames == setAt) parent->QuestStartScreened = true; }
    void RemoveThing(const Thing&, bool a, bool b) { ++removed; removeA = a; removeB = b; }
};
struct Script {
    Game* G; Thing Me; Parent* ParentClass; int filters;
    bool IsDone() { return G->doneAt && G->frames >= G->doneAt; }
    void RegisterFilter() { ++filters; }
    void Main()
    {
        RegisterFilter();
        G->Update();
        if (IsDone()) return;
        while (!ParentClass->QuestStartScreened) { G->Update(); if (IsDone()) return; }
        if (IsDone()) return;
        G->RemoveThing(Me, false, true);
    }
};
static void Run()
{
    Parent p; p.QuestStartScreened = false;
    Game g; g.frames = 0; g.setAt = 4; g.removed = 0; g.doneAt = 0; g.parent = &p;
    Script s; s.G = &g; s.ParentClass = &p; s.filters = 0; s.Me.id = 1;
    s.Main();
    CHECK(s.filters == 1 && g.frames == 4 && g.removed == 1 && !g.removeA && g.removeB);
    p.QuestStartScreened = false; g.frames = 0; g.setAt = 10; g.removed = 0; g.doneAt = 3;
    s.Main();
    CHECK(g.frames == 3 && g.removed == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00d83dc0_TEST FAIL\n"); return 1; }
    std::printf("00d83dc0_TEST PASS\n");
    return 0;
}