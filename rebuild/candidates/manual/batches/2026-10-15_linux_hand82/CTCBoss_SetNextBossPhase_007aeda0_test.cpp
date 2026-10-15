// Behaviour test for 007aeda0 CTCBoss::SetNextBossPhase: only when phase changes are enabled or forced is the phase stored and the start position captured.
#include <cstdio>

struct C3DVector { float X, Y, Z; };

struct CThing
{
    C3DVector m_pos;
    const C3DVector& GetPosition() const { return m_pos; }
};

struct CTCBoss
{
    CThing*   ParentThing;
    long      currentBossPhase;
    bool      EnablePhaseChanges;
    C3DVector PhaseStartPos;

    void SetNextBossPhase(long phase, bool force)
    {
        if (EnablePhaseChanges || force)
        {
            currentBossPhase = phase;
            PhaseStartPos = ParentThing->GetPosition();
        }
    }
};

int main()
{
    CThing thing; thing.m_pos.X = 1.0f; thing.m_pos.Y = 2.0f; thing.m_pos.Z = 3.0f;
    CTCBoss boss; boss.ParentThing = &thing; boss.currentBossPhase = 0; boss.EnablePhaseChanges = false;
    boss.PhaseStartPos.X = boss.PhaseStartPos.Y = boss.PhaseStartPos.Z = 0.0f;
    boss.SetNextBossPhase(2, false);
    if (boss.currentBossPhase != 0 || boss.PhaseStartPos.X != 0.0f) { std::printf("007aeda0_TEST FAIL\n"); return 1; }
    boss.SetNextBossPhase(2, true);
    if (boss.currentBossPhase != 2 || boss.PhaseStartPos.Z != 3.0f) { std::printf("007aeda0_TEST FAIL\n"); return 1; }
    boss.EnablePhaseChanges = true; thing.m_pos.Y = 5.0f;
    boss.SetNextBossPhase(3, false);
    if (boss.currentBossPhase != 3 || boss.PhaseStartPos.Y != 5.0f) { std::printf("007aeda0_TEST FAIL\n"); return 1; }
    std::printf("007aeda0_TEST PASS\n");
    return 0;
}
