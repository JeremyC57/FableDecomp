#include <cstdio>
#include <cstring>
struct Handle { bool scripting; int releases; Handle() : scripting(false), releases(0) {} bool IsScripting() { return scripting; } void Release() { ++releases; } };
struct Game {
    int frames; int startAfter; int markers; int targets; int hero;
    void NewScriptFrame() { ++frames; }
    bool StartScriptingEntity(Handle&, int prio) { if (prio != 3) return false; return startAfter-- <= 0; }
    const int& GetHero() { return hero; }
    void MiniMapAddMarker(const char* m) { if (std::strcmp(m, "HUD_ORB_QUEST_CORE") == 0) ++markers; }
    void GiveThingBestEnemyTarget(const int& t) { if (t == 7) ++targets; }
};
struct Minion {
    Game* G; int doneAfterFrames;
    bool IsDone() { return G->frames > doneAfterFrames; }
    void Main()
    {
        G->NewScriptFrame();
        if (IsDone()) return;
        Handle handle;
        if (handle.IsScripting()) handle.Release();
        while (!G->StartScriptingEntity(handle, 3)) { G->NewScriptFrame(); if (IsDone()) return; }
        if (!IsDone()) { G->MiniMapAddMarker("HUD_ORB_QUEST_CORE"); G->GiveThingBestEnemyTarget(G->GetHero()); }
    }
};
int main()
{
    Game g1 = { 0, 2, 0, 0, 7 }; Minion a = { &g1, 10 }; a.Main();
    Game g2 = { 0, 5, 0, 0, 7 }; Minion b = { &g2, 2 }; b.Main();
    if (g1.markers != 1 || g1.targets != 1 || g1.frames != 3 || g2.markers != 0 || g2.frames != 3) { std::printf("00df4610_TEST FAIL\n"); return 1; }
    std::printf("00df4610_TEST PASS\n");
    return 0;
}