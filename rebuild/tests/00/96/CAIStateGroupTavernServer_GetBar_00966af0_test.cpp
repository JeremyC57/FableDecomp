#include <cstdio>
#include <list>
// Model of GetBar's logic: keep alive bars, then pick index GFRandom(size, seed) from the list.
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed *= 0x24a1; *seed += 0x24df; *seed = ror13(*seed);
    if (range) return *seed % range;
    return 0;
}
struct Thing { int id; bool dead; bool bar; };
static bool GetBar(Thing* things, int n, unsigned long* seed, Thing** pBar)
{
    std::list<Thing*> bars;
    for (int i = 0; i < n; ++i)
        if (!things[i].dead && things[i].bar)
            bars.push_back(&things[i]);
    if (bars.empty())
        return false;
    *pBar = 0;
    while (true) {
        if (!bars.empty()) {
            long index = GFRandom(bars.size(), seed);
            for (std::list<Thing*>::iterator it = bars.begin(); it != bars.end(); ++it) {
                if (index == 0) { *pBar = *it; break; }
                --index;
            }
            return true;
        }
        return false;
    }
}
int main()
{
    Thing t[5] = { {0,false,false}, {1,true,true}, {2,false,true}, {3,false,true}, {4,false,true} };
    unsigned long seed = 12345, s2 = 12345;
    Thing* bar = &t[0];
    if (!GetBar(t, 5, &seed, &bar)) { std::printf("00966af0_TEST FAIL\n"); return 1; }
    unsigned long idx = GFRandom(3, &s2);
    if (bar != &t[2 + idx] || seed != s2) { std::printf("00966af0_TEST FAIL\n"); return 1; }
    Thing none[2] = { {0,true,true}, {1,false,false} };
    Thing* keep = &t[0];
    if (GetBar(none, 2, &seed, &keep) || keep != &t[0]) { std::printf("00966af0_TEST FAIL\n"); return 1; }
    std::printf("00966af0_TEST PASS\n");
    return 0;
}