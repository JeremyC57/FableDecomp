// Behaviour test for 0x007D17A0 std::__copy_backward over CShoveHaPennyRegionDef (overlapping shift up).
#include <cstdio>

struct Region {
    int vtag;
    unsigned long GlobalIndex;
    unsigned char Setup : 1;
    float Start, Score;
    Region& operator=(const Region& o)
    {
        GlobalIndex = o.GlobalIndex; Setup = o.Setup; Start = o.Start; Score = o.Score;
        return *this;
    }
};

static Region* copy_backward_n(Region* first, Region* last, Region* result)
{
    for (int n = (int)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}

int main()
{
    Region a[10];
    for (int i = 0; i < 10; ++i) { a[i].vtag = i; a[i].GlobalIndex = i; a[i].Setup = i & 1; a[i].Start = (float)i; a[i].Score = 0; }
    Region* r = copy_backward_n(a + 1, a + 7, a + 9);     // shift [1,7) up by two: overlapping
    if (r != a + 3) { std::printf("007d17a0_TEST FAIL\n"); return 1; }
    for (int i = 3; i < 9; ++i)
        if (a[i].GlobalIndex != (unsigned long)(i - 2) || a[i].Setup != ((i - 2) & 1) || a[i].vtag != i) {
            std::printf("007d17a0_TEST FAIL\n"); return 1;
        }
    if (a[0].GlobalIndex != 0 || a[9].GlobalIndex != 9 || copy_backward_n(a, a, a + 4) != a + 4) {
        std::printf("007d17a0_TEST FAIL\n"); return 1;
    }
    std::printf("007d17a0_TEST PASS\n");
    return 0;
}