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
struct CTCPerceived { float value; };
struct CGroup { CThing* creature; bool IsRunnable() { CTCPerceived* p; if (GetInterface(creature, &p, 0x34)) { if (p->value != 0.0f) return true; } return false; } };
int main()
{
    CThing c; CTCPerceived p; p.value = 0.0f; CGroup g; g.creature = &c;
    bool ok = !g.IsRunnable();
    GiveTC(&c, 0x34, &p); ok = ok && !g.IsRunnable();
    p.value = -0.25f; ok = ok && g.IsRunnable();
    if (!ok) { std::printf("0091b340_TEST FAIL\n"); return 1; }
    std::printf("0091b340_TEST PASS\n"); return 0;
}