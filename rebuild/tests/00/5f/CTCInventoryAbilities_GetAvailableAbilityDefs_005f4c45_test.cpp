#include <cstdio>
// Behaviour test for CTCInventoryAbilities::GetAvailableAbilityDefs (0x005F4C45): same logic over a copy of the model.
struct CHeroAbilityDef { long Ability; };
struct CHeroAbilityDefPointer { const CHeroAbilityDef* Object; };
struct CHeroAbilityDefPointerVector {
    const CHeroAbilityDef* items[8]; int count;
    void push_back(const CHeroAbilityDefPointer& value) { items[count++] = value.Object; }
};
struct SAbilityEntry { long Level; CHeroAbilityDefPointer PDef; };
struct CAbilityEntryVector { SAbilityEntry* first; SAbilityEntry* last; SAbilityEntry* end_of_storage; SAbilityEntry* begin() { return first; } SAbilityEntry* end() { return last; } };
struct CTCInventoryAbilities_Retail {
    CAbilityEntryVector Abilities;
    bool IsAbilityPotentiallyAvailable(long ability) { return ability != 13; }
    void GetAvailableAbilityDefs(CHeroAbilityDefPointerVector* defs)
    {
        for (SAbilityEntry* it = Abilities.begin(); it != Abilities.end(); ++it) {
            if (it->Level > -1) {
                CHeroAbilityDefPointer& def = it->PDef;
                if (def.Object != 0 && IsAbilityPotentiallyAvailable(def.Object->Ability))
                    defs->push_back(def);
            }
        }
    }
};
int main()
{
    CHeroAbilityDef d1 = { 1 }, d13 = { 13 }, d4 = { 4 }, d5 = { 5 };
    SAbilityEntry e[5] = { { 0, { &d1 } }, { 2, { &d13 } }, { -1, { &d4 } }, { 1, { 0 } }, { 3, { &d5 } } };
    CTCInventoryAbilities_Retail a; a.Abilities.first = e; a.Abilities.last = e + 5; a.Abilities.end_of_storage = e + 5;
    CHeroAbilityDefPointerVector out; out.count = 0;
    a.GetAvailableAbilityDefs(&out);
    if (out.count != 2 || out.items[0] != &d1 || out.items[1] != &d5) { std::printf("005f4c45_TEST FAIL\n"); return 1; }
    std::printf("005f4c45_TEST PASS\n");
    return 0;
}