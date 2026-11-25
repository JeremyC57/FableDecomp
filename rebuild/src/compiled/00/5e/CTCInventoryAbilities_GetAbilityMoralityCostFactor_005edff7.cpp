// Float literals (NF418, 2026-11-25): retail's merged __real@ pool constants were modelled as data: g_defaultMoralityCostFactor (table ?g_defaultMoralityCostFactor@@3MA -> 0x0122DEDC) -> 0.0f. Spelled at the use
//   site; VC7.1 references a plain float literal through __real@<bits> (COMDAT, exe-wide merged), e.g. `return 0.0f;` -> fld [__real@00000000].
#pragma optimize("s",on)
struct AbilityEntry { void* a; void* ability; };
struct AbilityObj { char pad[0x90]; float costFactor; };
class CTCInventoryAbilities { public:
    char pad[0x160];
    AbilityEntry* table;
    float GetAbilityMoralityCostFactor(int index);
};
float CTCInventoryAbilities::GetAbilityMoralityCostFactor(int index)
{
    AbilityObj* ab = (AbilityObj*)table[index].ability;
    if (ab)
        return ab->costFactor;
    return 0.0f;
}