// Behaviour test for 0x008EE720 CMaterialDef::GetRandomBerserkedSwordHitParticleEffect: LCG + ror13 step, index = r % size, 0 for an empty list.
#include <cstdio>
static unsigned int ROR13(unsigned int v) { return (v >> 13) | (v << 19); }
struct CRandomGenerator {
    unsigned int Seed;
    unsigned int Next() { Seed = Seed * 0x24a1 + 0x24df; Seed = ROR13(Seed); return Seed; }
    unsigned int Random(unsigned int n) { unsigned int r = Next(); return n ? r % n : 0; }
};
static CRandomGenerator g_rng;
struct Vec { int* b; int* e; };
struct CMaterialDef {
    Vec v;
    int Get()
    {
        if (v.b != v.e) { CRandomGenerator* rng = &g_rng; return v.b[rng->Random((unsigned int)(v.e - v.b))]; }
        return 0;
    }
};
int main()
{
    int data[3] = { 11, 22, 33 };
    CMaterialDef m; m.v.b = data; m.v.e = data + 3;
    g_rng.Seed = 1;
    unsigned int expect = ROR13(1 * 0x24a1 + 0x24df);
    int r = m.Get();
    bool ok = r == data[expect % 3] && g_rng.Seed == expect;
    m.v.e = data;
    ok = ok && m.Get() == 0 && g_rng.Seed == expect;
    if (!ok) { std::printf("008ee720_TEST FAIL\n"); return 1; }
    std::printf("008ee720_TEST PASS\n");
    return 0;
}