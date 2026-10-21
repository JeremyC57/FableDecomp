// Behaviour test for CUndeadRisingZombieAttacker::Main (0x00E0CFF0): retries StartScriptingEntity frame by frame, targets
// the hero, waits for the quest state 2, then removes the zombie; an early termination skips the removal.
#include <cstdio>

struct CScriptThing { int Id; };
static int g_frames, g_starts, g_startsNeeded, g_released, g_removed, g_terminateAt; static const CScriptThing* g_target;
struct CHandle { bool Scripting; CHandle() : Scripting(true) { } bool IsScripting() { return Scripting; } void Release() { Scripting = false; ++g_released; } };
struct CQuest { long State; };
struct CGameScriptInterface {
    CScriptThing Hero; CQuest* Quest;
    void NewScriptFrame() const { ++g_frames; if (g_frames == 6) Quest->State = 2; }
    bool StartScriptingEntity(const CScriptThing&, CHandle&, int priority) { ++g_starts; return priority == 3 && g_starts >= g_startsNeeded; }
    CScriptThing& GetHero() { return Hero; }
    void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing& t) const { g_target = &t; }
    void RemoveThing(const CScriptThing&, bool a, bool b) const { if (a && b) ++g_removed; }
};
struct CZombie {
    CGameScriptInterface* Game; CScriptThing Me; CQuest* ParentClass;
    bool IsActiveThreadTerminating() { return g_terminateAt >= 0 && g_frames >= g_terminateAt; }
    void Main()
    {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        CHandle handle;
        if (handle.IsScripting()) handle.Release();
        while (!Game->StartScriptingEntity(Me, handle, 3)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        while (ParentClass->State != 2) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(Me, true, true);
    }
};

int main()
{
    CQuest q = { 0 };
    CGameScriptInterface gsi; gsi.Quest = &q;
    CZombie z = { &gsi, { 1 }, &q };
    g_startsNeeded = 3; g_terminateAt = -1;
    z.Main();
    bool ok = g_starts == 3 && g_released == 1 && g_target == &gsi.Hero && g_removed == 1 && g_frames == 6;
    q.State = 0; g_frames = 0; g_starts = 0; g_removed = 0; g_terminateAt = 2;
    z.Main();
    ok = ok && g_removed == 0;
    if (!ok) { std::printf("00e0cff0_TEST FAIL\n"); return 1; }
    std::printf("00e0cff0_TEST PASS\n");
    return 0;
}
