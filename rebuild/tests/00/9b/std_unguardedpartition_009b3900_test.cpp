#include <cstdio>
// Behaviour model: after the unguarded partition everything before the returned split is not above the pivot and
// everything from it on is not below it.
struct E { unsigned long k; long v; };
static E* part(E* f, E* l, unsigned long p)
{
    for (;;) {
        while (f->k < p) ++f;
        --l;
        while (p < l->k) --l;
        if (!(f < l)) return f;
        E t = *f; *f = *l; *l = t;
        ++f;
    }
}
int main()
{
    E a[7] = { {5, 0}, {1, 1}, {9, 2}, {3, 3}, {7, 4}, {2, 5}, {8, 6} };
    E* s = part(a, a + 7, 5);
    for (E* p = a; p < s; ++p) if (p->k > 5) { std::printf("009b3900_TEST FAIL\n"); return 1; }
    for (E* p = s; p < a + 7; ++p) if (p->k < 5) { std::printf("009b3900_TEST FAIL\n"); return 1; }
    std::printf("009b3900_TEST PASS\n");
    return 0;
}