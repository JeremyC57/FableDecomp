// Behaviour test for 0x006F4850 CDefendingCombatantInfo ctor: empty attackers and wheel, null last attacker and attack pattern (standalone copy of the logic)
#include <cstdio>
#include <map>
#include <vector>
struct Ring { int body[7]; };
struct IntelligentPointer { void* PData; IntelligentPointer() : PData(0) { } };
struct Defending { std::map<unsigned long, void*> Attackers; std::vector<Ring> CombatWheel; float LastPos[3]; long LastFrame; IntelligentPointer PLastAttacker; void* PAttackPattern;
    Defending() : PAttackPattern(0) { } };

int main()
{
    Defending d; if (!d.Attackers.empty() || !d.CombatWheel.empty() || d.PLastAttacker.PData || d.PAttackPattern) { std::printf("006f4850_TEST FAIL\n"); return 1; }
    std::printf("006f4850_TEST PASS\n");
    return 0;
}