// Test for 0x005EDE37 CTCInventoryAbilities::GetCategory: the category table read from retail's index table.
#include <cstdio>
enum EHeroAbility { };
class CTCInventoryAbilities { public:
    enum EUISpellCategory { };
    EUISpellCategory GetCategory(EHeroAbility ability);
};
CTCInventoryAbilities::EUISpellCategory CTCInventoryAbilities::GetCategory(EHeroAbility ability)
{
    switch (ability) {
    case 1: case 2: case 5: case 6: case 10: return (EUISpellCategory)1;
    case 3: case 9: case 11: case 12: case 16: return (EUISpellCategory)0;
    case 4: case 8: case 13: case 14: case 15: case 17: return (EUISpellCategory)2;
    default: return (EUISpellCategory)0;
    }
}
int main()
{
    // retail 0x005EDE6E index table (ability - 1) -> jump entry; entries 0 = 1, 1 = 0, 2 = 2, 3 = default 0
    static const unsigned char idx[17] = { 0,0,1,2,0,0,3,2,1,0,1,1,2,2,2,1,2 };
    static const int entry[4] = { 1, 0, 2, 0 };
    CTCInventoryAbilities a;
    for (int i = -3; i < 25; ++i) {
        int want = (i >= 1 && i <= 17) ? entry[idx[i - 1]] : 0;
        if ((int)a.GetCategory((EHeroAbility)i) != want) { std::printf("005ede37_TEST FAIL\n"); return 1; }
    }
    std::printf("005ede37_TEST PASS\n");
    return 0;
}