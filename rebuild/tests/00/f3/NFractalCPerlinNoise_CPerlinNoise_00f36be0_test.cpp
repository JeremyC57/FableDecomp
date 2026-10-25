// Behaviour test for NFractal::CPerlinNoise::CPerlinNoise(unsigned long) (0x00F36BE0): the constructor runs Initialise(seed).
#include <cstdio>

class CPerlinNoise {
public:
    long  Permutation[0x100];
    float Tables[0x300];
    unsigned long LastSeed;
    CPerlinNoise(unsigned long seed) { Initialise(seed); }
    void Initialise(unsigned long seed)
    {
        // stand-in for 0x00F35ED0: identity permutation shuffled by an LCG
        LastSeed = seed;
        for (long i = 0; i < 0x100; ++i)
            Permutation[i] = i;
        unsigned long r = seed;
        for (long i = 0; i < 0x100; ++i) {
            r = r * 0x24A1 + 0x24DF;
            long j = (long)(r & 0xFF);
            long t = Permutation[i]; Permutation[i] = Permutation[j]; Permutation[j] = t;
        }
    }
};

int main()
{
    static CPerlinNoise a(0), b(0), c(7);
    if (a.LastSeed != 0 || c.LastSeed != 7) { std::printf("00f36be0_TEST FAIL\n"); return 1; }
    long sum = 0;
    for (int i = 0; i < 0x100; ++i) {
        if (a.Permutation[i] != b.Permutation[i]) { std::printf("00f36be0_TEST FAIL\n"); return 1; }
        sum += a.Permutation[i];
    }
    if (sum != 0xFF * 0x100 / 2) { std::printf("00f36be0_TEST FAIL\n"); return 1; }
    std::printf("00f36be0_TEST PASS\n");
    return 0;
}