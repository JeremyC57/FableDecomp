// CTCInventoryAbilities::GetButtonAbilityIsSetTo(EHeroAbility, long*) -- finds which button the ability is assigned to; writes the button (or -1) and returns whether one was found
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EDF33, length 67 bytes.
// Callees: none (the search over AbilityButtons is inlined).
// Pragma: optimize("s", on) -- size TU (`or dword ptr [eax],-1` for the -1 store).
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors); in the shipped class
//         AbilityButtons (CVectorMap<long, EHeroAbility>: a vector of {button, ability} pairs) starts at +0x16C.
// Retry (Fable landing session): matched once the __forceinline search declares the cursor BEFORE `last` (retail loads
// first then last, runs the cursor in edx and leaves eax = last on the not-found exit).
// Shape: a linear search over the pairs comparing the ability straight from its argument slot, the
//        not-found iterator equal to end() re-read from the member for the final test.

struct SButtonEntry {                             // pair<long, EHeroAbility>
    long Button;                                  // +0x00
    long Ability;                                 // +0x04
};

struct CButtonMap {                               // CVectorMap<long, EHeroAbility>
    SButtonEntry* first;                          // +0x00
    SButtonEntry* last;                           // +0x04
    SButtonEntry* end_of_storage;                 // +0x08
    SButtonEntry* begin() { return first; }
    SButtonEntry* end() { return last; }
};

__forceinline SButtonEntry* FindAbility(CButtonMap& map, long ability)
{
    SButtonEntry* it = map.first;
    SButtonEntry* last = map.last;
    for (; it != last; ++it) {
        if (it->Ability == ability)
            return it;
    }
    return last;
}

struct CTCInventoryAbilities_Retail {
    unsigned char _pad_0x000[0x16C];
    CButtonMap    AbilityButtons;                 // +0x16C (PDB +0x190)

    bool GetButtonAbilityIsSetTo(long ability, long* button);
};

bool CTCInventoryAbilities_Retail::GetButtonAbilityIsSetTo(long ability, long* button)
{
    SButtonEntry* it = FindAbility(AbilityButtons, ability);
    if (it != AbilityButtons.end()) {
        *button = it->Button;
        return true;
    }
    *button = -1;
    return false;
}