#include <cstdio>
// Behaviour test for 0x00572636: category 1..2 -> stat; 3..5 -> ability if indicator moved else stat; others nothing.
static int g_stat = 0, g_ability = 0;
struct CCategory { long Id; long GetCategoryIdentifier() { return Id; } };
struct CTCInventoryExperience {
    long statIndicatorPos;
    CCategory* Selected;
    CCategory* GetSelectedCategory() { return Selected; }
    void UpdateExperienceSpendStat() { ++g_stat; }
    void UpdateExperienceSpendAbility() { ++g_ability; }
    void ProcessExperienceAction();
};
void CTCInventoryExperience::ProcessExperienceAction()
{
    CCategory* selected = GetSelectedCategory();
    if (selected) {
        switch (selected->GetCategoryIdentifier()) {
        case 1:
        case 2:
            UpdateExperienceSpendStat();
            break;
        case 3:
        case 4:
        case 5:
            if (statIndicatorPos)
                UpdateExperienceSpendAbility();
            else
                UpdateExperienceSpendStat();
            break;
        }
    }
}
static bool Check(CTCInventoryExperience& e, CCategory* c, long id, long pos, int stat, int ab)
{
    if (c) c->Id = id;
    e.Selected = c; e.statIndicatorPos = pos; g_stat = g_ability = 0;
    e.ProcessExperienceAction();
    return g_stat == stat && g_ability == ab;
}
int main()
{
    CTCInventoryExperience e; CCategory c;
    bool ok = Check(e, 0, 1, 0, 0, 0) && Check(e, &c, 0, 1, 0, 0) && Check(e, &c, -2, 1, 0, 0) &&
              Check(e, &c, 1, 1, 1, 0) && Check(e, &c, 2, 3, 1, 0) && Check(e, &c, 3, 0, 1, 0) &&
              Check(e, &c, 4, 2, 0, 1) && Check(e, &c, 5, 1, 0, 1) && Check(e, &c, 6, 1, 0, 0);
    if (!ok) { std::printf("00572636_TEST FAIL\n"); return 1; }
    std::printf("00572636_TEST PASS\n");
    return 0;
}