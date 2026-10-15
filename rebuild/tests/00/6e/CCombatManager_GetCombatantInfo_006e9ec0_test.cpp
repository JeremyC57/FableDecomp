// Behaviour test for 0x006E9EC0 (label CCombatManager::GetCombatantInfo): an attacking creature is reassigned on its victim's wheel.
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
void ReassignAttackerOnVictim(CCombatManager& m, const CThingCreatureBase& creature, const C3DVector* position)
{
    unsigned int uid = creature.GetUID();
    if (m.IsCreatureAttacking(uid)) {
        CCombatantInfo& attacker = m.GetCombatantInfo(uid);
        CCombatantInfo& victim = m.GetCombatantInfo(attacker.AttackerInfo.Attacked->GetUID());
        victim.CombatWheel.ReassignAttacker(m.World, &attacker, &victim, position);
    }
}
int main()
{
    CThingCreatureBase attacker = { 1 }, victim = { 2 }; CWorld world = { 0 }; C3DVector p = { 1, 2, 3 };
    CCombatantInfo ai, vi; ai.AttackerInfo.Attacked = &victim; vi.AttackerInfo.Attacked = 0; vi.CombatWheel.attacker = 0;
    CCombatManager m; m.finds = 0; m.World = &world; m.Combatants[1] = &ai; m.Combatants[2] = &vi;
    ReassignAttackerOnVictim(m, attacker, &p);
    if (vi.CombatWheel.world != &world || vi.CombatWheel.attacker != &ai || vi.CombatWheel.defender != &vi || vi.CombatWheel.pos != &p) {
        std::printf("006e9ec0_TEST FAIL\n"); return 1;
    }
    vi.CombatWheel.attacker = 0;
    ReassignAttackerOnVictim(m, victim, &p);             // the victim attacks nobody
    if (vi.CombatWheel.attacker != 0) { std::printf("006e9ec0_TEST FAIL\n"); return 1; }
    std::printf("006e9ec0_TEST PASS\n");
    return 0;
}