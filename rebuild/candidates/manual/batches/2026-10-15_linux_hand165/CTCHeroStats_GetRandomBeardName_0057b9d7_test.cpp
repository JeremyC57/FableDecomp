#include <cstdio>
#include <cstring>
// Behaviour model of CTCHeroStats::GetRandomBeardName (0057b9d7): builds a 5-entry name list whose last entry is empty and
// returns the entry chosen by GFRandom(5, seed), here a deterministic LCG.
static unsigned int Random(unsigned int range, unsigned int* seed) { *seed = *seed * 1103515245u + 12345u; return (*seed >> 16) % range; }
static const char* Pick(unsigned int* seed)
{
    const char* names[5];
    names[0] = "A"; names[1] = "B"; names[2] = "C"; names[3] = "D"; names[5 - 1] = "";
    return names[Random(5, seed)];
}
int main()
{
    unsigned int seed = 1;
    int empty = 0, total = 0;
    for (int i = 0; i < 200; ++i) {
        unsigned int before = seed;
        const char* n = Pick(&seed);
        if (seed == before) { std::printf("0057b9d7_TEST FAIL\n"); return 1; }
        if (std::strlen(n) == 0) ++empty;
        ++total;
    }
    if (empty == 0 || empty == total) { std::printf("0057b9d7_TEST FAIL\n"); return 1; }
    std::printf("0057b9d7_TEST PASS\n");
    return 0;
}
