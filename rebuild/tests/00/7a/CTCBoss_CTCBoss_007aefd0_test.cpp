// Behaviour test for CTCBoss::CTCBoss (0x007AEFD0): zeroed counters, both flags set, boss sub-def fetched, phase start copied
// from the owner's position, effects cleared.
#include <cstdio>
struct Vec3 { float X, Y, Z; };
static int g_subdef, g_clear;
struct BossDef { int id; };
static BossDef g_def;
struct Thing { Vec3 Pos; const Vec3& GetPosition() const { return Pos; } };
struct Boss {
    Thing* ParentThing; BossDef* PBossDef; long GenericCounter, StrikesSinceHit, currentBossPhase; bool BCountStrikes, EnablePhaseChanges;
    Vec3 PhaseStartPos; void* helpers[3]; void* effects[3];
    void GetSubDef(BossDef** out) { ++g_subdef; *out = &g_def; }
    void ClearEffects() { ++g_clear; }
    Boss(Thing* owner) : ParentThing(owner), PBossDef(0)
    {
        for (int i = 0; i < 3; ++i) { helpers[i] = 0; effects[i] = 0; }
        currentBossPhase = 0; StrikesSinceHit = 0; BCountStrikes = true; GenericCounter = 0; EnablePhaseChanges = true;
        GetSubDef(&PBossDef);
        PhaseStartPos = ParentThing->GetPosition();
        ClearEffects();
    }
};
int main()
{
    Thing t; t.Pos.X = 4; t.Pos.Y = 5; t.Pos.Z = 6;
    Boss b(&t);
    if (b.PBossDef != &g_def || b.GenericCounter || b.StrikesSinceHit || b.currentBossPhase || !b.BCountStrikes || !b.EnablePhaseChanges ||
        b.PhaseStartPos.X != 4 || b.PhaseStartPos.Z != 6 || g_subdef != 1 || g_clear != 1 || b.helpers[0] || b.effects[2]) { std::printf("007aefd0_TEST FAIL\n"); return 1; }
    std::printf("007aefd0_TEST PASS\n");
    return 0;
}