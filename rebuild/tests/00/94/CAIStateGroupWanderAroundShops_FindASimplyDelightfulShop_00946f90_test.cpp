// Test for CAIStateGroup_WanderAroundShops::FindASimplyDelightfulShop (0x00946F90): gather the scannable buildings that
// have a shop component (TC 0x53), keep those whose shopper count is below the global maximum, and return one of them
// picked by the world seed (GFRandom(count, seed)); no candidate returns null.
#include <cstdio>
class CThing { public: bool IncludedInScans; bool HasShop; long Shoppers; };
static unsigned long g_MaxShoppersPerShop = 2;
static unsigned long GFRandom(unsigned long n, unsigned long* seed) { *seed = *seed * 0x24a1 + 0x24df; return *seed % n; }
static CThing* FindASimplyDelightfulShop(CThing* things, int count, unsigned long* seed)
{
    CThing* shops[8]; int nshops = 0;
    for (int i = 0; i < count; ++i) if (things[i].IncludedInScans && things[i].HasShop) shops[nshops++] = &things[i];
    CThing* available[8]; int navail = 0;
    for (int i = 0; i < nshops; ++i) if ((unsigned long)shops[i]->Shoppers < g_MaxShoppersPerShop) available[navail++] = shops[i];
    if (navail == 0) return 0;
    return available[GFRandom(navail, seed)];
}
int main()
{
    CThing t[5] = { { true, true, 5 }, { true, false, 0 }, { false, true, 0 }, { true, true, 1 }, { true, true, 0 } };
    unsigned long seed = 1;
    CThing* s = FindASimplyDelightfulShop(t, 5, &seed);
    bool ok = (s == &t[3] || s == &t[4]) && seed != 1;
    unsigned long seed2 = 1; FindASimplyDelightfulShop(t, 5, &seed2);
    ok = ok && seed2 == seed;                          // deterministic for one seed
    t[3].Shoppers = 2; t[4].Shoppers = 9;
    ok = ok && FindASimplyDelightfulShop(t, 5, &seed) == 0;
    std::printf(ok ? "00946f90_TEST PASS\n" : "00946f90_TEST FAIL\n");
    return ok ? 0 : 1;
}