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
struct CBase { int baseCtor; CBase() : baseCtor(1) { } };
struct CPtrT : CBaseIntelligentPointer { CPtrT() : CBaseIntelligentPointer() { } };
struct CPtrC : CBaseIntelligentPointer { CPtrC() : CBaseIntelligentPointer() { } };
struct CGroup : CBase { CPtrT PStealableItem; CPtrC PEscapePoint; CPtrT PPreviouslyCarriedItem; long ActionPointIndex; bool GotActionPoint, ReachedDestination; long PickUpActionID; bool NeedsCleanup;
    CGroup() : CBase(), PStealableItem(), PEscapePoint(), PPreviouslyCarriedItem() { NeedsCleanup = false; } };
int main()
{
    CGroup g; bool ok = g.baseCtor == 1 && g.PStealableItem.GetPItem() == 0 && g.PEscapePoint.GetPItem() == 0 && g.PPreviouslyCarriedItem.GetPItem() == 0 && !g.NeedsCleanup;
    if (!ok) { std::printf("0090151a_TEST FAIL\n"); return 1; }
    std::printf("0090151a_TEST PASS\n"); return 0;
}