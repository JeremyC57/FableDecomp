// Behaviour test for 0x00513270 std::random_shuffle<CThingAICreature**, RandFunc>(first, last, rand): for each i after
// first, *i is swapped with first[rand(i - first + 1)] where rand truncates the generator's float in [0, n); the
// result is a permutation of the input, the generator is asked once per element after the first, and an empty or
// one-element range is left untouched.
#include <cstdio>

static int g_calls = 0;

static float float_random(float range, unsigned long* seed)   // LCG stand-in for GFFloatRandom
{
    ++g_calls;
    *seed = *seed * 0x24a1 + 0x24df;
    return range * (float)(*seed % 1000) / 1000.0f;
}

struct RandFunc {
    unsigned long* Seed;
    long operator()(long n) { return (long)float_random((float)n, Seed); }
};

static void model(int** first, int** last, RandFunc& rand)
{
    if (first == last) return;
    for (int** i = first + 1; i != last; ++i) {
        int** j = first + rand((long)(i - first) + 1);
        int* tmp = *i;
        *i = *j;
        *j = tmp;
    }
}

int main()
{
    int items[6];
    int* v[6];
    for (int k = 0; k < 6; ++k) v[k] = &items[k];
    unsigned long seed = 12345;
    RandFunc r = { &seed };
    model(v, v + 6, r);
    if (g_calls != 5) { std::printf("00513270_TEST FAIL\n"); return 1; }
    for (int k = 0; k < 6; ++k) {
        int seen = 0;
        for (int m = 0; m < 6; ++m) if (v[m] == &items[k]) ++seen;
        if (seen != 1) { std::printf("00513270_TEST FAIL\n"); return 1; }
    }
    int* single[1] = { &items[0] };
    model(single, single + 1, r);
    model(single, single, r);
    if (g_calls != 5 || single[0] != &items[0]) { std::printf("00513270_TEST FAIL\n"); return 1; }
    std::printf("00513270_TEST PASS\n");
    return 0;
}