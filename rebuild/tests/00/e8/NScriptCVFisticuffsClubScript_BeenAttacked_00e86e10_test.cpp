// Behaviour test for CV_FisticuffsClubScript::BeenAttacked (0x00E86E10).
#include <cstdio>

static bool g_terminating = false;
struct CScriptDef { long FisticuffsAttackWarningLimit; };
CScriptDef* g_pScriptDef = 0;

struct CV_FisticuffsClubScript {
    bool HeroInFisticuffsClub;
    long AttackWarnings;
    bool EverybodyAttack;
    bool IsActiveThreadTerminating() { return g_terminating; }
    void BeenAttacked();
};

void CV_FisticuffsClubScript::BeenAttacked()
{
    if (!HeroInFisticuffsClub) {
        if (IsActiveThreadTerminating())
            return;
        if (AttackWarnings < g_pScriptDef->FisticuffsAttackWarningLimit) {
            if (IsActiveThreadTerminating())
                return;
            ++AttackWarnings;
        } else {
            if (IsActiveThreadTerminating())
                return;
            EverybodyAttack = true;
        }
    }
}

int main()
{
    CScriptDef def; def.FisticuffsAttackWarningLimit = 2; g_pScriptDef = &def;
    CV_FisticuffsClubScript s; s.HeroInFisticuffsClub = false; s.AttackWarnings = 0; s.EverybodyAttack = false;
    s.BeenAttacked(); s.BeenAttacked();
    if (s.AttackWarnings != 2 || s.EverybodyAttack) { std::printf("00e86e10_TEST FAIL\n"); return 1; }
    s.BeenAttacked();
    if (s.AttackWarnings != 2 || !s.EverybodyAttack) { std::printf("00e86e10_TEST FAIL\n"); return 1; }
    s.EverybodyAttack = false; s.AttackWarnings = 0; s.HeroInFisticuffsClub = true; s.BeenAttacked();
    if (s.AttackWarnings != 0 || s.EverybodyAttack) { std::printf("00e86e10_TEST FAIL\n"); return 1; }
    s.HeroInFisticuffsClub = false; g_terminating = true; s.BeenAttacked();
    if (s.AttackWarnings != 0) { std::printf("00e86e10_TEST FAIL\n"); return 1; }
    std::printf("00e86e10_TEST PASS\n");
    return 0;
}