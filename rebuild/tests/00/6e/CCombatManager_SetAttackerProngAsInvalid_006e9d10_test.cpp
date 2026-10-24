// Behaviour test for 0x006E9D10 (CCombatManager::SetAttackerProngAsInvalid: block the attacker's prong on its victim's wheel).
#include <cstdio>
#include <map>

struct Creature { unsigned long long UID; bool Eligible; };
struct AttackerInfo { const Creature* Attacked; long CombatWheelRingIndex; };
struct Wheel {
    int calls; const Creature* lastAttacker; AttackerInfo* lastInfo;
    void SetAttackerProngAsBlocked(const Creature& c, AttackerInfo& info) { ++calls; lastAttacker = &c; lastInfo = &info; info.CombatWheelRingIndex = -1; }
};
struct Info { AttackerInfo Attacker; Wheel CombatWheel; };
struct Manager {
    std::map<unsigned long long, Info> Combatants;
    void SetAttackerProngAsInvalid(Creature& attacker)
    {
        if (attacker.Eligible) {
            Info& info = Combatants.find(attacker.UID)->second;
            unsigned long long attackedUID = info.Attacker.Attacked->UID;
            if (info.Attacker.CombatWheelRingIndex >= 0)
                Combatants.find(attackedUID)->second.CombatWheel.SetAttackerProngAsBlocked(attacker, info.Attacker);
        }
    }
};
#define FAIL { std::printf("006e9d10_TEST FAIL\n"); return 1; }
int main()
{
    Creature attacker = { 1, true }, victim = { 2, true };
    Manager m;
    Info a = { { &victim, 2 }, { 0, 0, 0 } };
    Info v = { { 0, -1 }, { 0, 0, 0 } };
    m.Combatants[1] = a; m.Combatants[2] = v;
    m.SetAttackerProngAsInvalid(attacker);
    Wheel& w = m.Combatants[2].CombatWheel;
    if (w.calls != 1 || w.lastAttacker != &attacker || w.lastInfo != &m.Combatants[1].Attacker) FAIL;
    if (m.Combatants[1].Attacker.CombatWheelRingIndex != -1) FAIL;
    m.SetAttackerProngAsInvalid(attacker);                    // ring index now -1: nothing
    if (w.calls != 1) FAIL;
    m.Combatants[1].Attacker.CombatWheelRingIndex = 0; attacker.Eligible = false;
    m.SetAttackerProngAsInvalid(attacker);
    if (w.calls != 1) FAIL;
    std::printf("006e9d10_TEST PASS\n");
    return 0;
}