#include <cstdio>
// Behaviour model: keep scan-visible things of the wanted type that pass the filter, in list order.
struct Thing { unsigned long flags; unsigned char bits; int id; };
static bool visible(const Thing* t) { unsigned char f = t->bits; return (f & 4) && (f & 8) && !(f & 1) && !(f & 0x20); }
static int collect(const Thing* things, int n, unsigned long mask, bool (*filter)(const Thing&), int* out)
{
    int k = 0;
    for (int i = 0; i < n; ++i)
        if (visible(&things[i]) && (things[i].flags & mask) == mask && filter(things[i])) out[k++] = things[i].id;
    return k;
}
static bool even(const Thing& t) { return (t.id & 1) == 0; }
int main()
{
    Thing t[6] = { {0x1F, 0x0C, 2}, {0x0F, 0x0C, 4}, {0x1F, 0x0D, 6}, {0x1F, 0x2C, 8}, {0x1F, 0x0C, 9}, {0x3F, 0x0C, 10} };
    int out[6];
    int k = collect(t, 6, 0x1F, even, out);
    if (k != 2 || out[0] != 2 || out[1] != 10) { std::printf("0092b2d0_TEST FAIL\n"); return 1; }
    if (collect(t, 6, 0x7, even, out) != 3) { std::printf("0092b2d0_TEST FAIL\n"); return 1; }
    std::printf("0092b2d0_TEST PASS\n");
    return 0;
}