// behaviour test for 0x00CD2C7C std::__copy_backward over 0x10-byte elements: last - first assignments from the back
// (so an overlapping shift right is safe), the returned pointer is result - n, an empty range assigns nothing
#include <cstdio>
static int gAssigns = 0;
struct Elem { int Words[4]; Elem& operator=(const Elem& o) { for (int i = 0; i < 4; ++i) Words[i] = o.Words[i]; ++gAssigns; return *this; } };
static Elem* CopyBackward(Elem* first, Elem* last, Elem* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
int main()
{
    int fail = 0;
    Elem a[5];
    for (int i = 0; i < 5; ++i) for (int w = 0; w < 4; ++w) a[i].Words[w] = i * 10 + w;
    Elem* r = CopyBackward(a, a + 3, a + 4);                 // shift [0,3) right by one
    if (r != a + 1 || gAssigns != 3) fail |= 1;
    for (int i = 1; i < 4; ++i) if (a[i].Words[0] != (i - 1) * 10) fail |= 2;
    if (a[0].Words[0] != 0 || a[4].Words[0] != 40) fail |= 4;
    if (CopyBackward(a, a, a + 2) != a + 2 || gAssigns != 3) fail |= 8;
    if (fail) { std::printf("00cd2c7c_TEST FAIL %d\n", fail); return 1; }
    std::printf("00cd2c7c_TEST PASS\n");
    return 0;
}