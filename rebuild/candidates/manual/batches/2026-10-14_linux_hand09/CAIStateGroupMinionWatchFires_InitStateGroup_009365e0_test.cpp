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
struct CTCIdle { void* group; long type; void RegisterIdleStateGroup(void* g, long t) { group = g; type = t; } };
struct CGroup { void InitStateGroup(void* brain, CThing& creature) { CTCIdle* p; if (GetInterface(&creature, &p, 0xD3)) p->RegisterIdleStateGroup(this, 6); } };
int main()
{
    CThing c; CTCIdle s; s.group = 0; s.type = 0; CGroup g;
    g.InitStateGroup(0, c); bool ok = s.group == 0;
    GiveTC(&c, 0xD3, &s); g.InitStateGroup(0, c); ok = ok && s.group == &g && s.type == 6;
    if (!ok) { std::printf("009365e0_TEST FAIL\n"); return 1; }
    std::printf("009365e0_TEST PASS\n"); return 0;
}
