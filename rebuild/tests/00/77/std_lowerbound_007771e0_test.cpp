// Behaviour test for 0x007771E0: __lower_bound over sorted longs returns the first element not below the value
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

static long* LowerBound(long* first, long* last, const long& val)
{
    int len = (int)(last - first);
    while (len > 0) {
        int half = len >> 1; long* mid = first + half;
        if (*mid < val) { first = mid + 1; len = len - half - 1; } else len = half;
    }
    return first;
}
int main()
{
    long v[7] = { -5, -1, 0, 0, 3, 8, 8 };
    long k;
    k = -10; CHECK(LowerBound(v, v + 7, k) == v);
    k = 0;   CHECK(LowerBound(v, v + 7, k) == v + 2);
    k = 1;   CHECK(LowerBound(v, v + 7, k) == v + 4);
    k = 8;   CHECK(LowerBound(v, v + 7, k) == v + 5);
    k = 9;   CHECK(LowerBound(v, v + 7, k) == v + 7);
    if (!g_fail) printf("007771e0_TEST PASS\n");
    return g_fail;
}