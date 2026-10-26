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
struct CPtrA : CBaseIntelligentPointer { CPtrA() : CBaseIntelligentPointer() { } };
struct CPtrB : CBaseIntelligentPointer { CPtrB() : CBaseIntelligentPointer() { } };
struct CPair { CPtrA PTargetCreature; long value; CPtrB PThing; bool f14, f15, f16; CPair(long unused) : PTargetCreature(), PThing() { f14 = false; f15 = false; f16 = false; } };
int main()
{
    CPair p(3); bool ok = p.PTargetCreature.GetPItem() == 0 && p.PThing.GetPItem() == 0 && !p.f14 && !p.f15 && !p.f16 && p.PTargetCreature.sets == 0;
    if (!ok) { std::printf("00793c00_TEST FAIL\n"); return 1; }
    std::printf("00793c00_TEST PASS\n"); return 0;
}