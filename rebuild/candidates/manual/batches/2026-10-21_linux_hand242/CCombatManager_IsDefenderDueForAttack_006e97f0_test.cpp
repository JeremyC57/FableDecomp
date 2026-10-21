// Behaviour test for CCombatManager::IsDefenderDueForAttack (0x006E97F0): refused when the defender cannot be
// attacked, when another attacker runs a melee/ranged sequence, or when the current pattern entry is not an attack.
#include <cstdio>

static int g_traces = 0;
struct CCharString { int len; };
static CCharString operator+(const CCharString& a, const char*) { CCharString r = a; r.len += 1; return r; }
static void CombatTrace(const CCharString&) { ++g_traces; }
struct CThingCreatureBase { bool Attackable; int Id; };
static bool IsAbleToBeAttacked(const CThingCreatureBase& c) { return c.Attackable; }
static CCharString GetName(const CThingCreatureBase&) { CCharString s = { 3 }; return s; }
struct CCombatSequenceBase { bool Melee, Ranged, Running; bool IsMeleeSequence() { return Melee; } bool IsRangedSequence() { return Ranged; } };
struct CCombatantInfo {
    CThingCreatureBase* Parent;
    long CurrentSequence; CCombatSequenceBase* Sequences[2];
    CThingCreatureBase* Attackers[4]; unsigned long AttackerCount;
    long* AttackPattern; long AttackPatternIndex;
};
struct CCombatManager {
    CCombatantInfo* Infos; int Count;
    const CCombatantInfo& GetCombatantInfo(int id) const { for (int i = 0; i < Count; ++i) if (Infos[i].Parent->Id == id) return Infos[i]; return Infos[0]; }
    bool IsAnotherAttackerAttacking(const CCombatantInfo& info) const
    {
        if (info.AttackerCount > 1) {
            for (unsigned long i = 0; i < info.AttackerCount; ++i) {
                const CCombatantInfo& attacker = GetCombatantInfo(info.Attackers[i]->Id);
                if (attacker.CurrentSequence >= 0) {
                    CCombatSequenceBase* sequence = attacker.Sequences[attacker.CurrentSequence];
                    if ((sequence->IsMeleeSequence() || sequence->IsRangedSequence()) && sequence->Running)
                        return true;
                }
            }
        }
        return false;
    }
    bool IsDefenderDueForAttack(const CCombatantInfo& info) const;
};

bool CCombatManager::IsDefenderDueForAttack(const CCombatantInfo& info) const
{
    if (!IsAbleToBeAttacked(*info.Parent)) {
        { CCharString name = GetName(*info.Parent); CCharString message = name + " mode"; CombatTrace(message); }
        return false;
    }
    if (IsAnotherAttackerAttacking(info)) {
        { CCharString name = GetName(*info.Parent); CCharString message = name + " other attackers"; CombatTrace(message); }
        return false;
    }
    if (info.AttackPattern[info.AttackPatternIndex] <= 0) {
        { CCharString name = GetName(*info.Parent); CCharString message = name + " attack patterns"; CombatTrace(message); }
        return false;
    }
    return true;
}

int main()
{
    CThingCreatureBase hero = { true, 1 }, a1 = { true, 2 }, a2 = { true, 3 };
    CCombatSequenceBase idle = { false, false, true }, melee = { true, false, true };
    long pattern[2] = { 1, 0 };
    CCombatantInfo infos[3];
    infos[0].Parent = &hero; infos[0].CurrentSequence = -1; infos[0].AttackerCount = 2; infos[0].Attackers[0] = &a1; infos[0].Attackers[1] = &a2;
    infos[0].AttackPattern = pattern; infos[0].AttackPatternIndex = 0;
    infos[1].Parent = &a1; infos[1].CurrentSequence = 0; infos[1].Sequences[0] = &idle;
    infos[2].Parent = &a2; infos[2].CurrentSequence = -1;
    CCombatManager m; m.Infos = infos; m.Count = 3;
    bool ok = m.IsDefenderDueForAttack(infos[0]) && g_traces == 0;
    infos[1].Sequences[0] = &melee;
    ok = ok && !m.IsDefenderDueForAttack(infos[0]) && g_traces == 1;
    infos[0].AttackerCount = 1;                       // a single attacker is never checked
    ok = ok && m.IsDefenderDueForAttack(infos[0]);
    infos[0].AttackPatternIndex = 1;
    ok = ok && !m.IsDefenderDueForAttack(infos[0]) && g_traces == 2;
    hero.Attackable = false;
    ok = ok && !m.IsDefenderDueForAttack(infos[0]) && g_traces == 3;
    if (!ok) { std::printf("006e97f0_TEST FAIL\n"); return 1; }
    std::printf("006e97f0_TEST PASS\n");
    return 0;
}
