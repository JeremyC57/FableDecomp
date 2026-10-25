// Behaviour test for 0x007D14B0 std::__copy over CShoveHaPennyRegionDef (implicit def assignment, vptr untouched).
#include <cstdio>

struct Region {
    int vtag;                         // stands in for the vptr: assignment must not copy it
    long words[6];
    long PDefManager;
    unsigned long GlobalIndex;
    unsigned char Setup : 1;
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
    float Start, Score;
    Region& operator=(const Region& o)
    {
        for (int i = 0; i < 6; ++i) words[i] = o.words[i];
        PDefManager = o.PDefManager; GlobalIndex = o.GlobalIndex;
        Setup = o.Setup; Template = o.Template; DefaultValsApplied = o.DefaultValsApplied;
        Start = o.Start; Score = o.Score;
        return *this;
    }
};

static Region* copy_n(Region* first, Region* last, Region* result)
{
    for (int n = (int)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    Region a[7], b[9];
    for (int i = 0; i < 7; ++i) {
        a[i].vtag = 100 + i; a[i].GlobalIndex = i; a[i].Setup = i & 1; a[i].Template = (i >> 1) & 1;
        a[i].DefaultValsApplied = (i >> 2) & 1; a[i].Start = i * 1.5f; a[i].Score = -i * 1.0f;
        for (int k = 0; k < 6; ++k) a[i].words[k] = i * 10 + k;
        a[i].PDefManager = i;
    }
    for (int i = 0; i < 9; ++i) { b[i].vtag = 7; b[i].GlobalIndex = 999; }
    Region* end = copy_n(a, a + 7, b + 1);
    if (end != b + 8 || b[0].GlobalIndex != 999 || b[8].GlobalIndex != 999) { std::printf("007d14b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) {
        const Region& r = b[i + 1];
        if (r.vtag != 7 || r.GlobalIndex != (unsigned long)i || r.Setup != (i & 1) || r.Template != ((i >> 1) & 1)
            || r.DefaultValsApplied != ((i >> 2) & 1) || r.Start != i * 1.5f || r.words[5] != i * 10 + 5) {
            std::printf("007d14b0_TEST FAIL\n"); return 1;
        }
    }
    if (copy_n(a, a, b) != b) { std::printf("007d14b0_TEST FAIL\n"); return 1; }
    std::printf("007d14b0_TEST PASS\n");
    return 0;
}