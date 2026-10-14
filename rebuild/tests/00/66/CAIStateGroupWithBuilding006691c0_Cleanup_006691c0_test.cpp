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
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(CThing* p) : CBaseIntelligentPointer() { SetPItem(p); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }
};
struct COwner { bool flag58, flag59; CIntelligentPointer PBuilding; COwner(CThing* t) : flag58(true), flag59(true), PBuilding(t) { }
    void Cleanup() { flag58 = false; flag59 = false; PBuilding = 0; } };
int main()
{
    CThing building; COwner o(&building);
    o.Cleanup();
    bool ok = o.PBuilding.GetPItem() == 0 && o.PBuilding.sets == 2;
    if (0) ok = ok && !o.flag58 && !o.flag59;
    if (!ok) { std::printf("006691c0_TEST FAIL\n"); return 1; }
    std::printf("006691c0_TEST PASS\n"); return 0;
}