#include <cstdio>
// Behaviour of CTCHeroStats::Drink (0x0057A82A): reservoir += multiplier * amount, one more drink, and the
// multiplier scaled by 1 - (1 - def multiplier-multiplier) * amount.
class CHeroStatsDef { public: float DrinkAffectMultiplierMultiplier; };
struct CDefinitionHolder { CHeroStatsDef* PHeroStatsDef; };
static CDefinitionHolder* g_holder = 0;
struct CTCHeroStats_Retail {
    long NumberOfDrinks;
    float BeerReservoir;
    float DrinkAffectMultiplier;
    CDefinitionHolder* GetDefinitions() const { return g_holder; }
    void Drink(float amount)
    {
        BeerReservoir += DrinkAffectMultiplier * amount;
        float sobering = 1.0f - GetDefinitions()->PHeroStatsDef->DrinkAffectMultiplierMultiplier;
        ++NumberOfDrinks;
        DrinkAffectMultiplier *= 1.0f - sobering * amount;
    }
};
int main()
{
    CHeroStatsDef def; def.DrinkAffectMultiplierMultiplier = 0.75f;
    CDefinitionHolder holder; holder.PHeroStatsDef = &def; g_holder = &holder;
    CTCHeroStats_Retail s; s.NumberOfDrinks = 2; s.BeerReservoir = 1.0f; s.DrinkAffectMultiplier = 0.5f;
    s.Drink(2.0f);
    // reservoir 1 + 0.5*2 = 2; multiplier 0.5 * (1 - 0.25*2) = 0.25
    if (s.NumberOfDrinks != 3 || s.BeerReservoir != 2.0f || s.DrinkAffectMultiplier != 0.25f) { std::printf("0057a82a_TEST FAIL\n"); return 1; }
    std::printf("0057a82a_TEST PASS\n");
    return 0;
}