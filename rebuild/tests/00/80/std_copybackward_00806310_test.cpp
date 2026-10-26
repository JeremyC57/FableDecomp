// behaviour test for 0x00806310 std::__copy_backward over 16-byte pair<C3DVector, long> elements: [first, last) is copied back to front so that it ends at result (correct for an overlapping shift to the right), the start of the copy is returned, and an empty range returns result unchanged
#include <cstdio>
struct Elem { int W[4]; };
static Elem* CopyBackward(Elem* first, Elem* last, Elem* result)   // the function under test (STLport 4.6 shape)
{
    for (int n = (int)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
int main()
{
    int fail = 0;
    Elem a[8];
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 4; ++j) a[i].W[j] = i * 10 + j;
    if (CopyBackward(a + 1, a + 5, a + 7) != a + 3) fail |= 1;          // overlapping shift right by two
    for (int i = 3; i < 7; ++i) if (a[i].W[0] != (i - 2) * 10 || a[i].W[3] != (i - 2) * 10 + 3) fail |= 2;
    if (a[7].W[0] != 70 || a[0].W[0] != 0) fail |= 4;
    if (CopyBackward(a + 2, a + 2, a + 6) != a + 6) fail |= 8;          // empty range
    if (fail) { std::printf("00806310_TEST FAIL %d\n", fail); return 1; }
    std::printf("00806310_TEST PASS\n");
    return 0;
}