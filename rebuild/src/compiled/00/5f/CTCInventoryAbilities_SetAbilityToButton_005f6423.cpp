// CTCInventoryAbilities::SetAbilityToButton(EHeroAbility ability, long button, bool flag) -- assign a special ability to a quick-access button (moving it off any other button), or clear the button for the null ability
// Leaf: SetAbilityToButton
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F6423, 186 bytes, `ret 0Ch` (__fastcall; the bool is not read here).
//   Ego_r.pdb: `?SetAbilityToButton@CTCInventoryAbilities@@QAEXW4EHeroAbility@@J_N@Z`.
// Callees (AbilityButtons, CVectorMap<long button, EHeroAbility>): LowerBound(const long&) @ 0x005F7F4C (non-const copy),
//   vector::erase(iterator) @ 0x005F87C0, vector::push_back(const pair&) @ 0x005F9C4F, CVectorMap::Sort() @ 0x005FA47D
//   (sort + `Dirty = false`; the out-of-line Update() `if (Dirty) Sort();` is 0x005FA4AF).
// Callers: 0x005F650E, 0x005F6629, 0x005F6D4F, 0x005F7245 (CTCInventoryAbilities).
// Layout: engine/CTCInventoryAbilities.h (FABLE_RETAIL_CTCINVENTORYABILITIES: the shipped layout, AbilityButtons at +0x16C)
//   and engine/CVectorMap.h (std::pair<long, EHeroAbility> entries over the STLport vector of stlport/fable_containers.h).
// Shape: frame-pointer size TU (`optimize("y", off)` + `("s", on)`); FableWin.pdb names the CVectorMap members Find /
//   Insert(const K&, const V&) / Update() / Sort(): Find inlined (`(it == end() || key < it->first) ? end() : it`),
//   the value search is the landed 0x005EDF33 helper (an early `return it`, then `return last`), Insert builds the
//   pair temporary in the 8-byte frame, push_back, `Dirty = true`, and the inline Update() re-reads Dirty before Sort().
// Label correction: catalogued as `AbilityMap_SetData`; the PDB name is SetAbilityToButton.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_BUTTONS
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CVECTORMAP_VALFIND
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#define FABLE_RETAIL_CVECTORMAP_SORT_OUTOFLINE
#include "fable_string.h"
#include "engine/CTCInventoryAbilities.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

void CTCInventoryAbilities::SetAbilityToButton(EHeroAbility ability, long button, bool flag)
{
    if (ability == HERO_ABILITY_NULL) {
        std::pair<long, EHeroAbility>* it = AbilityButtons.Find(button);
        if (it != AbilityButtons.end())
            AbilityButtons.erase(it);
    }
    else {
        std::pair<long, EHeroAbility>* it = AbilityButtons.ValFind(ability);
        if (it != AbilityButtons.end())
            AbilityButtons.erase(it);
        it = AbilityButtons.Find(button);
        if (it == AbilityButtons.end()) {
            AbilityButtons.Insert(button, ability);
            AbilityButtons.Update();
        }
        else
            it->second = ability;
    }
}