#include <cstdio>
#include <cstring>
struct CScriptThing { int id; };
struct Filter { CScriptThing m_thing; Filter(const CScriptThing& t) : m_thing(t) { } };
struct Game {
    int updates, opened; const char* activeQuest;
    void Update() { ++updates; }
    bool IsQuestActive(const char* q) const { return activeQuest && std::strcmp(q, activeQuest) == 0; }
    void OpenDoor(const CScriptThing& d) { opened = d.id; }
};
struct Gates {
    Game* G; CScriptThing Me; int registered; int doneAfter; int doneCalls;
    void RegisterFilter(Filter& f) { registered = f.m_thing.id; }
    bool IsDone() { return ++doneCalls > doneAfter; }
    void Main()
    {
        { Filter filter(Me); RegisterFilter(filter); }
        G->Update();
        if (!IsDone()) {
            bool raidInactive;
            { const char* quest = "Q_OrchardFarmRaid"; raidInactive = G->IsQuestActive(quest) ? false : true; }
            if (raidInactive && !IsDone()) G->OpenDoor(Me);
        }
    }
};
int main()
{
    Game g; g.updates = 0; g.opened = 0; g.activeQuest = 0;
    Gates s; s.G = &g; s.Me.id = 12; s.registered = 0; s.doneAfter = 10; s.doneCalls = 0;
    s.Main();
    bool fail = g.opened != 12 || s.registered != 12 || g.updates != 1;
    g.opened = 0; g.activeQuest = "Q_OrchardFarmRaid"; s.doneCalls = 0; s.Main(); if (g.opened) fail = true;
    g.activeQuest = 0; s.doneAfter = 0; s.doneCalls = 0; s.Main(); if (g.opened) fail = true;
    if (fail) { std::printf("00cdf080_TEST FAIL\n"); return 1; }
    std::printf("00cdf080_TEST PASS\n");
    return 0;
}
