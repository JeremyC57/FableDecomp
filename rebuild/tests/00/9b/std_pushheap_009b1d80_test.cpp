#include <cstdio>
// Behaviour model: sift-up in a max-heap keyed by the unsigned first member.
struct E { unsigned long k; long v; };
static void push(E* f, int hole, int top, E val)
{
    int parent = (hole - 1) / 2;
    while (hole > top && f[parent].k < val.k) { f[hole] = f[parent]; hole = parent; parent = (hole - 1) / 2; }
    f[hole] = val;
}
int main()
{
    E h[5] = { {9, 0}, {7, 1}, {8, 2}, {3, 3}, {0, 0} };
    E x = { 10, 4 };
    push(h, 4, 0, x);
    if (h[0].k != 10 || h[1].k != 9 || h[4].k != 7 || h[0].v != 4) { std::printf("009b1d80_TEST FAIL\n"); return 1; }
    E y = { 0xFFFFFFF0u, 5 };
    E g[2] = { {5, 0}, {0, 0} };
    push(g, 1, 0, y);
    if (g[0].k != 0xFFFFFFF0u || g[1].k != 5) { std::printf("009b1d80_TEST FAIL\n"); return 1; }
    std::printf("009b1d80_TEST PASS\n");
    return 0;
}