#include <cstdio>

// Model of CTCInventoryExperience::UpdateExternalExperienceSpendAbility @ 0x00572689: the inventory screen forwards the amount
// to the per-Ability spent table of the hero's CTCInventoryAbilities.
class CTCInventoryAbilities { public:
    long Spent[20];
    void SetExperienceSpentOnAbility(long index, long amount) { Spent[index] = amount; }
};
class CTCInventoryExperience { public:
    CTCInventoryAbilities* Target;
    CTCInventoryAbilities* GetTCInventoryAbilities() { return Target; }
    void UpdateExternalExperienceSpendAbility(long index, long amount) { GetTCInventoryAbilities()->SetExperienceSpentOnAbility(index, amount); }
};

int main()
{
    CTCInventoryAbilities target; for (int i = 0; i < 20; ++i) target.Spent[i] = 0;
    CTCInventoryExperience inv; inv.Target = &target;
    inv.UpdateExternalExperienceSpendAbility(3, 250);
    inv.UpdateExternalExperienceSpendAbility(7, -5);
    if (target.Spent[3] != 250 || target.Spent[7] != -5 || target.Spent[0] != 0) { std::printf("00572689_TEST FAIL\n"); return 1; }
    std::printf("00572689_TEST PASS\n");
    return 0;
}