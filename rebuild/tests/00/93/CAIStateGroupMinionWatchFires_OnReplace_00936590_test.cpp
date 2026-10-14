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
struct CTCIdle { int chosen; void ChooseNewIdle() { ++chosen; } };
struct CGroup { CThing* creature; void OnReplace() { CTCIdle* p; if (GetInterface(creature, &p, 0xD3)) p->ChooseNewIdle(); } };
int main()
{
    CThing c; CTCIdle s; s.chosen = 0; CGroup g; g.creature = &c;
    g.OnReplace(); bool ok = s.chosen == 0;
    GiveTC(&c, 0xD3, &s); g.OnReplace(); ok = ok && s.chosen == 1;
    if (!ok) { std::printf("00936590_TEST FAIL\n"); return 1; }
    std::printf("00936590_TEST PASS\n"); return 0;
}