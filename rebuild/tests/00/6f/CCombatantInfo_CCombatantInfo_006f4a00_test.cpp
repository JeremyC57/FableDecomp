// Behaviour test for 0x006F4A00 CCombatantInfo ctor: the attacking info and the embedded defending info start empty (standalone copy of the logic)
#include <cstdio>
#include <map>
#include <vector>
struct Ring { int body[7]; };
struct IntelligentPointer { void* PData; IntelligentPointer() : PData(0) { } };
struct Defending { std::map<unsigned long, void*> Attackers; std::vector<Ring> CombatWheel; float LastPos[3]; long LastFrame; IntelligentPointer PLastAttacker; void* PAttackPattern;
    Defending() : PAttackPattern(0) { } };
struct Attacking { std::vector<int> Targets; };
struct Combatant { Attacking AttackerInfo; Defending DefenderInfo; };

int main()
{
    Combatant c; if (!c.AttackerInfo.Targets.empty() || !c.DefenderInfo.Attackers.empty() || c.DefenderInfo.PAttackPattern || c.DefenderInfo.PLastAttacker.PData) { std::printf("006f4a00_TEST FAIL\n"); return 1; }
    std::printf("006f4a00_TEST PASS\n");
    return 0;
}