// Behaviour test for CDragon::Main (0x00D25A00): enemies spawn on each take-off, the dragon state steps down through
// the health thresholds and copies the def's target counts, and the loop ends at DS_DEAD.
#include <cstdio>

class CScriptThing { public: int id; };
static int g_frames, g_terminateAt, g_spawns;
static float g_health[64]; static bool g_flying[64];
class CScriptDef { public: long MinMed, MinLow, MinVLow, SumMed, SumLow, SumVLow; };
static CScriptDef g_def = { 5, 7, 9, 1, 2, 3 };
struct GameCalls {
    void NewScriptFrame() const { ++g_frames; }
    float GetHealth(const CScriptThing&) const { return g_health[g_frames]; }
    bool IsCreatureFlying(const CScriptThing&) const { return g_flying[g_frames]; }
};
struct CQ_DragonBossFightScript {
    long DragonState, TargetNumMinions, TargetNumSummoners;
    bool IsActiveThreadTerminating() { return g_frames >= g_terminateAt; }
    void SpawnEnemies() { ++g_spawns; if (g_frames == 9) DragonState = 4; }
};
struct CDragon {
    GameCalls* Game; CScriptThing Me; CQ_DragonBossFightScript* ParentClass;
    float MediumHealth, LowHealth, VeryLowHealth;
    bool IsActiveThreadTerminating() { return g_frames >= g_terminateAt; }
    void Main()
    {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        bool flying = false;
        while (ParentClass->DragonState != 4) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (!flying) {
                if (IsActiveThreadTerminating()) return;
                if (Game->IsCreatureFlying(Me)) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass->SpawnEnemies();
                    flying = true;
                }
            } else {
                if (IsActiveThreadTerminating()) return;
                if (!Game->IsCreatureFlying(Me)) {
                    if (IsActiveThreadTerminating()) return;
                    flying = false;
                }
            }
            if (ParentClass->DragonState == 0 && Game->GetHealth(Me) <= MediumHealth) {
                if (IsActiveThreadTerminating()) return;
                CQ_DragonBossFightScript* p = ParentClass;
                p->DragonState = 1;
                if (!p->IsActiveThreadTerminating()) { p->TargetNumMinions = g_def.MinMed; p->TargetNumSummoners = g_def.SumMed; }
            } else if (ParentClass->DragonState == 1 && Game->GetHealth(Me) <= LowHealth) {
                if (IsActiveThreadTerminating()) return;
                CQ_DragonBossFightScript* p = ParentClass;
                p->DragonState = 2;
                if (!p->IsActiveThreadTerminating()) { p->TargetNumMinions = g_def.MinLow; p->TargetNumSummoners = g_def.SumLow; }
            } else if (ParentClass->DragonState == 2 && Game->GetHealth(Me) <= VeryLowHealth) {
                if (IsActiveThreadTerminating()) return;
                CQ_DragonBossFightScript* p = ParentClass;
                p->DragonState = 3;
                if (!p->IsActiveThreadTerminating()) { p->TargetNumMinions = g_def.MinVLow; p->TargetNumSummoners = g_def.SumVLow; }
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
};

int main()
{
    int i;
    for (i = 0; i < 64; ++i) { g_health[i] = 100.0f; g_flying[i] = false; }
    // frames: take off at 3, land at 5, take off at 9 (state -> dead from SpawnEnemies)
    g_flying[3] = g_flying[4] = true; g_flying[9] = true;
    g_health[4] = 70.0f; g_health[5] = 70.0f; g_health[6] = 40.0f; g_health[7] = 10.0f;
    GameCalls game; CQ_DragonBossFightScript parent = { 0, 0, 0 };
    CDragon d; d.Game = &game; d.ParentClass = &parent;
    d.MediumHealth = 75.0f; d.LowHealth = 50.0f; d.VeryLowHealth = 25.0f;
    g_terminateAt = 100;
    d.Main();
    bool ok = g_spawns == 2 && parent.DragonState == 4 && parent.TargetNumMinions == 9 && parent.TargetNumSummoners == 3 && g_frames == 9;
    g_frames = 0; g_terminateAt = 1; parent.DragonState = 0; g_spawns = 0;
    d.Main();
    ok = ok && g_spawns == 0 && parent.DragonState == 0;
    if (!ok) { std::printf("00d25a00_TEST FAIL\n"); return 1; }
    std::printf("00d25a00_TEST PASS\n");
    return 0;
}