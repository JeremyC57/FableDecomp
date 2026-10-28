// Behaviour test: CAIStateGroup_HandyMan::AcquireWorkSpot (0x0091B010) -- search the 60-unit square round the creature
// with the work-spot property and a 20-unit range, drop the current spot from the list, fail (spot cleared) when
// nothing is left, else take the GFRandom-indexed one (seed = ROR13(seed * 0x24A1 + 0x24DF), index = r % count).
#include <cstdio>
static unsigned long GFROR13(unsigned long v) { v &= 0xffffffffUL; return ((v >> 13) | (v << 19)) & 0xffffffffUL; }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = (*seed * 0x24a1 + 0x24df) & 0xffffffffUL;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
class CThing { public: float X, Y; int Property; };
static CThing g_things[5];
static int g_numThings;
static int g_boxTLX, g_boxBRX;
struct CHandyMan {
    CThing* WorkSpot; float CX, CY; unsigned long Seed;
    bool AcquireWorkSpot()
    {
        CThing* old = WorkSpot;
        WorkSpot = 0;
        long x = (long)CX, y = (long)CY;
        g_boxTLX = x - 30; g_boxBRX = x + 30;
        CThing* list[5]; int n = 0;
        for (int i = 0; i < g_numThings; ++i) {
            CThing* t = &g_things[i];
            float dx = t->X - CX, dy = t->Y - CY;
            if (t->Property == 0x4000000 && dx * dx + dy * dy <= 20.0f * 20.0f && t->X >= x - 30 && t->X <= x + 30
                && t->Y >= y - 30 && t->Y <= y + 30)
                list[n++] = t;
        }
        int m = 0;
        for (int i = 0; i < n; ++i) if (list[i] != old) list[m++] = list[i];
        if (m == 0) return false;
        long index = (long)GFRandom(m, &Seed);
        WorkSpot = list[index];
        return true;
    }
};
int main()
{
    g_numThings = 4;
    CThing a = { 1, 1, 0x4000000 }, b = { 5, 5, 0x4000000 }, c = { 50, 50, 0x4000000 }, d = { 2, 2, 1 };
    g_things[0] = a; g_things[1] = b; g_things[2] = c; g_things[3] = d;
    CHandyMan h = { &g_things[0], 0.0f, 0.0f, 1 };
    unsigned long seed = 1;
    unsigned long expect = GFRandom(1, &seed);              // only g_things[1] remains after dropping the old spot
    bool ok = h.AcquireWorkSpot() && h.WorkSpot == &g_things[1] && expect == 0 && h.Seed == seed && g_boxTLX == -30 && g_boxBRX == 30;
    g_numThings = 1;
    CHandyMan e = { &g_things[0], 0.0f, 0.0f, 1 };
    ok = ok && !e.AcquireWorkSpot() && e.WorkSpot == 0 && e.Seed == 1;
    if (!ok) { std::printf("0091b010_TEST FAIL\n"); return 1; }
    std::printf("0091b010_TEST PASS\n");
    return 0;
}