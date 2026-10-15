// Behaviour test for 0x006E9E40 (label CCombatManager::GetCombatantInfo): an attacking creature blocks its prong on its victim's wheel.
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
void BlockAttackerProngOnVictim(CCombatManager& m, const CThingCreatureBase& creature)
{
    unsigned int uid = creature.GetUID();
    if (m.IsCreatureAttacking(uid)) {
        CAttackingCombatantInfo& attackerInfo = m.GetCombatantInfo(uid).AttackerInfo;
        m.GetCombatantInfo(attackerInfo.Attacked->GetUID()).CombatWheel.BlockAttackerProng(creature, &attackerInfo);
    }
}
int main()
{
    CThingCreatureBase attacker = { 1 }, victim = { 2 };
    CCombatantInfo ai, vi; ai.AttackerInfo.Attacked = 0; vi.AttackerInfo.Attacked = 0; vi.CombatWheel.blocked = 0; vi.CombatWheel.blockedInfo = 0;
    CCombatManager m; m.finds = 0; m.Combatants[1] = &ai; m.Combatants[2] = &vi;
    BlockAttackerProngOnVictim(m, attacker);             // not attacking: nothing
    if (vi.CombatWheel.blocked != 0 || m.finds != 1) { std::printf("006e9e40_TEST FAIL\n"); return 1; }
    ai.AttackerInfo.Attacked = &victim;
    BlockAttackerProngOnVictim(m, attacker);
    if (vi.CombatWheel.blocked != &attacker || vi.CombatWheel.blockedInfo != &ai.AttackerInfo || m.finds != 4) { std::printf("006e9e40_TEST FAIL\n"); return 1; }
    std::printf("006e9e40_TEST PASS\n");
    return 0;
}