// behaviour test for 0x0047978D: growth insert copies head, inserted value(s) and tail, keeping object counts balanced
#include <cstdio>
#include <vector>
static int g_live = 0;
struct R { int v; R(int x = 0) : v(x) { ++g_live; } R(const R& o) : v(o.v) { ++g_live; } virtual ~R() { --g_live; } };
int main()
{
    {
        std::vector<R> v; for (int i = 0; i < 3; ++i) v.push_back(R(i));
        v.insert(v.begin() + 1, 4, R(9));
        if (v.size() != 7 || v[0].v != 0 || v[1].v != 9 || v[5].v != 1 || v[6].v != 2) { std::printf("0047978d_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("0047978d_TEST FAIL\n"); return 1; }
    std::printf("0047978d_TEST PASS\n");
    return 0;
}