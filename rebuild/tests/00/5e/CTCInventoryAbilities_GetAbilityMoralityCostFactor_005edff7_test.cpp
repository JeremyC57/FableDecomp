// Behaviour test for 0x005EDFF7 CTCInventoryAbilities::GetAbilityMoralityCostFactor: the ability's cost factor (+0x90), else 0.0f (retail's pooled
// __real@00000000, 0x0122DEDC) for an empty slot.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object (and
// defines its callees under the row's spellings); it asserts the value of the float literal the row now spells (a mutated literal fails).
#include <cstdio>
struct AbilityEntry { void* a; void* ability; };
struct AbilityObj { char pad[0x90]; float costFactor; };
class CTCInventoryAbilities { public:
    char pad[0x160];
    AbilityEntry* table;
    float GetAbilityMoralityCostFactor(int index);
};
int main()
{
    static CTCInventoryAbilities inv; static AbilityObj ab; AbilityEntry entries[2];
    ab.costFactor = 2.5f; entries[0].a = 0; entries[0].ability = 0; entries[1].a = 0; entries[1].ability = &ab; inv.table = entries;
    float r0 = inv.GetAbilityMoralityCostFactor(0), r1 = inv.GetAbilityMoralityCostFactor(1);
    if (r0 != 0.0f || r1 != 2.5f) { std::printf("BAD %f %f\n", r0, r1); return 1; }
    std::printf("OK_0x005edff7\n");
    return 0;
}
