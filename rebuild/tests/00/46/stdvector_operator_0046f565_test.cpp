// behaviour test for 0x0046F565: vector assignment of records with user copy/assign: grow, shrink and extend
#include <cstdio>
#include <vector>
static int g_live = 0;
struct R { int v; R(int x = 0) : v(x) { ++g_live; } R(const R& o) : v(o.v) { ++g_live; } R& operator=(const R& o) { v = o.v; return *this; } virtual ~R() { --g_live; } };
int main()
{
    {
        std::vector<R> a(5, R(3)), b(2, R(1)), c(9, R(7));
        b = a; c = a;
        std::vector<R> d; d.reserve(8); d.push_back(R(2)); d = a;
        int ok = b.size() == 5 && b[4].v == 3 && c.size() == 5 && c[0].v == 3 && d.size() == 5 && d[2].v == 3;
        if (!ok) { std::printf("0046f565_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("0046f565_TEST FAIL\n"); return 1; }
    std::printf("0046f565_TEST PASS\n");
    return 0;
}