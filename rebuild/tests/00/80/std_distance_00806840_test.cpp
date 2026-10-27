// Behaviour test for 0x00806840: __distance on 4-byte random-access iterators
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

template <class I> static int Distance(const I& first, const I& last) { return (int)(last - first); }
int main()
{
    long v[10]; long* a = v + 2; long* b = v + 9;
    CHECK(Distance(a, b) == 7 && Distance(b, a) == -7 && Distance(a, a) == 0);
    if (!g_fail) printf("00806840_TEST PASS\n");
    return g_fail;
}