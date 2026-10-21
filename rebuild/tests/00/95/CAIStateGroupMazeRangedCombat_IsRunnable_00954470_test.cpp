// Behaviour test for CAIStateGroup_MazeRangedCombat::IsRunnable (0x00954470): model copy with stub callees.
#include <cstdio>
#include <cmath>

struct C3DVector { float X, Y, Z; };
struct CThing { unsigned char m_flags91; C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
struct CPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } CPointer& operator=(CThing* t) { m_pItem = t; return *this; } };
struct CTCTargetingAI { CPointer PEnemy; };
struct CTCBoss { long phase; long GetCurrentPhase() { return phase; } };
struct CCreature { CThing thing; CTCBoss* boss; CTCTargetingAI* targeting; };
static CThing* g_hero;
struct CAIBrain { CCreature* PCreature; };

struct CAIStateGroup_MazeRangedCombat {
    CAIBrain* PBrain; CPointer PTarget; long LastCompletedPhase;
    CThing* GetHero() { return g_hero; }
    bool IsRunnable();
};

bool CAIStateGroup_MazeRangedCombat::IsRunnable()
{
    CTCBoss* boss = PBrain->PCreature->boss;
    if (boss->GetCurrentPhase() != 0 && boss->GetCurrentPhase() != 5 && LastCompletedPhase < boss->GetCurrentPhase()) {
        const CCreature* creature = PBrain->PCreature;
        if (creature->targeting != 0) {
            CTCTargetingAI* targeting = creature->targeting;
            if (targeting->PEnemy.GetPItem() != 0)
                PTarget = targeting->PEnemy.GetPItem();
        }
        if (PTarget.GetPItem() == 0 || (PTarget.GetPItem()->m_flags91 & 1))
            PTarget = GetHero();
        if (PTarget.GetPItem() != 0 && !(PTarget.GetPItem()->m_flags91 & 1)) {
            const CThing* self = &PBrain->PCreature->thing;
            const CThing* target = PTarget.GetPItem();
            const C3DVector& me = self->GetPosition();
            const C3DVector& him = target->GetPosition();
            float dx = him.X - me.X;
            float dy = him.Y - me.Y;
            float dist = (float)std::sqrt(dx * dx + dy * dy);
            if (dist < 50.0f && dist > 8.0f)
                return true;
        }
    }
    return false;
}

int main()
{
    int ok = 1;
    CThing hero = { 0, { 30, 0, 0 } }, enemy = { 0, { 0, 20, 99 } };
    CTCBoss boss = { 2 };
    CTCTargetingAI targeting = { { 0 } };
    CCreature c = { { 0, { 0, 0, 0 } }, &boss, &targeting };
    CAIBrain brain = { &c };
    CAIStateGroup_MazeRangedCombat s; s.PBrain = &brain; s.PTarget.m_pItem = 0; s.LastCompletedPhase = 1;
    g_hero = &hero;
    // no enemy: falls back to the hero at 30 units
    if (!s.IsRunnable() || s.PTarget.m_pItem != &hero) ok = 0;
    // the targeting enemy wins (20 units, Z ignored)
    targeting.PEnemy.m_pItem = &enemy;
    if (!s.IsRunnable() || s.PTarget.m_pItem != &enemy) ok = 0;
    // too close / too far
    enemy.pos.Y = 8; if (s.IsRunnable()) ok = 0;
    enemy.pos.Y = 50; if (s.IsRunnable()) ok = 0;
    enemy.pos.Y = 20;
    // dead enemy: hero again
    enemy.m_flags91 = 1; if (!s.IsRunnable() || s.PTarget.m_pItem != &hero) ok = 0; enemy.m_flags91 = 0;
    // phases: 0, 5, and an already completed phase are not runnable
    boss.phase = 0; if (s.IsRunnable()) ok = 0;
    boss.phase = 5; if (s.IsRunnable()) ok = 0;
    boss.phase = 1; if (s.IsRunnable()) ok = 0;
    boss.phase = 3; if (!s.IsRunnable()) ok = 0;
    // no targeting AI, no hero, dead target
    c.targeting = 0; s.PTarget.m_pItem = 0; g_hero = 0; if (s.IsRunnable()) ok = 0;
    if (!ok) { std::printf("00954470_TEST FAIL\n"); return 1; }
    std::printf("00954470_TEST PASS\n");
    return 0;
}