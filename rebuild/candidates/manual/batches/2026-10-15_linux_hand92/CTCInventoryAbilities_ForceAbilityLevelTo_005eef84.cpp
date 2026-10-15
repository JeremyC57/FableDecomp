// CTCInventoryAbilities::ForceAbilityLevelTo(EHeroAbility, long) -- sets an ability's level, clamped to 0..its maximum level, then refreshes the hero's morph state
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EEF84, length 51 bytes.
// Callees: CTCInventoryAbilities::GetAbilityMaximumLevel(EHeroAbility) @ 0x005EDEA3 (same TU, its real body is
//          reproduced below -- it grades MATCH from this file too), CTCInventoryAbilities::UpdateWillMorph() @ 0x005EE887.
// Pragmas: optimize("s", on) for the TU; GetAbilityMaximumLevel additionally has frame-pointer omission off
//          (`optimize("y", off)`, its retail body keeps ebp) and the pragma is restored before this function.
// Shape: retail keeps `this` in EDX across the GetAbilityMaximumLevel call (`mov edx,ecx` before it, `mov ecx,edx`
//        for UpdateWillMorph). VC7.1 does that only when the callee is DEFINED EARLIER IN THE SAME TU (it then knows
//        the callee leaves edx alone); with the callee merely declared it saves `this` in esi/edi (53v51). The clamp is
//        `if (level < 0) level = 0; else if (level > max) level = max;` (`test ; jge ; xor ; jmp` then `cmp ; jle ; mov`).
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors); in the shipped class Abilities
//         (vector<pair<long level, CDefPointer<const CHeroAbilityDef>>>, 8-byte entries) is at +0x160; retail
//         CHeroAbilityDef keeps ExperienceCostsToUpgrade at +0x80 (PDB +0x8C).

struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};
struct CHeroAbilityDef {
    unsigned char _pad_0x00[0x80];
    CLongVector   ExperienceCostsToUpgrade;       // +0x80
};
struct SAbilityEntry { long Level; const CHeroAbilityDef* PDef; };
struct CAbilityEntryVector {
    SAbilityEntry* first; SAbilityEntry* last; SAbilityEntry* end_of_storage;
    SAbilityEntry& operator[](unsigned long n) { return first[n]; }
};
template <class T> inline const T& Min(const T& left, const T& right) { return right < left ? right : left; }
static const long MAX_ABILITY_LEVEL = 3;

struct CTCInventoryAbilities_Retail {
    unsigned char       _pad_0x000[0x160];
    CAbilityEntryVector Abilities;                // +0x160

    long GetAbilityMaximumLevel(long ability);    // 0x005EDEA3 (defined below)
    void UpdateWillMorph();                       // 0x005EE887
    void ForceAbilityLevelTo(long ability, long level);
};

#pragma optimize("y", off)
long CTCInventoryAbilities_Retail::GetAbilityMaximumLevel(long ability)
{
    if (ability != 0) {
        const CHeroAbilityDef* def = Abilities[ability].PDef;
        if (def != 0) {
            long level = def->ExperienceCostsToUpgrade.size() - 1;
            if (level < 0)
                level = 0;
            return Min(level, MAX_ABILITY_LEVEL);
        }
    }
    return 0;
}
#pragma optimize("y", on)

void CTCInventoryAbilities_Retail::ForceAbilityLevelTo(long ability, long level)
{
    long maximum = GetAbilityMaximumLevel(ability);
    if (level < 0)
        level = 0;
    else if (level > maximum)
        level = maximum;
    Abilities[ability].Level = level;
    UpdateWillMorph();
}
