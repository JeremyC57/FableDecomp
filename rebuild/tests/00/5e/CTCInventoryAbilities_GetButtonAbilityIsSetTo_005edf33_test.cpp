#include <cstdio>
// Behaviour of CTCInventoryAbilities::GetButtonAbilityIsSetTo (0x005EDF33): the button paired with the ability, true; else -1, false.
struct SButtonEntry { long Button; long Ability; };
struct CButtonMap {
    SButtonEntry* first; SButtonEntry* last; SButtonEntry* end_of_storage;
    SButtonEntry* begin() { return first; }
    SButtonEntry* end() { return last; }
};
static SButtonEntry* FindAbility(CButtonMap& map, long ability)
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
    CButtonMap AbilityButtons;
    bool GetButtonAbilityIsSetTo(long ability, long* button)
    {
        SButtonEntry* it = FindAbility(AbilityButtons, ability);
        if (it != AbilityButtons.end()) { *button = it->Button; return true; }
        *button = -1;
        return false;
    }
};
int main()
{
    SButtonEntry entries[3] = { { 0, 11 }, { 1, 22 }, { 2, 33 } };
    CTCInventoryAbilities_Retail a; a.AbilityButtons.first = entries; a.AbilityButtons.last = entries + 3; a.AbilityButtons.end_of_storage = entries + 3;
    long button = 99;
    if (!a.GetButtonAbilityIsSetTo(22, &button) || button != 1) { std::printf("005edf33_TEST FAIL\n"); return 1; }
    if (!a.GetButtonAbilityIsSetTo(33, &button) || button != 2) { std::printf("005edf33_TEST FAIL\n"); return 1; }
    if (a.GetButtonAbilityIsSetTo(44, &button) || button != -1) { std::printf("005edf33_TEST FAIL\n"); return 1; }
    a.AbilityButtons.last = entries;
    if (a.GetButtonAbilityIsSetTo(11, &button) || button != -1) { std::printf("005edf33_TEST FAIL\n"); return 1; }
    std::printf("005edf33_TEST PASS\n"); return 0;
}