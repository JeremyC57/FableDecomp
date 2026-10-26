// behaviour test for 0x00AE76A0 std::__copy_backward [C3DGroup2]: last - first elements are assigned back to front into the range ending at result (overlap-safe to the right), the result moved back is returned
#include <cstdio>
static int gAssigns = 0;
struct Member { int v; Member& operator=(const Member& o) { ++gAssigns; v = o.v; return *this; } };
struct Elem { int key; Member m; };
static Elem* CopyBackward(Elem* first, Elem* last, Elem* result)   // the function under test
{
    for (int n = (int)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
int main()
{
    int fail = 0;
    Elem a[6];
    for (int i = 0; i < 6; ++i) { a[i].key = 10 + i; a[i].m.v = 100 + i; }
    Elem* r = CopyBackward(a, a + 4, a + 6);                 // shift [0,4) right by two, overlapping
    if (r != a + 2) fail |= 1;
    for (int i = 0; i < 4; ++i) if (a[i + 2].key != 10 + i || a[i + 2].m.v != 100 + i) fail |= 2;
    if (gAssigns != 4) fail |= 4;
    if (CopyBackward(a, a, a + 3) != a + 3 || gAssigns != 4) fail |= 8;
    if (fail) { std::printf("00ae76a0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00ae76a0_TEST PASS\n");
    return 0;
}