#include <cstdio>
// Behaviour model of CTCSoundPlayer::PlayAttachedSample (0x0066B300): the generator steps seed = ror13(seed * 0x24A1 + 0x24DF), the
// index is min + (r % (max - min + 1)) (0 offset for an empty range) and is passed on.
static unsigned long Ror13(unsigned long v) { v &= 0xffffffffUL; return ((v >> 13) | (v << 19)) & 0xffffffffUL; }
struct Rng { unsigned long seed; unsigned long Next() { seed = (seed * 0x24a1UL + 0x24dfUL) & 0xffffffffUL; seed = Ror13(seed); return seed; } };
static long Pick(Rng& r, long mn, long mx) { unsigned long n = (unsigned long)(mx - mn + 1); unsigned long v = r.Next(); return mn + (long)(n ? v % n : 0); }
int main()
{
    Rng r = { 12345 };
    for (int i = 0; i < 100; ++i) {
        long k = Pick(r, 3, 7);
        if (k < 3 || k > 7) { std::printf("0066b300_TEST FAIL\n"); return 1; }
    }
    Rng a = { 1 }, b = { 1 };
    if (Pick(a, 0, 9) != Pick(b, 0, 9) || Pick(a, 4, 4) != 4) { std::printf("0066b300_TEST FAIL\n"); return 1; }
    std::printf("0066b300_TEST PASS\n");
    return 0;
}