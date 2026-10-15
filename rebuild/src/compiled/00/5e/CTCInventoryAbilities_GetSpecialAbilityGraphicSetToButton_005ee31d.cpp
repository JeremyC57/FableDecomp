// CTCInventoryAbilities::GetSpecialAbilityGraphicSetToButton(long) -- the icon graphic of the ability assigned to a button, 0 when the button has none
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE31D, length 72 bytes.
// Callees: CVectorMap<long, EHeroAbility>::LowerBound(const long&) @ 0x005F7F89 (on AbilityButtons).
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors); in the shipped class Abilities
//         (vector<pair<long level, CDefPointer<const CHeroAbilityDef>>>) is at +0x160 and AbilityButtons
//         (CVectorMap<long button, EHeroAbility>: {first, last, end_of_storage} of 8-byte pairs) at +0x16C.
//         Retail CHeroAbilityDef keeps IconGraphicIndex at +0x44 (PDB +0x48).
// Pragma: optimize("s", on) -- size TU (one shared epilogue reached by `jmp`); under it Find must be
//        `__forceinline` (plain inline is called out of line: 51v72).
// Shape: CVectorMap::Find inlined as `it = LowerBound(key); return (it == end() || key < it->key) ? end() : it;`
//        (the map pointer held in esi, then overwritten by end()), and the caller compares the result with
//        end() re-read from the member.

struct CHeroAbilityDef {
    unsigned char _pad_0x00[0x44];
    long          IconGraphicIndex;               // +0x44 (PDB +0x48)
};

struct SAbilityEntry {                            // pair<long, CDefPointer<const CHeroAbilityDef>>
    long                   Level;                 // +0x00
    const CHeroAbilityDef* PDef;                  // +0x04 CDefPointer::Object
};

struct CAbilityEntryVector {
    SAbilityEntry* first;                         // +0x00
    SAbilityEntry* last;                          // +0x04
    SAbilityEntry* end_of_storage;                // +0x08
    SAbilityEntry& operator[](unsigned long n) { return first[n]; }
};

struct SButtonEntry {                             // pair<long, EHeroAbility>
    long Button;                                  // +0x00
    long Ability;                                 // +0x04
};

struct CButtonMap {                               // CVectorMap<long, EHeroAbility>
    SButtonEntry* first;                          // +0x00
    SButtonEntry* last;                           // +0x04
    SButtonEntry* end_of_storage;                 // +0x08
    SButtonEntry* end() { return last; }
    SButtonEntry* LowerBound(const long& key);    // 0x005F7F89
    __forceinline SButtonEntry* Find(const long& key)
    {
        SButtonEntry* it = LowerBound(key);
        return (it == end() || key < it->Button) ? end() : it;
    }
};

struct CTCInventoryAbilities_Retail {
    unsigned char       _pad_0x000[0x160];
    CAbilityEntryVector Abilities;                // +0x160 (PDB +0x180)
    CButtonMap          AbilityButtons;           // +0x16C (PDB +0x190)

    long GetSpecialAbilityGraphicSetToButton(long button);
};

long CTCInventoryAbilities_Retail::GetSpecialAbilityGraphicSetToButton(long button)
{
    SButtonEntry* it = AbilityButtons.Find(button);
    if (it != AbilityButtons.end())
        return Abilities[it->Ability].PDef->IconGraphicIndex;
    return 0;
}