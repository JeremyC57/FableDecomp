#include <cstdio>
#include <cmath>
#include <cstring>

// Behaviour model of GFFloatRandom (0x004978A0) + GFROR13 (0x00497890) and the STLport random_shuffle with RandFunc.
static unsigned long Ror13(unsigned long v) { v &= 0xFFFFFFFFUL; return ((v >> 13) | (v << 19)) & 0xFFFFFFFFUL; }

static float GFFloatRandom(float range, unsigned long* seed)
{
    *seed = (*seed * 0x24A1UL + 0x24DFUL) & 0xFFFFFFFFUL;
    double scaled = 65536.0 * range;
    *seed = Ror13(*seed);
    if (scaled > 0.0001)
        return (float)(std::fmod((double)*seed, scaled) * (1.0 / 65536.0));
    return 0.0f;
}

static int g_draws = 0;

struct RandFunc {
    unsigned long* Seed;
    long operator()(long n) { ++g_draws; return (long)GFFloatRandom((float)n, Seed); }
};

template <class _Tp>
static void __iter_swap_aux(_Tp* __a, _Tp* __b)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
static void random_shuffle(long* first, long* last, RandFunc& rand)
{
    if (first == last) return;
    for (long* i = first + 1; i != last; ++i)
        __iter_swap_aux(i, first + rand((long)(i - first) + 1));
}

int main()
{
    unsigned long seed = 1234;
    RandFunc rand; rand.Seed = &seed;
    random_shuffle((long*)0, (long*)0, rand);               // empty: no draw
    long one[1] = { 7 };
    random_shuffle(one, one + 1, rand);                      // single: no draw
    if (g_draws != 0 || seed != 1234 || one[0] != 7) { std::printf("0049b1e0_TEST FAIL\n"); return 1; }
    for (int n = 2; n <= 50; ++n) {
        long a[50], b[50];
        for (int i = 0; i < n; ++i) a[i] = b[i] = 100 + i;
        unsigned long s1 = (unsigned long)n * 77u, s2 = s1;
        RandFunc r1; r1.Seed = &s1; RandFunc r2; r2.Seed = &s2;
        g_draws = 0;
        random_shuffle(a, a + n, r1);
        if (g_draws != n - 1) { std::printf("0049b1e0_TEST FAIL\n"); return 1; }
        // reference: the same Fisher-Yates draw sequence written out
        for (int i = 1; i < n; ++i) {
            long j = (long)GFFloatRandom((float)(i + 1), &s2);
            if (j < 0 || j > i) { std::printf("0049b1e0_TEST FAIL\n"); return 1; }
            long t = b[i]; b[i] = b[j]; b[j] = t;
        }
        if (std::memcmp(a, b, sizeof(long) * n) != 0 || s1 != s2) { std::printf("0049b1e0_TEST FAIL\n"); return 1; }
        int seen[50] = { 0 };
        for (int i = 0; i < n; ++i) { if (a[i] < 100 || a[i] >= 100 + n || seen[a[i] - 100]) { std::printf("0049b1e0_TEST FAIL\n"); return 1; } seen[a[i] - 100] = 1; }
    }
    std::printf("0049b1e0_TEST PASS\n");
    return 0;
}