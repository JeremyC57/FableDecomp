#include <cstdio>
// Behaviour model of CTCCreatureGeneratorCreator::IsGeneratorAlive (0x007C98A0).
struct Gen { bool Destroyed; };
struct Entry { long type; Gen* component; };
struct Thing {
    unsigned int bits[9];
    Entry* first; Entry* last;
    bool dead;
    Entry* LowerBound(long t) { Entry* e = first; while (e != last && e->type < t) ++e; return e; }
    bool GetInterface(long t, Gen** out)
    {
        if (bits[t >> 5] & (1u << (t & 31))) {
            Entry* it = LowerBound(t);
            if (it == last || t < it->type) it = last;
            *out = it->component;
            return true;
        }
        return false;
    }
};
struct Creator {
    Thing* gen;
    bool IsGeneratorAlive()
    {
        if (gen && !gen->dead) {
            Gen* g = 0;
            gen->GetInterface(0x3d, &g);
            if (!g->Destroyed) return true;
        }
        return false;
    }
};
int main()
{
    Gen g; g.Destroyed = false;
    Entry e[2]; e[0].type = 0x3d; e[0].component = &g; e[1].type = 0; e[1].component = 0;
    Thing t; for (int i = 0; i < 9; ++i) t.bits[i] = 0; t.bits[1] = 1u << 29; t.first = e; t.last = e + 1; t.dead = false;
    Creator c; c.gen = 0;
    if (c.IsGeneratorAlive()) { std::printf("007c98a0_TEST FAIL\n"); return 1; }
    c.gen = &t;
    if (!c.IsGeneratorAlive()) { std::printf("007c98a0_TEST FAIL\n"); return 1; }
    g.Destroyed = true;
    if (c.IsGeneratorAlive()) { std::printf("007c98a0_TEST FAIL\n"); return 1; }
    g.Destroyed = false; t.dead = true;
    if (c.IsGeneratorAlive()) { std::printf("007c98a0_TEST FAIL\n"); return 1; }
    std::printf("007c98a0_TEST PASS\n");
    return 0;
}
