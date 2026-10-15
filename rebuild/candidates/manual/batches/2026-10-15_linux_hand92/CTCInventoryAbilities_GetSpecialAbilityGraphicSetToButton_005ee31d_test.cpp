#include <cstdio>
// Behaviour test for CTCInventoryAbilities::GetSpecialAbilityGraphicSetToButton (0x005EE31D): same logic over a copy of the model.
struct CHeroAbilityDef { long IconGraphicIndex; };
struct SAbilityEntry { long Level; const CHeroAbilityDef* PDef; };
struct CAbilityEntryVector { SAbilityEntry* first; SAbilityEntry* last; SAbilityEntry* end_of_storage; SAbilityEntry& operator[](unsigned long n) { return first[n]; } };
struct SButtonEntry { long Button; long Ability; };
struct CButtonMap {
    SButtonEntry* first; SButtonEntry* last; SButtonEntry* end_of_storage;
    SButtonEntry* end() { return last; }
    SButtonEntry* LowerBound(const long& key) { SButtonEntry* it = first; while (it != last && it->Button < key) ++it; return it; }
    SButtonEntry* Find(const long& key) { SButtonEntry* it = LowerBound(key); return (it == end() || key < it->Button) ? end() : it; }
};
struct CTCInventoryAbilities_Retail {
    CAbilityEntryVector Abilities; CButtonMap AbilityButtons;
    long GetSpecialAbilityGraphicSetToButton(long button)
    {
        SButtonEntry* it = AbilityButtons.Find(button);
        if (it != AbilityButtons.end())
            return Abilities[it->Ability].PDef->IconGraphicIndex;
        return 0;
    }
};
int main()
{
    CHeroAbilityDef d1 = { 101 }, d2 = { 202 };
    SAbilityEntry e[3] = { { 0, 0 }, { 0, &d1 }, { 0, &d2 } };
    SButtonEntry b[2] = { { 1, 2 }, { 3, 1 } };
    CTCInventoryAbilities_Retail a;
    a.Abilities.first = e; a.Abilities.last = e + 3; a.Abilities.end_of_storage = e + 3;
    a.AbilityButtons.first = b; a.AbilityButtons.last = b + 2; a.AbilityButtons.end_of_storage = b + 2;
    if (a.GetSpecialAbilityGraphicSetToButton(1) != 202 || a.GetSpecialAbilityGraphicSetToButton(3) != 101
        || a.GetSpecialAbilityGraphicSetToButton(2) != 0 || a.GetSpecialAbilityGraphicSetToButton(9) != 0) { std::printf("005ee31d_TEST FAIL\n"); return 1; }
    std::printf("005ee31d_TEST PASS\n");
    return 0;
}
