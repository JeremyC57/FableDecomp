// Behaviour test for 0x006E9450 CCombatManager::IsCreatureWithValidMeleeCombatSequence.
#include <cstdio>
#include <map>
struct CThingCreatureBase { unsigned int uid; unsigned int GetUID() const { return uid; } };
struct CCombatSequenceBase {
    bool melee, valid, zone;
    bool IsMeleeSequence() { return melee; } bool IsValid() { return valid; } bool IsZoneValid() { return zone; }
};
struct CSequencePointer { CCombatSequenceBase* Object; void* Count; };
struct CCombatantInfo;
struct CWorld { int id; };
struct C3DVector { float X, Y, Z; };
struct CCombatWheel {
    const CThingCreatureBase* blocked; const void* blockedInfo;
    CWorld* world; CCombatantInfo* attacker; CCombatantInfo* defender; const C3DVector* pos;
    void BlockAttackerProng(const CThingCreatureBase& a, const void* info) { blocked = &a; blockedInfo = info; }
    void ReassignAttacker(CWorld* w, CCombatantInfo* a, CCombatantInfo* d, const C3DVector* p) { world = w; attacker = a; defender = d; pos = p; }
};
struct CAttackingCombatantInfo { CThingCreatureBase* Attacked; CSequencePointer* seqFirst; CSequencePointer* seqLast; };
struct CCombatantInfo { CAttackingCombatantInfo AttackerInfo; CCombatWheel CombatWheel; };
struct CCombatManager {
    std::map<unsigned int, CCombatantInfo*> Combatants; CWorld* World; int finds;
    CCombatantInfo* Find(unsigned int uid) { ++finds; std::map<unsigned int, CCombatantInfo*>::iterator it = Combatants.find(uid); return it == Combatants.end() ? 0 : it->second; }
    CCombatantInfo& GetCombatantInfo(unsigned int uid) { return *Find(uid); }
    bool IsCreatureAttacking(unsigned int uid) { CCombatantInfo* i = Find(uid); return i != 0 && i->AttackerInfo.Attacked != 0; }
};
bool IsCreatureWithValidMeleeCombatSequence(CCombatManager& m, const CThingCreatureBase& creature)
{
    CCombatantInfo& info = m.GetCombatantInfo(creature.GetUID());
    for (CSequencePointer* it = info.AttackerInfo.seqFirst; it != info.AttackerInfo.seqLast; ++it) {
        CCombatSequenceBase* s = it->Object;
        if (s->IsMeleeSequence() && s->IsValid() && s->IsZoneValid()) return true;
    }
    return false;
}
int main()
{
    CCombatSequenceBase s1 = { true, true, false }, s2 = { false, true, true }, s3 = { true, true, true };
    CSequencePointer seqs[3] = { { &s1, 0 }, { &s2, 0 }, { &s3, 0 } };
    CThingCreatureBase c = { 5 };
    CCombatantInfo info; info.AttackerInfo.Attacked = 0; info.AttackerInfo.seqFirst = seqs; info.AttackerInfo.seqLast = seqs + 2;
    CCombatManager m; m.finds = 0; m.Combatants[5] = &info;
    if (IsCreatureWithValidMeleeCombatSequence(m, c)) { std::printf("006e9450_TEST FAIL\n"); return 1; }
    info.AttackerInfo.seqLast = seqs + 3;
    if (!IsCreatureWithValidMeleeCombatSequence(m, c)) { std::printf("006e9450_TEST FAIL\n"); return 1; }
    std::printf("006e9450_TEST PASS\n");
    return 0;
}