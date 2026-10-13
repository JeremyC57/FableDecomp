#include <cstdio>
struct Tg {};
static long* __fastcall F(long* f, long* l, const long& v, Tg) { long n; for (n = (l - f) >> 2; 0 < n; --n) { if (*f == v) return f; ++f; if (*f == v) return f; ++f; if (*f == v) return f; ++f; if (*f == v) return f; ++f; } switch (l - f) { case 3: if (*f == v) return f; ++f; case 2: if (*f == v) return f; ++f; case 1: if (*f == v) return f; default: break; } return l; }
int main()
{
    long a[7] = { 1, 2, 3, 4, 5, 6, 7 }; long k = 6, m = 9; if (F(a, a + 7, k, Tg()) != a + 5 || F(a, a + 7, m, Tg()) != a + 7) { std::printf("0091f680_TEST FAIL\n"); return 1; }

    std::printf("0091f680_TEST PASS\n");
    return 0;
}
