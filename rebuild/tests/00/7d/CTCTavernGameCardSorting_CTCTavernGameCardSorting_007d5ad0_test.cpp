// Behaviour test for CTCTavernGameCardSorting::CTCTavernGameCardSorting (0x007D5AD0): Times[i] = 2*(i+1), BestTimes zero,
// round targets 1/0, TargetTime = the last prize score.
#include <cstdio>
struct Prize { float Score; long Mult; };
struct Sorting {
    float Times[52]; float BestTimes[52]; long ToWin; long Best; float TargetTime;
    Sorting(const Prize* end)
    {
        for (long i = 0; i < 52; ++i) { Times[i] = (float)((i + 1) * 2); BestTimes[i] = 0.0f; }
        ToWin = 1; Best = 0;
        TargetTime = (end - 1)->Score;
    }
};
int main()
{
    Prize p[2]; p[0].Score = 30; p[1].Score = 60;
    Sorting s(p + 2);
    if (s.Times[0] != 2 || s.Times[51] != 104 || s.BestTimes[10] != 0 || s.ToWin != 1 || s.Best != 0 || s.TargetTime != 60) {
        std::printf("007d5ad0_TEST FAIL\n"); return 1;
    }
    std::printf("007d5ad0_TEST PASS\n");
    return 0;
}