#include <cstdio>
static unsigned int ROR13(unsigned int v) { return (v >> 13) | (v << 19); }
class CTCWound { public:
    unsigned int seed; long added;
    bool Add(float z) {
        long loc; if (z > 1.55f) loc = 1; else loc = 6;
        seed = seed * 0x24a1u + 0x24dfu; seed = ROR13(seed);
        if ((long)(seed % 100u) >= 95) { added = loc; return true; }
        return false;
    }
};
int main()
{
    CTCWound w = { 12345u, 0 };
    int hits = 0, hi = 0, lo = 0;
    for (int i = 0; i < 20000; ++i) { w.added = 0; if (w.Add(i & 1 ? 2.0f : 1.0f)) { ++hits; if (w.added == 1) ++hi; else if (w.added == 6) ++lo; } }
    if (hits == 0 || hits > 3000 || hi + lo != hits || hi == 0 || lo == 0) { std::printf("00757f90_TEST FAIL\n"); return 1; }
    std::printf("00757f90_TEST PASS\n");
    return 0;
}