#include <cstdio>

// Model of CTCInventoryExperience::UpdateExternalExperienceSpendStat @ 0x00572672: the inventory screen forwards the amount
// to the per-Stat spent table of the hero's CTCHeroExperience.
struct CTCHeroExperience {
    long Spent[20];
    void SetExperienceSpentOnStat(long index, long amount) { Spent[index] = amount; }
};
struct CTCInventoryExperience {
    CTCHeroExperience* Target;
    CTCHeroExperience* GetTCHeroExperience() { return Target; }
    void UpdateExternalExperienceSpendStat(long index, long amount) { GetTCHeroExperience()->SetExperienceSpentOnStat(index, amount); }
};

int main()
{
    CTCHeroExperience target; for (int i = 0; i < 20; ++i) target.Spent[i] = 0;
    CTCInventoryExperience inv; inv.Target = &target;
    inv.UpdateExternalExperienceSpendStat(3, 250);
    inv.UpdateExternalExperienceSpendStat(7, -5);
    if (target.Spent[3] != 250 || target.Spent[7] != -5 || target.Spent[0] != 0) { std::printf("00572672_TEST FAIL\n"); return 1; }
    std::printf("00572672_TEST PASS\n");
    return 0;
}
