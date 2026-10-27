// behaviour test for 0x004EDDF9 uninitialized_fill_n (model: n copies constructed, the end returned)
#include <cstdio>
#include <new>
static int g_copies = 0;
struct E { int v; E(int x) : v(x) { } E(const E& o) : v(o.v) { ++g_copies; } };
int main()
{
    static double raw[8];
    E* first = (E*)raw; E x(7); E* cur = first;
    for (unsigned int n = 3; n > 0; --n, ++cur) new (cur) E(x);
    int ok = cur == first + 3 && first[2].v == 7 && g_copies == 3;
    if (!ok) { std::printf("004eddf9_TEST FAIL\n"); return 1; }
    std::printf("004eddf9_TEST PASS\n");
    return 0;
}