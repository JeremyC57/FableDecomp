// Behaviour test for 0x00959800 CAIStateGroup_HoverWanderAroundThing::IsRunnable: alive thing -> true untouched; otherwise prefer a
// random dead creature (inline GFRandom), else a random object (GFRandom), else false.
#include <cstdio>
#include <vector>
struct Thing { bool Dead; int id; };
static unsigned long g_seed;
static long GFROR13(unsigned long v) { return (long)((v >> 13) | (v << 19)); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    if (range != 0) return *seed % range;
    return 0;
}
struct Model {
    Thing* PThing;
    std::vector<Thing*> objects, dead;
    bool IsRunnable()
    {
        if (PThing != 0 && !PThing->Dead) return true;
        if (!dead.empty()) { PThing = dead[GFRandom((unsigned long)dead.size(), &g_seed)]; return true; }
        if (!objects.empty()) { PThing = objects[GFRandom((unsigned long)objects.size(), &g_seed)]; return true; }
        return false;
    }
};
int main()
{
    Thing a = {false, 1}, d = {true, 2}, o1 = {false, 3}, o2 = {false, 4}, c1 = {true, 5}, c2 = {true, 6};
    Model m; m.PThing = &a; m.objects.push_back(&o1);
    bool ok = m.IsRunnable() && m.PThing == &a;
    m.PThing = &d; g_seed = 1;
    ok = ok && m.IsRunnable() && m.PThing == &o1 && g_seed == (unsigned long)GFROR13(1 * 0x24a1 + 0x24df);
    m.PThing = &d; m.objects.push_back(&o2); m.dead.push_back(&c1); m.dead.push_back(&c2); g_seed = 7;
    unsigned long s = 7, idx = GFRandom(2, &s);
    ok = ok && m.IsRunnable() && m.PThing == (idx ? &c2 : &c1);
    Model e; e.PThing = 0; ok = ok && !e.IsRunnable() && e.PThing == 0;
    unsigned long z = 5; ok = ok && GFRandom(0, &z) == 0;
    if (!ok) { std::printf("00959800_TEST FAIL\n"); return 1; }
    std::printf("00959800_TEST PASS\n");
    return 0;
}