// CTCInventoryAbilities::OnSpecialAbilityButtonPressed(long button) -- cast the special ability assigned to a quick-access button, if the button has one
// Leaf: OnSpecialAbilityButtonPressed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EED4C, 64 bytes, `ret 4` (__fastcall).
//   Ego_r.pdb: `?OnSpecialAbilityButtonPressed@CTCInventoryAbilities@@QAEXJ@Z`.
// Callees: CVectorMap<long, EHeroAbility>::LowerBound(const long&) @ 0x005F7F4C (the non-const copy; the const one is
//   0x005F7F89), CTCInventoryAbilities::OnSpecialAbilityUsed(EHeroAbility, bool) @ 0x005EEA9B (landed).
// Callers: 0x00690104 (the player's special-ability button handler).
// Layout: engine/CTCInventoryAbilities.h (FABLE_RETAIL_CTCINVENTORYABILITIES: the shipped layout, 12-byte STLport vectors;
//   Abilities +0x160, AbilityButtons +0x16C, as SetAbilityAsAvailable 0x005F6BD6 uses it) and engine/CVectorMap.h
//   (LowerBound returns std::pair<long, EHeroAbility>*, retail ?LowerBound@?$CVectorMap@JW4EHeroAbility@@...@@QAEPAU?$pair@JW4EHeroAbility@@@std@@ABJ@Z).
// Shape: CVectorMap::Find inlined (`it = LowerBound(key); return (it == end() || key < it->first) ? end() : it;`, map
//   pointer in esi overwritten by end()), the caller's end() test re-reads the member, then
//   OnSpecialAbilityUsed(it->second, false). Size TU (`optimize("s", on)`) like the sibling 0x005EE31D.
// Label correction: catalogued as `CTCInventoryAbilities_TriggerSpecialAbility`; the PDB name is
//   OnSpecialAbilityButtonPressed.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ONSPECIALABILITYBUTTONPRESSED
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "fable_string.h"
#include "engine/CTCInventoryAbilities.h"
#pragma optimize("s", on)

void CTCInventoryAbilities::OnSpecialAbilityButtonPressed(long button)
{
    std::pair<long, EHeroAbility>* it = AbilityButtons.Find(button);
    if (it != AbilityButtons.end())
        OnSpecialAbilityUsed(it->second, false);
}