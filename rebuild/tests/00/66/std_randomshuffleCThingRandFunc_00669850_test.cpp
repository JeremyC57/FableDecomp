// Behaviour test for 0x00669850 std::random_shuffle<CThing**, RandFunc> (model copy)
#include <cstdio>
static unsigned int Ror13(unsigned int v) { return (v >> 13) | (v << 19); }
static unsigned int GFRandom(unsigned int range, unsigned int* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = Ror13(*seed);
    return range ? *seed % range : 0;
}
struct RandFunc { unsigned int* Seed; unsigned int operator()(unsigned int n) { return GFRandom(n, Seed); } };
template <class It, class R> void Shuffle(It first, It last, R& rand)
{
    if (first == last) return;
    for (It i = first + 1; i != last; ++i) {
        It j = first + rand((unsigned int)(i - first) + 1);
        int* t = *i; *i = *j; *j = t;
    }
}
int main()
{
    int v[8]; int* p[8];
    for (int k = 0; k < 8; ++k) { v[k] = k; p[k] = &v[k]; }
    unsigned int seed = 12345;
    RandFunc r = { &seed };
    Shuffle(p, p + 8, r);
    bool ok = seed != 12345;                         // seven draws advanced the seed
    int seen[8] = { 0 };
    for (int k = 0; k < 8; ++k) seen[*p[k]]++;
    for (int k = 0; k < 8; ++k) ok = ok && seen[k] == 1;   // still a permutation
    // replay: the same seed gives the same order
    int* q[8]; for (int k = 0; k < 8; ++k) q[k] = &v[k];
    unsigned int seed2 = 12345; RandFunc r2 = { &seed2 };
    Shuffle(q, q + 8, r2);
    for (int k = 0; k < 8; ++k) ok = ok && q[k] == p[k];
    unsigned int seed3 = 7; RandFunc r3 = { &seed3 };
    Shuffle(q, q, r3); ok = ok && seed3 == 7;        // empty range draws nothing
    Shuffle(q, q + 1, r3); ok = ok && seed3 == 7;    // one element draws nothing
    std::printf(ok ? "00669850_TEST PASS\n" : "00669850_TEST FAIL\n");
    return ok ? 0 : 1;
}