#include <cstdio>
// Behaviour test for 0x0056F03D: pools 0..3, stats 0..9, abilities 1..19 reset currentSpend; magnitude zeroed.
struct ExperienceSpendInfo { long currentSpend; long initialSpend; long levelUpSpend; };
struct SSpendInfoVector { ExperienceSpendInfo* _M_start; ExperienceSpendInfo* _M_finish; ExperienceSpendInfo* _M_end_of_storage; };
struct CTCInventoryExperience {
    SSpendInfoVector ExperienceSpendValuesPool, ExperienceSpendValuesStats, ExperienceSpendValuesAbility;
    long ExperienceSpendMagnitude;
    void ResetExperienceSpendValues();
};
void CTCInventoryExperience::ResetExperienceSpendValues()
{
    long i;
    for (i = 0; i < 4; ++i)
        ExperienceSpendValuesPool._M_start[i].currentSpend = ExperienceSpendValuesPool._M_start[i].initialSpend;
    for (i = 0; i < 10; ++i)
        ExperienceSpendValuesStats._M_start[i].currentSpend = ExperienceSpendValuesStats._M_start[i].initialSpend;
    for (i = 1; i < 20; ++i)
        ExperienceSpendValuesAbility._M_start[i].currentSpend = ExperienceSpendValuesAbility._M_start[i].initialSpend;
    ExperienceSpendMagnitude = 0;
}
static void Fill(ExperienceSpendInfo* v, long n) { for (long i = 0; i < n; ++i) { v[i].currentSpend = 1000 + i; v[i].initialSpend = i; v[i].levelUpSpend = 7; } }
int main()
{
    ExperienceSpendInfo pool[5], stats[11], ab[21];
    Fill(pool, 5); Fill(stats, 11); Fill(ab, 21);
    CTCInventoryExperience e;
    e.ExperienceSpendValuesPool._M_start = pool; e.ExperienceSpendValuesStats._M_start = stats; e.ExperienceSpendValuesAbility._M_start = ab;
    e.ExperienceSpendMagnitude = 99;
    e.ResetExperienceSpendValues();
    bool ok = e.ExperienceSpendMagnitude == 0;
    for (long i = 0; i < 5; ++i) ok = ok && pool[i].currentSpend == (i < 4 ? i : 1000 + i) && pool[i].levelUpSpend == 7;
    for (long i = 0; i < 11; ++i) ok = ok && stats[i].currentSpend == (i < 10 ? i : 1000 + i);
    for (long i = 0; i < 21; ++i) ok = ok && ab[i].currentSpend == (i >= 1 && i < 20 ? i : 1000 + i);
    if (!ok) { std::printf("0056f03d_TEST FAIL\n"); return 1; }
    std::printf("0056f03d_TEST PASS\n");
    return 0;
}