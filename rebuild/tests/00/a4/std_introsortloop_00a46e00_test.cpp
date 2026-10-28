// Behaviour test for 0x00A46E00 std::__introsort_loop over C3DSkeletalBoneMorph (by C3DSkeletalBoneMorph::CompareCRCs)
#include <cstdio>
#include <cstdlib>

struct BoneMorph { int Name; unsigned int CRC; float X, Y, Z; };
static bool CompareCRCs(const BoneMorph& a, const BoneMorph& b) { return a.CRC < b.CRC; }
static int g_partialSorts = 0;

static const BoneMorph& Median(const BoneMorph& a, const BoneMorph& b, const BoneMorph& c)
{
    if (CompareCRCs(a, b)) {
        if (CompareCRCs(b, c)) return b;
        else if (CompareCRCs(a, c)) return c;
        else return a;
    }
    else if (CompareCRCs(a, c)) return a;
    else if (CompareCRCs(b, c)) return c;
    else return b;
}
static BoneMorph* UnguardedPartition(BoneMorph* first, BoneMorph* last, BoneMorph pivot)   // 0x00A461D0
{
    for (;;) {
        while (CompareCRCs(*first, pivot)) ++first;
        --last;
        while (CompareCRCs(pivot, *last)) --last;
        if (!(first < last)) return first;
        BoneMorph t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void PartialSort(BoneMorph* first, BoneMorph* last)            // 0x00A46940 stand-in (whole range)
{
    ++g_partialSorts;
    for (BoneMorph* i = first + 1; i < last; ++i)
        for (BoneMorph* j = i; j > first && CompareCRCs(*j, *(j - 1)); --j) { BoneMorph t = *j; *j = *(j - 1); *(j - 1) = t; }
}
static void IntrosortLoop(BoneMorph* first, BoneMorph* last, int depth)  // the row's body
{
    while (last - first > 16) {
        if (depth == 0) { PartialSort(first, last); return; }
        --depth;
        BoneMorph* cut = UnguardedPartition(first, last, Median(*first, *(first + (last - first) / 2), *(last - 1)));
        IntrosortLoop(cut, last, depth);
        last = cut;
    }
}
static int Sorted(const BoneMorph* a, int n) { for (int i = 1; i < n; ++i) if (a[i].CRC < a[i - 1].CRC) return 0; return 1; }

int main()
{
    int ok = 1;
    BoneMorph a[300];
    std::srand(529);
    for (int i = 0; i < 300; ++i) { a[i].Name = i; a[i].CRC = (unsigned int)(std::rand() % 1000); }
    IntrosortLoop(a, a + 300, 2 * 8);
    PartialSort(a, a + 300);                                           // the final insertion sort's job
    ok &= Sorted(a, 300);
    for (int i = 0; i < 300; ++i) a[i].CRC = (unsigned int)(300 - i);
    g_partialSorts = 0;
    IntrosortLoop(a, a + 300, 0);                                      // depth exhausted: heap/partial sort fallback
    ok &= g_partialSorts == 1 && Sorted(a, 300);
    for (int i = 0; i < 10; ++i) a[i].CRC = (unsigned int)(10 - i);
    g_partialSorts = 0;
    IntrosortLoop(a, a + 10, 4);                                       // 16 or fewer: left for the final insertion sort
    ok &= g_partialSorts == 0 && a[0].CRC == 10u;
    if (ok) std::printf("00a46e00_TEST PASS\n"); else std::printf("00a46e00_TEST FAIL\n");
    return ok ? 0 : 1;
}