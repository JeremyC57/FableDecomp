// Behaviour test: CAIStateGroup_WanderAroundShops::FindASimplyDelightfulShop (0x00946F90) -- of the shop buildings, keep
// those with fewer shoppers than g_MaxShoppersPerShop (3) and return the GFRandom-picked one, null when none qualifies.
#include <cstdio>
static unsigned long GFROR13(unsigned long v) { v &= 0xffffffffUL; return ((v >> 13) | (v << 19)) & 0xffffffffUL; }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = (*seed * 0x24a1 + 0x24df) & 0xffffffffUL;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
static unsigned long g_MaxShoppersPerShop = 3;
struct CShop { int NumShoppers; };
static CShop* Find(CShop* shops, int n, unsigned long* seed)
{
    CShop* delightful[8]; int m = 0;
    for (int i = 0; i < n; ++i)
        if ((unsigned long)shops[i].NumShoppers < g_MaxShoppersPerShop) delightful[m++] = &shops[i];
    if (m == 0) return 0;
    return delightful[GFRandom(m, seed)];
}
int main()
{
    CShop shops[4] = { { 5 }, { 1 }, { 3 }, { 0 } };
    unsigned long seed = 7, check = 7;
    unsigned long index = GFRandom(2, &check);
    CShop* got = Find(shops, 4, &seed);
    bool ok = got == (index == 0 ? &shops[1] : &shops[3]) && seed == check;
    CShop full[2] = { { 3 }, { 9 } };
    unsigned long seed2 = 7;
    ok = ok && Find(full, 2, &seed2) == 0 && seed2 == 7 && Find(shops, 0, &seed2) == 0;
    if (!ok) { std::printf("00946f90_TEST FAIL\n"); return 1; }
    std::printf("00946f90_TEST PASS\n");
    return 0;
}
