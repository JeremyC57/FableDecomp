// Behaviour test for 0x006E9680 (CCombatManager::GetCombatWheelRadiusForAttacker: the victim's wheel radius = sum of all ring thicknesses).
#include <cstdio>
#include <map>
#include <vector>

struct Creature { unsigned long long UID; };
struct Ring { float RingThickness; };
struct Wheel {
    std::vector<Ring> CombatRings;
    const Ring* GetRing(long i) const { return i >= 0 && (unsigned long)i < CombatRings.size() ? &CombatRings[i] : 0; }
    float GetRingRadius(long index) const { float sum = 0.0f; for (long i = index; i >= 0; --i) sum += GetRing(i)->RingThickness; return sum; }
    float GetWheelRadius() const { return GetRingRadius((long)CombatRings.size() - 1); }
};
struct Info { const Creature* Attacked; Wheel CombatWheel; };
struct Manager {
    std::map<unsigned long long, Info> Combatants;
    const Info& GetCombatantInfo(const Creature& c) const { return Combatants.find(c.UID)->second; }
    float GetCombatWheelRadiusForAttacker(const Creature& attacker) const
    {
        const Info& info = GetCombatantInfo(attacker);
        return GetCombatantInfo(*info.Attacked).CombatWheel.GetWheelRadius();
    }
};
#define FAIL { std::printf("006e9680_TEST FAIL\n"); return 1; }
int main()
{
    Creature attacker = { 3 }, victim = { 4 };
    Manager m;
    Info a; a.Attacked = &victim;
    Info v; v.Attacked = 0;
    Ring r[3] = { { 1.5f }, { 2.0f }, { 0.25f } };
    for (int i = 0; i < 3; ++i) v.CombatWheel.CombatRings.push_back(r[i]);
    Ring own = { 100.0f }; a.CombatWheel.CombatRings.push_back(own);
    m.Combatants[3] = a; m.Combatants[4] = v;
    if (m.GetCombatWheelRadiusForAttacker(attacker) != 3.75f) FAIL;
    m.Combatants[4].CombatWheel.CombatRings.clear();
    if (m.GetCombatWheelRadiusForAttacker(attacker) != 0.0f) FAIL;   // no rings: index -1, empty sum
    std::printf("006e9680_TEST PASS\n");
    return 0;
}