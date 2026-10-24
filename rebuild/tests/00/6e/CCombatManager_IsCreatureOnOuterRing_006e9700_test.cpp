// Behaviour test for 0x006E9700 (CCombatManager::IsCreatureOnOuterRing).
#include <cstdio>
#include <map>

struct TypeDef { long OuterRing; };
struct Creature { unsigned long long UID; bool Eligible; TypeDef Def; const TypeDef* GetCombatTypeDef() const { return &Def; } };
struct Wheel {
    long RingCount;
    bool IsCombatRingOuterRing(const TypeDef& def, long index) const { return index == RingCount - 1 || index >= def.OuterRing; }
};
struct Info { const Creature* Attacked; long CombatWheelRingIndex; bool UsingCombatWheel; Wheel CombatWheel; };
struct Manager {
    std::map<unsigned long long, Info> Combatants;
    bool IsCreatureOnOuterRing(const Creature& creature) const
    {
        if (creature.Eligible) {
            const Info& info = Combatants.find(creature.UID)->second;
            if (!info.UsingCombatWheel) return true;
            const Creature* attacked = info.Attacked;
            std::map<unsigned long long, Info>::const_iterator entry = Combatants.find(attacked->UID);
            long ringIndex = info.CombatWheelRingIndex;
            if (ringIndex < 0 || entry->second.CombatWheel.IsCombatRingOuterRing(*attacked->GetCombatTypeDef(), ringIndex))
                return true;
        }
        return false;
    }
};
#define FAIL { std::printf("006e9700_TEST FAIL\n"); return 1; }
int main()
{
    Creature attacker = { 1, true, { 99 } }, victim = { 2, true, { 2 } };
    Manager m;
    Info a = { &victim, 0, true, { 0 } };
    Info v = { 0, -1, false, { 3 } };
    m.Combatants[1] = a; m.Combatants[2] = v;
    if (m.IsCreatureOnOuterRing(attacker)) FAIL;                  // ring 0 of 3, victim's type outer ring 2
    m.Combatants[1].CombatWheelRingIndex = 2;
    if (!m.IsCreatureOnOuterRing(attacker)) FAIL;                 // last ring
    m.Combatants[1].CombatWheelRingIndex = 1; victim.Def.OuterRing = 1;
    if (!m.IsCreatureOnOuterRing(attacker)) FAIL;                 // the victim's combat type decides
    victim.Def.OuterRing = 5;
    if (m.IsCreatureOnOuterRing(attacker)) FAIL;
    m.Combatants[1].CombatWheelRingIndex = -1;
    if (!m.IsCreatureOnOuterRing(attacker)) FAIL;                 // no ring
    m.Combatants[1].CombatWheelRingIndex = 0; m.Combatants[1].UsingCombatWheel = false;
    if (!m.IsCreatureOnOuterRing(attacker)) FAIL;                 // not on the wheel
    attacker.Eligible = false;
    if (m.IsCreatureOnOuterRing(attacker)) FAIL;
    std::printf("006e9700_TEST PASS\n");
    return 0;
}