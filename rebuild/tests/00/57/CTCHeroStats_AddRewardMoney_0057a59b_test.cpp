// 0057a59b behaviour test for CTCHeroStats::AddRewardMoney(long)
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
class CTCHeroStats {
public:
    long Experience;
    long RewardMoney;
    CTCHeroStats() : Experience(0), RewardMoney(0) { }
    void AddRewardMoney(long amount);
};
void CTCHeroStats::AddRewardMoney(long amount)
{
    RewardMoney += amount;
}
static void run()
{
    CTCHeroStats s;
    s.AddRewardMoney(150);
    s.AddRewardMoney(25);
    CHECK(s.RewardMoney == 175);
    s.AddRewardMoney(-75);
    CHECK(s.RewardMoney == 100);
    CHECK(s.Experience == 0);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("0057a59b_TEST FAIL\n");
        return 1;
    }
    std::printf("0057a59b_TEST PASS\n");
    return 0;
}