#include <cstdio>
// Behaviour of CTCHeroStats::GetHeroStatExperience (0x0057A8AF): stats 0..3 index the vector, anything else reads slot 0.
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
    long GetHeroStatExperience(long stat) const { return HeroStatsExperience[GetHeroStatIndex(stat)]; }
};
int main()
{
    long xp[4] = { 10, 20, 30, 40 };
    CTCHeroStats_Retail s; s.HeroStatsExperience = xp;
    if (s.GetHeroStatExperience(0) != 10 || s.GetHeroStatExperience(1) != 20 || s.GetHeroStatExperience(2) != 30
        || s.GetHeroStatExperience(3) != 40 || s.GetHeroStatExperience(7) != 10 || s.GetHeroStatExperience(-1) != 10) {
        std::printf("0057a8af_TEST FAIL\n"); return 1;
    }
    std::printf("0057a8af_TEST PASS\n");
    return 0;
}
