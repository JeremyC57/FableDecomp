#include <cstdio>
#include <vector>
// Behaviour model: Insert appends the pair and marks the map dirty.
struct P { unsigned long k; long a, b; };
struct M { std::vector<P> d; bool dirty; void Insert(unsigned long k, long a, long b) { P p = { k, a, b }; d.push_back(p); dirty = true; } };
int main()
{
    M m; m.dirty = false;
    for (unsigned long i = 0; i < 5; ++i) m.Insert(10 - i, (long)i, 0);
    if (m.d.size() != 5 || !m.dirty || m.d[4].k != 6 || m.d[0].a != 0) { std::printf("0081d8a0_TEST FAIL\n"); return 1; }
    std::printf("0081d8a0_TEST PASS\n");
    return 0;
}