// behaviour test for 0x00565A90 std::__copy [std::pair<float, CWideString>]: last - first elements are assigned front to back into the range starting at result (overlap-safe to the left), the advanced result is returned
#include <cstdio>
static int gAssigns = 0;
struct Member { int v; Member& operator=(const Member& o) { ++gAssigns; v = o.v; return *this; } };
struct Elem { int key; Member m; };
static Elem* CopyForward(Elem* first, Elem* last, Elem* result)    // the function under test
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
    int fail = 0;
    Elem a[6];
    for (int i = 0; i < 6; ++i) { a[i].key = 10 + i; a[i].m.v = 100 + i; }
    Elem* r = CopyForward(a + 2, a + 6, a);                  // shift [2,6) left by two, overlapping
    if (r != a + 4) fail |= 1;
    for (int i = 0; i < 4; ++i) if (a[i].key != 12 + i || a[i].m.v != 102 + i) fail |= 2;
    if (gAssigns != 4) fail |= 4;
    if (CopyForward(a, a, a + 3) != a + 3 || gAssigns != 4) fail |= 8;
    if (fail) { std::printf("00565a90_TEST FAIL %d\n", fail); return 1; }
    std::printf("00565a90_TEST PASS\n");
    return 0;
}