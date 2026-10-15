// CTCInventoryAbilities::ForceAllAbilitiesToMaxLevel() -- raises every potentially available ability to its maximum level
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F3E7D, length 77 bytes.
// Callees: CTCInventoryAbilities::IsAbilityPotentiallyAvailable(EHeroAbility) @ 0x005EE02E,
//          CTCInventoryAbilities::ForceAbilityLevelTo(EHeroAbility, long) @ 0x005EEF84,
//          CTCInventoryAbilities::GetAbilityMaximumLevel(EHeroAbility) @ 0x005EDEA3.
// Pragma: optimize("s", on) -- size TU (unaligned loop head).
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors); in the shipped class
//         Abilities (vector<pair<long level, CDefPointer<const CHeroAbilityDef>>>, 8-byte entries) is at +0x160.
// Shape: a signed index over the signed element count re-evaluated per iteration (`sar eax,3 ; jl`; the
//        entry test `0 < size()` folds to `test eax,0FFFFFFF8h ; jle`).
//        The maximum level is fetched before the index push (right-to-left argument evaluation).

struct SAbilityEntry {                            // pair<long, CDefPointer<const CHeroAbilityDef>>
    long  Level;                                  // +0x00
    void* PDef;                                   // +0x04
};

struct CAbilityEntryVector {
    SAbilityEntry* first;                         // +0x00
    SAbilityEntry* last;                          // +0x04
    SAbilityEntry* end_of_storage;                // +0x08
    long size() const { return last - first; }
    SAbilityEntry& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventoryAbilities_Retail {
    unsigned char       _pad_0x000[0x160];
    CAbilityEntryVector Abilities;                // +0x160 (PDB +0x180)

    bool IsAbilityPotentiallyAvailable(long ability);     // 0x005EE02E
    void ForceAbilityLevelTo(long ability, long level);   // 0x005EEF84
    long GetAbilityMaximumLevel(long ability);            // 0x005EDEA3
    void ForceAllAbilitiesToMaxLevel();
};

void CTCInventoryAbilities_Retail::ForceAllAbilitiesToMaxLevel()
{
    for (long ability = 0; ability < Abilities.size(); ++ability) {
        if (IsAbilityPotentiallyAvailable(ability))
            ForceAbilityLevelTo(ability, GetAbilityMaximumLevel(ability));
    }
}