// CTCInventoryAbilities::GetAvailableAbilityDefs(vector<CDefPointer<const CHeroAbilityDef>>*) -- collects the defs of every owned ability (level > -1) that is potentially available
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F4C45, length 74 bytes.
// Callees: CTCInventoryAbilities::IsAbilityPotentiallyAvailable(EHeroAbility) @ 0x005EE02E,
//          std::vector<CDefPointer<const CHeroAbilityDef>>::push_back(const CDefPointer&) @ 0x005FA3A5.
// Pragma: optimize("s", on) -- size TU (unaligned loop head).
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors); in the shipped class Abilities
//         (vector<pair<long level, CDefPointer<const CHeroAbilityDef>>>, 8-byte entries) is at +0x160.
//         Retail CHeroAbilityDef keeps Ability at +0x3C (PDB +0x40).
// Shape: an iterator loop with end() re-read per iteration; the entry's def pointer is bound to a reference
//        once the level test passed (`lea edi,[esi+4]`), null-tested through it, and passed by reference.

struct CHeroAbilityDef {
    unsigned char _pad_0x00[0x3C];
    long          Ability;                        // +0x3C (PDB +0x40)
};

struct CHeroAbilityDefPointer {                   // CDefPointer<const CHeroAbilityDef>
    const CHeroAbilityDef* Object;
};

struct CHeroAbilityDefPointerVector {             // std::vector<CDefPointer<const CHeroAbilityDef>>
    CHeroAbilityDefPointer* first;
    CHeroAbilityDefPointer* last;
    CHeroAbilityDefPointer* end_of_storage;
    void push_back(const CHeroAbilityDefPointer& value);   // 0x005FA3A5
};

struct SAbilityEntry {                            // pair<long, CDefPointer<const CHeroAbilityDef>>
    long                   Level;                 // +0x00 (-1 = not owned)
    CHeroAbilityDefPointer PDef;                  // +0x04
};

struct CAbilityEntryVector {
    SAbilityEntry* first;                         // +0x00
    SAbilityEntry* last;                          // +0x04
    SAbilityEntry* end_of_storage;                // +0x08
    SAbilityEntry* begin() { return first; }
    SAbilityEntry* end() { return last; }
};

struct CTCInventoryAbilities_Retail {
    unsigned char       _pad_0x000[0x160];
    CAbilityEntryVector Abilities;                // +0x160 (PDB +0x180)

    bool IsAbilityPotentiallyAvailable(long ability);   // 0x005EE02E
    void GetAvailableAbilityDefs(CHeroAbilityDefPointerVector* defs);
};

void CTCInventoryAbilities_Retail::GetAvailableAbilityDefs(CHeroAbilityDefPointerVector* defs)
{
    for (SAbilityEntry* it = Abilities.begin(); it != Abilities.end(); ++it) {
        if (it->Level > -1) {
            CHeroAbilityDefPointer& def = it->PDef;
            if (def.Object != 0 && IsAbilityPotentiallyAvailable(def.Object->Ability))
                defs->push_back(def);
        }
    }
}
