// CTCInventoryAbilities::ForceAllAbilitiesToLevel(long level) -- sets every potentially available ability to the given level (ForceAbilityLevelTo clamps it)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F3ECA, length 74 bytes.
// Callees: CTCInventoryAbilities::IsAbilityPotentiallyAvailable(EHeroAbility) @ 0x005EE02E,
//          CTCInventoryAbilities::ForceAbilityLevelTo(EHeroAbility, long) @ 0x005EEF84.
// Pragma: optimize("s", on) -- size TU (unaligned loop head).
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors); in the shipped class
//         Abilities (vector<pair<long level, CDefPointer<const CHeroAbilityDef>>>, 8-byte entries) is at +0x160.
// Shape: a signed index over the signed element count re-evaluated per iteration (`sar eax,3 ; jl`; the
//        entry test `0 < size()` folds to `test eax,0FFFFFFF8h ; jle`).
//        The level is forwarded with a pass-through `push [esp+0Ch]`.

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
    void ForceAllAbilitiesToLevel(long level);
};

void CTCInventoryAbilities_Retail::ForceAllAbilitiesToLevel(long level)
{
    for (long ability = 0; ability < Abilities.size(); ++ability) {
        if (IsAbilityPotentiallyAvailable(ability))
            ForceAbilityLevelTo(ability, level);
    }
}
