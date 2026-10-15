#include <cstdio>
// Behaviour of CTCHeroStats::AddHeroStatExperience (0x0057A8DA): adds to the stat's slot (unknown stats go to slot 0).
static inline long GetHeroStatIndex(long stat)
{
    long index = 0;
    switch (stat) {
    case 0: index = 0; break;
    case 1: index = 1; break;
    case 2: index = 2; break;
    case 3: index = 3; break;
    }
    return index;
}
struct CTCHeroStats_Retail {
    long* HeroStatsExperience;
    void AddHeroStatExperience(long stat, long amount) { HeroStatsExperience[GetHeroStatIndex(stat)] += amount; }
};
int main()
{
    long xp[4] = { 0, 0, 0, 0 };
    CTCHeroStats_Retail s; s.HeroStatsExperience = xp;
    s.AddHeroStatExperience(2, 15); s.AddHeroStatExperience(3, 4); s.AddHeroStatExperience(9, 7); s.AddHeroStatExperience(2, 5);
    if (xp[0] != 7 || xp[1] != 0 || xp[2] != 20 || xp[3] != 4) { std::printf("0057a8da_TEST FAIL\n"); return 1; }
    std::printf("0057a8da_TEST PASS\n");
    return 0;
}