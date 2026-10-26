#include <cstdio>
class CThing { public: unsigned char flags91; unsigned long bits[9]; long compType; void* comp; CThing() : flags91(0), compType(-1), comp(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; } };
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
struct CTCSchool { bool alive; };
static long IsAlive(const CTCSchool* s) { return s->alive ? 1 : 0; }
struct CGroup { CThing* creature; bool IsStateGroupStillValid() { CTCSchool* p; return GetInterface(creature, &p, 0x23) && IsAlive(p); } };
int main()
{
    CThing c; CTCSchool s; s.alive = true; CGroup g; g.creature = &c;
    bool ok = !g.IsStateGroupStillValid();
    GiveTC(&c, 0x23, &s); ok = ok && g.IsStateGroupStillValid();
    s.alive = false; ok = ok && !g.IsStateGroupStillValid();
    if (!ok) { std::printf("0090fcd0_TEST FAIL\n"); return 1; }
    std::printf("0090fcd0_TEST PASS\n"); return 0;
}