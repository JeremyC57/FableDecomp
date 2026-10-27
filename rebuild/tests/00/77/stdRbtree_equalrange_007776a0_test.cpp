// Behaviour test for 0x007776A0: tree equal_range on a signed key: [lower_bound, upper_bound)
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

static int Lower(const long* v, int n, long k) { int i = 0; while (i < n && v[i] < k) ++i; return i; }
static int Upper(const long* v, int n, long k) { int i = 0; while (i < n && !(k < v[i])) ++i; return i; }
struct SRange { int first, second; };
static SRange EqualRange(const long* v, int n, long k) { SRange r; r.first = Lower(v, n, k); r.second = Upper(v, n, k); return r; }
int main()
{
    long v[6] = { -3, 1, 1, 1, 4, 9 };
    SRange r = EqualRange(v, 6, 1); CHECK(r.first == 1 && r.second == 4);
    r = EqualRange(v, 6, 2); CHECK(r.first == 4 && r.second == 4);
    r = EqualRange(v, 6, -3); CHECK(r.first == 0 && r.second == 1);
    r = EqualRange(v, 6, 10); CHECK(r.first == 6 && r.second == 6);
    if (!g_fail) printf("007776a0_TEST PASS\n");
    return g_fail;
}