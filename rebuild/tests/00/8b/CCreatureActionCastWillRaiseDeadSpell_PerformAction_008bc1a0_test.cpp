// Behaviour test for 0x008BC1A0 CCreatureAction_CastWillRaiseDeadSpell::PerformAction (see src/008bc1a0.cpp): of the
// candidate positions only those whose 2D distance from the caster is at least radius + 0.3 are kept (by index), one is
// picked with GFRandom (seed = seed*0x24a1 + 0x24df rotated right 13, then modulo the count), and nothing is raised when
// none is valid.
#include <cstdio>
struct V { float X, Y, Z; };
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = Ror13(*seed);
    return range ? *seed % range : 0;
}
static long Pick(const V* positions, long count, V caster, float radius, unsigned long* seed, long* validCount)
{
    long valid[16]; long n = 0;
    for (long i = 0; i < count; ++i) {
        V d = { positions[i].X - caster.X, positions[i].Y - caster.Y, 0 };
        float r = radius + 0.3f;
        if (d.X * d.X + d.Y * d.Y >= r * r) valid[n++] = i;
    }
    *validCount = n;
    if (n == 0) return -1;
    return valid[GFRandom(n, seed)];
}
int main()
{
    V caster = { 0, 0, 0 };
    V pos[4] = { { 0.5f, 0, 9 }, { 2, 0, 0 }, { 0, 0.79f, 0 }, { 0, -3, 0 } };
    unsigned long seed = 1; long n;
    long a = Pick(pos, 4, caster, 0.5f, &seed, &n);   // radius 0.8: indices 1 and 3 (0.79 is inside, Z ignored)
    unsigned long s2 = 1; unsigned long expect = GFRandom(2, &s2);
    bool ok = n == 2 && a == (expect ? 3 : 1) && seed == s2;
    V close[2] = { { 0.1f, 0, 0 }, { 0, 0.2f, 0 } };
    long b = Pick(close, 2, caster, 0.5f, &seed, &n);
    ok = ok && b == -1 && n == 0;
    std::printf(ok ? "008bc1a0_TEST PASS\n" : "008bc1a0_TEST FAIL\n");
    return ok ? 0 : 1;
}