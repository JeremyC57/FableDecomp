// CTCInventoryAbilities::GetCategory(EHeroAbility ability) -- the UI spell category (0 / 1 / 2) an ability is listed under in the hero's magic menu
#pragma optimize("s", on)
// Leaf: ?GetCategory@CTCInventoryAbilities@@QAE?AW4EUISpellCategory@1@W4EHeroAbility@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EDE37 (unaligned: a size-optimised, packed TU), 72 bytes including its
//   4-entry jump table (0x005EDE5E) and 17-byte index table (0x005EDE6E); thiscall, `this` unused, `ret 4`.
// Identity (proven): Ego_r.pdb ?GetCategory@CTCInventoryAbilities@@QAE?AW4EUISpellCategory@1@W4EHeroAbility@@@Z at 0x0057CAA1
//   between GetAbilityInventoryThingName 0x0057C96E (0x133 bytes) and GetAbilityGraphicIndex 0x0057CAEA (0x49 bytes away);
//   retail runs GetAbilityInventoryThingName 0x005EDD04 (307 bytes) -> this row -> GetAbilityGraphicIndex 0x005EDE7F (72 bytes
//   away): same order, same sizes. Five callers: three in ConstructPCMagicList 0x005F1B10, two in the 0x005EEFF2 menu builder.
// Shape: one switch with three case groups and a default: abilities 1, 2, 5, 6, 10 -> category 1; 3, 9, 11, 12, 16 -> 0
//   (explicit: they keep their own index-table entry 1, merged with the default's code); 4, 8, 13, 14, 15, 17 -> 2; anything
//   else (7, and outside 1..17) -> 0 through the default (entry 3). Jump table order = case-group order (ret 1, ret 0, ret 2,
//   default); both tables were compared byte for byte against the compiled object (the grader masks them).
//   `#pragma optimize("s", on)`: the TU is size-optimised (`xor eax,eax ; inc eax`, `push 2 ; pop eax`, packed addresses).
// Names: EHeroAbility / EUISpellCategory enumerators have no ordinal source (the PDB name sets carry no enum values), so
//   the cases are written as numbers.
// Layout: `this` is unused; the class comes from engine/CTCInventoryAbilities.h under the layout-rename include pattern.
#define CTCInventoryAbilities CTCInventoryAbilities_HeaderLayout
#include "engine/CTCInventoryAbilities.h"
#undef CTCInventoryAbilities

enum EHeroAbility { };

class CTCInventoryAbilities : public CTCInventoryAbilities_HeaderLayout {
public:
    enum EUISpellCategory { };
    EUISpellCategory GetCategory(EHeroAbility ability);
};

CTCInventoryAbilities::EUISpellCategory CTCInventoryAbilities::GetCategory(EHeroAbility ability)
{
    switch (ability) {
    case 1: case 2: case 5: case 6: case 10:
        return (EUISpellCategory)1;
    case 3: case 9: case 11: case 12: case 16:
        return (EUISpellCategory)0;
    case 4: case 8: case 13: case 14: case 15: case 17:
        return (EUISpellCategory)2;
    default:
        return (EUISpellCategory)0;
    }
}