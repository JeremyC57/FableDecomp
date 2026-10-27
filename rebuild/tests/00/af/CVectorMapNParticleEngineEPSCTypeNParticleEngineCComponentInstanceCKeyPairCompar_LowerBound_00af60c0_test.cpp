#include <cstdio>
// Behaviour model: lower bound over sorted keys.
struct E { long k; char pad[20]; };
static const E* lb(const E* f, const E* l, long key)
{
    int count = l - f;
    while (count > 0) { int half = count / 2; const E* m = f + half; if (m->k < key) { f = m + 1; count -= half + 1; } else count = half; }
    return f;
}
int main()
{
    E e[5] = { {1}, {3}, {3}, {7}, {9} };
    if (lb(e, e + 5, 3) != e + 1 || lb(e, e + 5, 4) != e + 3 || lb(e, e + 5, 10) != e + 5 || lb(e, e + 5, 0) != e) { std::printf("00af60c0_TEST FAIL\n"); return 1; }
    std::printf("00af60c0_TEST PASS\n");
    return 0;
}