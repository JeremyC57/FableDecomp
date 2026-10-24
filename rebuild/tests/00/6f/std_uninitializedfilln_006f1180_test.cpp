// Behaviour test for 0x006F1180 (std::__uninitialized_fill_n over 28-byte CDefendingCombatantInfo::CProng).
#include <cstdio>
#include <cstring>
#include <new>
static int g_setCalls = 0;
struct CBaseIntelligentPointer {
    void* PData;
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { }
    void* GetPItem() const { return PData; }
    void SetPItem(void* p) { ++g_setCalls; PData = p; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); }
};
#pragma pack(push, 1)
struct C3DVector { float X, Y, Z; };
#pragma pack(pop)
struct CProng { bool Blocked; C3DVector Prong; long ProngBlockageTimer; CIntelligentPointer POccupier; };
struct __false_type { };
static CProng* FillN(CProng* first, unsigned int n, const CProng& x, const __false_type&)
{
    CProng* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CProng(x);
    return cur;
}
#define FAIL { std::printf("006f1180_TEST FAIL\n"); return 1; }
int main()
{
    int occupier = 7;
    CProng x; x.Blocked = true; x.Prong.X = 0.5f; x.Prong.Y = -1.0f; x.Prong.Z = 2.0f; x.ProngBlockageTimer = 42; x.POccupier.PData = &occupier;
    static CProng slots[4];
    if (FillN(slots, 0, x, __false_type()) != slots || g_setCalls != 0) FAIL;
    CProng* end = FillN(slots, 3, x, __false_type());
    if (end != slots + 3 || g_setCalls != 3) FAIL;
    for (int i = 0; i < 3; ++i)
        if (!slots[i].Blocked || slots[i].Prong.X != 0.5f || slots[i].Prong.Y != -1.0f || slots[i].Prong.Z != 2.0f
            || slots[i].ProngBlockageTimer != 42 || slots[i].POccupier.GetPItem() != &occupier) FAIL;
    if (slots[3].Blocked || slots[3].POccupier.GetPItem() != 0) FAIL;
    std::printf("006f1180_TEST PASS\n");
    return 0;
}