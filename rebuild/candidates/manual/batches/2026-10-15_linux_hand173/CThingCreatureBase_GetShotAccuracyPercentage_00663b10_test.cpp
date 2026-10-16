// behaviour test for CThingCreatureBase::GetShotAccuracyPercentage (0x00663B10)
#include <cstdio>
struct CTCHeroExperience { int pct; int GetShotAccuracyPercentage() { return pct; } };
struct CThing { bool runningAway; };
struct CCombatantInfo { int w; CThing* Attacked; };
struct CThingCreatureBase;
struct CCombatManager {
    bool attacking; CCombatantInfo info;
    bool IsCreatureAttacking(const CThingCreatureBase&) { return attacking; }
    CCombatantInfo* GetCombatantInfo(const CThingCreatureBase&) { return &info; }
};
static bool IsThingRunningAwayFromCombat(CThing* t) { return t->runningAway; }
struct CThingCreatureBase {
    unsigned char m_flags6c; bool hasExp; CTCHeroExperience* exp; CCombatManager* cm; int ShotAccuracyPercentage;
    int GetShotAccuracyPercentage()
    {
        if ((m_flags6c & 0x20) && hasExp) return exp->GetShotAccuracyPercentage();
        CCombatManager* combat = cm;
        if (combat->IsCreatureAttacking(*this)) {
            CCombatManager* combat2 = cm;
            if (IsThingRunningAwayFromCombat(combat2->GetCombatantInfo(*this)->Attacked)) return 100;
        }
        return ShotAccuracyPercentage;
    }
};
int main()
{
    CTCHeroExperience e; e.pct = 77; CThing target; target.runningAway = true;
    CCombatManager m; m.attacking = false; m.info.Attacked = &target;
    CThingCreatureBase c; c.m_flags6c = 0x20; c.hasExp = true; c.exp = &e; c.cm = &m; c.ShotAccuracyPercentage = 40;
    int bad = c.GetShotAccuracyPercentage() != 77;
    c.hasExp = false; if (c.GetShotAccuracyPercentage() != 40) bad = 1;
    m.attacking = true; if (c.GetShotAccuracyPercentage() != 100) bad = 1;
    target.runningAway = false; if (c.GetShotAccuracyPercentage() != 40) bad = 1;
    c.m_flags6c = 0; c.hasExp = true; if (c.GetShotAccuracyPercentage() != 40) bad = 1;
    if (bad) { std::printf("00663b10_TEST FAIL\n"); return 1; }
    std::printf("00663b10_TEST PASS\n");
    return 0;
}
