#include <cstdio>
struct CThing { unsigned char flags91; unsigned long bits[9]; long compType; void* comp; CThing() : flags91(0), compType(-1), comp(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; } };
struct CBaseIntelligentPointer {
    CThing* item; int sets;
    CBaseIntelligentPointer() : item(0), sets(0) { }
    CThing* GetPItem() const { return item; }
    void SetPItem(CThing* p) { item = p; ++sets; }
};
// the inlined GetInterface: presence bit, then the single-entry map
template<class T> bool GetInterface(CThing* t, T** out, long type)
{
    if (t->bits[type >> 5] & (1UL << (type & 31))) { *out = (t->compType == type) ? (T*)t->comp : 0; return true; }
    return false;
}
static void GiveTC(CThing* t, long type, void* comp) { t->bits[type >> 5] |= 1UL << (type & 31); t->compType = type; t->comp = comp; }
struct CTCGreet { bool greeted; };
struct CGroup { CThing* creature; bool ReactingToPlayer;
    bool Greeted() { if (ReactingToPlayer) { CTCGreet* p; GetInterface(creature, &p, 0x4F); return p->greeted; } return false; } };
int main()
{
    CThing c; CTCGreet gr; gr.greeted = true; GiveTC(&c, 0x4F, &gr); CGroup g; g.creature = &c; g.ReactingToPlayer = false;
    bool ok = !g.Greeted();
    g.ReactingToPlayer = true; ok = ok && g.Greeted();
    gr.greeted = false; ok = ok && !g.Greeted();
    if (!ok) { std::printf("0095f7d0_TEST FAIL\n"); return 1; }
    std::printf("0095f7d0_TEST PASS\n"); return 0;
}