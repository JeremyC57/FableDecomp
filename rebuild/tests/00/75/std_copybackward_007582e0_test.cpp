// behaviour test for 0x007582E0 std::__copy_backward over 0x2C-byte trivially-assigned records: last - first copies
// from the back (so an overlapping shift right is safe), the returned pointer is result - n, an empty range copies nothing
#include <cstdio>
struct Elem { int Words[11]; };
static Elem* CopyBackward(Elem* first, Elem* last, Elem* result)   // the function under test
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
int main()
{
    int fail = 0;
    if (sizeof(Elem) != 0x2Cu) fail |= 16;
    Elem a[5];
    for (int i = 0; i < 5; ++i) for (int w = 0; w < 11; ++w) a[i].Words[w] = i * 100 + w;
    Elem* r = CopyBackward(a, a + 3, a + 4);                 // shift [0,3) right by one
    if (r != a + 1) fail |= 1;
    for (int i = 1; i < 4; ++i) for (int w = 0; w < 11; ++w) if (a[i].Words[w] != (i - 1) * 100 + w) fail |= 2;
    if (a[0].Words[0] != 0 || a[4].Words[11 - 1] != 400 + 11 - 1) fail |= 4;
    if (CopyBackward(a, a, a + 2) != a + 2) fail |= 8;
    if (fail) { std::printf("007582e0_TEST FAIL %d\n", fail); return 1; }
    std::printf("007582e0_TEST PASS\n");
    return 0;
}