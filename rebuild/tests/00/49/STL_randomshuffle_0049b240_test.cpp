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
static int g_live = 0;
class CCharString { public:
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) { ++g_live; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& o) { PStringData = o.PStringData; return *this; }
};

static void random_shuffle(CCharString* first, CCharString* last, RandFunc& rand)
{
    if (first == last) return;
    for (CCharString* i = first + 1; i != last; ++i)
        __iter_swap_aux(i, first + rand((long)(i - first) + 1));
}

static const char* g_words[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p",
                                 "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "0", "1", "2", "3" };

int main()
{
    unsigned long seed = 99;
    RandFunc rand; rand.Seed = &seed;
    random_shuffle((CCharString*)0, (CCharString*)0, rand);
    if (g_draws != 0 || seed != 99) { std::printf("0049b240_TEST FAIL\n"); return 1; }
    for (int n = 1; n <= 30; ++n) {
        CCharString a[30] = { g_words[0], g_words[1], g_words[2], g_words[3], g_words[4], g_words[5], g_words[6], g_words[7],
                              g_words[8], g_words[9], g_words[10], g_words[11], g_words[12], g_words[13], g_words[14], g_words[15],
                              g_words[16], g_words[17], g_words[18], g_words[19], g_words[20], g_words[21], g_words[22], g_words[23],
                              g_words[24], g_words[25], g_words[26], g_words[27], g_words[28], g_words[29] };
        const char* b[30];
        for (int i = 0; i < 30; ++i) b[i] = g_words[i];
        unsigned long s1 = 5u + (unsigned long)n, s2 = s1;
        RandFunc r1; r1.Seed = &s1;
        int live = g_live;
        g_draws = 0;
        random_shuffle(a, a + n, r1);
        if (g_live != live || g_draws != n - 1) { std::printf("0049b240_TEST FAIL\n"); return 1; }   // swap temporaries destroyed
        for (int i = 1; i < n; ++i) {
            long j = (long)GFFloatRandom((float)(i + 1), &s2);
            const char* t = b[i]; b[i] = b[j]; b[j] = t;
        }
        for (int i = 0; i < 30; ++i) if (a[i].PStringData != b[i]) { std::printf("0049b240_TEST FAIL\n"); return 1; }
        if (s1 != s2) { std::printf("0049b240_TEST FAIL\n"); return 1; }
    }
    std::printf("0049b240_TEST PASS\n");
    return 0;
}