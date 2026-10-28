// Test: CAIStateGroup_MinionSitAtSchool::AllocChairSittingMode -- GFRandom(2) (LCG step, ROR 13, modulo) picks the studious chair mode (0) or the base mode (1)
#include <cstdio>
static unsigned int GFROR13(unsigned int v) { return (v >> 13) | (v << 19); }
static unsigned int GFRandom(unsigned int range, unsigned int* seed)
{
    *seed = *seed * 0x24A1u + 0x24DFu;
    *seed = GFROR13(*seed);
    return *seed % range;
}
static int Alloc(unsigned int* seed)
{
    switch (GFRandom(2, seed)) {
    case 0: return 1;   // new CModeSitChairStudious(*PBrain->Creature)
    case 1: return 2;   // CAIStateGroup_UsesSeat::AllocChairSittingMode()
    }
    return 0;
}
int main()
{
    unsigned int seed = 1;
    int seen1 = 0, seen2 = 0;
    if (GFROR13(0x2000u) != 1u || GFROR13(1u) != 0x80000u) { std::printf("0090f7d0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 64; ++i) {
        int r = Alloc(&seed);
        if (r != (int)(seed % 2) + 1) { std::printf("0090f7d0_TEST FAIL\n"); return 1; }
        if (r == 1) seen1 = 1; else seen2 = 1;
    }
    if (!seen1 || !seen2) { std::printf("0090f7d0_TEST FAIL\n"); return 1; }
    std::printf("0090f7d0_TEST PASS\n");
    return 0;
}