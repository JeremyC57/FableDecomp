#include <cstdio>
// Behaviour model: every element of [first, last) becomes value; nothing outside changes.
struct E { long a; int b; };
int main()
{
    E v[5] = { {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5} };
    E x = { 9, 8 };
    for (E* f = v + 1; f != v + 4; ++f) *f = x;
    if (v[0].a != 1 || v[1].a != 9 || v[3].b != 8 || v[4].a != 5) { std::printf("00728c10_TEST FAIL\n"); return 1; }
    std::printf("00728c10_TEST PASS\n");
    return 0;
}