#include <cstdio>
struct K { unsigned char p[4]; unsigned long k; };
static bool __stdcall L(const K* a, const K* b) { if (a->k != b->k) return a->k < b->k; return a < b; }
int main()
{
    K x, y; x.k = 1; y.k = 2; if (!L(&x, &y) || L(&y, &x)) { std::printf("00c248e0_TEST FAIL\n"); return 1; }
    y.k = 1; if (L(&x, &y) != (&x < &y)) { std::printf("00c248e0_TEST FAIL\n"); return 1; }

    std::printf("00c248e0_TEST PASS\n");
    return 0;
}
