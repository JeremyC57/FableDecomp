// behaviour test for 0x00AD78D0 std::fill<QuadricOptimiserInternals::COptimisedVertex*, QuadricOptimiserInternals::COptimisedVertex >: every element of [first, last) is assigned the value exactly once, in order, and nothing outside the range is touched; an empty range assigns nothing
#include <cstdio>
static int gAssigns = 0, gOrder[8], gN = 0;
struct Elem {
    int Tag, Pos;
    Elem& operator=(const Elem& o) { ++gAssigns; if (gN < 8) gOrder[gN++] = Pos; Tag = o.Tag; return *this; }
};
template <class It, class T> void Fill(It first, It last, const T& v)   // the function under test
{
    for ( ; first != last; ++first)
        *first = v;
}
int main()
{
    int fail = 0;
    Elem a[5];
    for (int i = 0; i < 5; ++i) { a[i].Tag = -1; a[i].Pos = i; }
    Elem v; v.Tag = 7; v.Pos = 99;
    Fill(a + 1, a + 4, v);
    if (gAssigns != 3 || gOrder[0] != 1 || gOrder[1] != 2 || gOrder[2] != 3) fail |= 1;
    if (a[0].Tag != -1 || a[4].Tag != -1 || a[1].Tag != 7 || a[2].Tag != 7 || a[3].Tag != 7) fail |= 2;
    if (a[1].Pos != 1) fail |= 4;
    Fill(a, a, v);
    if (gAssigns != 3) fail |= 8;
    if (fail) { std::printf("00ad78d0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00ad78d0_TEST PASS\n");
    return 0;
}