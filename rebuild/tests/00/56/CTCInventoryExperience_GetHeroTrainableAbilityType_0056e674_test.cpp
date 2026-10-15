#include <cstdio>
// Behaviour test for 0x0056e674: category + button lookup in a 20-entry table, first match wins.
struct CCategory { long Id; long GetCategoryIdentifier() { return Id; } };
struct STrainableEntry { long Result; long Category; long Button; };
static STrainableEntry g_Table[20 + 1];
struct CTCInventoryExperience {
    CCategory* Selected;
    CCategory* GetSelectedCategory() { return Selected; }
    long GetHeroTrainableAbilityType(long button);
};
long CTCInventoryExperience::GetHeroTrainableAbilityType(long button)
{
    long category = 0;
    CCategory* selected = GetSelectedCategory();
    if (selected)
        category = selected->GetCategoryIdentifier();
    long i;
    for (i = 0; i < 20; ++i) {
        if (g_Table[i].Category == category && g_Table[i].Button == button)
            break;
    }
    return g_Table[i].Result;
}
int main()
{
    for (long i = 0; i <= 20; ++i) { g_Table[i].Result = 100 + i; g_Table[i].Category = i / 4; g_Table[i].Button = i % 4; }
    g_Table[20].Category = -1;
    g_Table[20 - 1].Category = 2; g_Table[20 - 1].Button = 1;     // duplicate of entry 9: first match must win
    CCategory c; c.Id = 2;
    CTCInventoryExperience e; e.Selected = &c;
    if (e.GetHeroTrainableAbilityType(1) != 109) { std::printf("0056e674_TEST FAIL\n"); return 1; }
    e.Selected = 0;                                                 // no category -> category 0
    if (e.GetHeroTrainableAbilityType(3) != 103) { std::printf("0056e674_TEST FAIL\n"); return 1; }
    if (e.GetHeroTrainableAbilityType(7) != 100 + 20) { std::printf("0056e674_TEST FAIL\n"); return 1; }   // no match reads entry [20]
    std::printf("0056e674_TEST PASS\n");
    return 0;
}