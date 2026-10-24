// Behaviour test for 0x006E93C0 (CCombatManager::IsCreatureOnRotatingCombatRing).
#include <cstdio>
#include <cmath>
#include <map>
#include <vector>

static const float g_Epsilon = 1e-4f;
struct Creature { unsigned long long UID; bool Eligible; };
struct Ring { float RotationAngle; float DeltaRotation; bool IsRotating() const { return (float)std::fabs(DeltaRotation) > g_Epsilon; } };
struct Wheel {
    std::vector<Ring> CombatRings;
    const Ring* GetRing(long i) const { if (i >= 0 && (unsigned long)i < CombatRings.size()) return &CombatRings[i]; return 0; }
};
struct Info { const Creature* Attacked; long CombatWheelRingIndex; Wheel CombatWheel; };
struct Manager {
    std::map<unsigned long long, Info> Combatants;
    const Info& GetCombatantInfo(const Creature& c) const { return Combatants.find(c.UID)->second; }
    bool IsCreatureOnRotatingCombatRing(const Creature& creature) const
    {
        if (creature.Eligible) {
            const Info& info = GetCombatantInfo(creature);
            unsigned long long attackedUID = info.Attacked->UID;
            long ringIndex = info.CombatWheelRingIndex;
            if (ringIndex >= 0 && Combatants.find(attackedUID)->second.CombatWheel.GetRing(ringIndex)->IsRotating())
                return true;
        }
        return false;
    }
};
#define FAIL { std::printf("006e93c0_TEST FAIL\n"); return 1; }
int main()
{
    Creature attacker = { 7, true }, victim = { 9, true };
    Manager m;
    Info a; a.Attacked = &victim; a.CombatWheelRingIndex = 1;
    Info v; v.Attacked = 0; v.CombatWheelRingIndex = -1;
    Ring r0 = { 0.0f, 0.5f }, r1 = { 0.0f, -0.25f };
    v.CombatWheel.CombatRings.push_back(r0); v.CombatWheel.CombatRings.push_back(r1);
    m.Combatants[7] = a; m.Combatants[9] = v;
    if (!m.IsCreatureOnRotatingCombatRing(attacker)) FAIL;            // |-0.25| > eps
    m.Combatants[9].CombatWheel.CombatRings[1].DeltaRotation = 0.00005f;
    if (m.IsCreatureOnRotatingCombatRing(attacker)) FAIL;             // below tolerance
    m.Combatants[7].CombatWheelRingIndex = 0;
    if (!m.IsCreatureOnRotatingCombatRing(attacker)) FAIL;
    m.Combatants[7].CombatWheelRingIndex = -1;
    if (m.IsCreatureOnRotatingCombatRing(attacker)) FAIL;             // not on the wheel
    attacker.Eligible = false; m.Combatants[7].CombatWheelRingIndex = 0;
    if (m.IsCreatureOnRotatingCombatRing(attacker)) FAIL;
    std::printf("006e93c0_TEST PASS\n");
    return 0;
}