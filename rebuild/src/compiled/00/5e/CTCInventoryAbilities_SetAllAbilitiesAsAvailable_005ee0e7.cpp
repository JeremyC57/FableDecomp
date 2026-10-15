// CTCInventoryAbilities::SetAllAbilitiesAsAvailable(bool) -- makes the hero learn every ability (1 .. count-1) that is potentially available to him
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE0E7, length 79 bytes.
// Callees: CTCInventoryAbilities::IsAbilityPotentiallyAvailable(EHeroAbility) @ 0x005EE02E,
//          CThingPlayerCreature::LearnAbility(EHeroAbility, bool, long, bool) @ 0x006AC690 on the parent thing.
// Pragma: optimize("s", on) -- size TU (unaligned loop head).
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors); in the shipped class
//         Abilities (8-byte entries) is at +0x160; the CTCBase parent thing pointer is at +0x04.
// Shape: the bool argument is unused (`ret 4`); the index is a signed long compared with the signed
//        element count re-evaluated per iteration (`sar eax,3 ; jl`; the entry test `1 < size()` folds to
//        `and eax,0FFFFFFF8h ; cmp eax,8 ; jle`).

struct CThingPlayerCreature {
    bool LearnAbility(long ability, bool notify, long level, bool silent);   // 0x006AC690
};

struct SAbilityEntry {                            // pair<long, CDefPointer<const CHeroAbilityDef>>
    long  Level;                                  // +0x00
    void* PDef;                                   // +0x04
};

struct CAbilityEntryVector {
    SAbilityEntry* first;                         // +0x00
    SAbilityEntry* last;                          // +0x04
    SAbilityEntry* end_of_storage;                // +0x08
    long size() const { return last - first; }
};

struct CTCInventoryAbilities_Retail {
    void*                 __vftable;              // +0x000
    CThingPlayerCreature* ParentThing;            // +0x004 CTCBase
    unsigned char         _pad_0x008[0x160 - 0x8];
    CAbilityEntryVector   Abilities;              // +0x160 (PDB +0x180)

    bool IsAbilityPotentiallyAvailable(long ability);   // 0x005EE02E
    void SetAllAbilitiesAsAvailable(bool available);
};

void CTCInventoryAbilities_Retail::SetAllAbilitiesAsAvailable(bool available)
{
    for (long ability = 1; ability < Abilities.size(); ++ability) {
        if (IsAbilityPotentiallyAvailable(ability))
            ParentThing->LearnAbility(ability, true, -1, true);
    }
}