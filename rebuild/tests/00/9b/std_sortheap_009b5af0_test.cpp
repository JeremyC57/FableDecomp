#include <cstdio>
#include <algorithm>
// Behaviour model: sort_heap over a heap of keyed pairs yields ascending keys.
struct P { unsigned long k; long a, b; };
static bool lessk(const P& x, const P& y) { return x.k < y.k; }
int main()
{
    P v[6] = { {5,0,0}, {1,0,0}, {9,0,0}, {3,0,0}, {7,0,0}, {2,0,0} };
    std::make_heap(v, v + 6, lessk);
    std::sort_heap(v, v + 6, lessk);
    for (int i = 1; i < 6; ++i) if (v[i - 1].k > v[i].k) { std::printf("009b5af0_TEST FAIL\n"); return 1; }
    std::printf("009b5af0_TEST PASS\n");
    return 0;
}