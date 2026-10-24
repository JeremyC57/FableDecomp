#include <cstdio>
#include <new>
// Test copy of the 0x006EFF60 model: CProng's out-of-line copy constructor over the byte-packed head.
static int g_links = 0;
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; ++g_links; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
#pragma pack(push, 1)
struct CProngHead {
    bool      Blocked;
    C3DVector Prong;
    CProngHead() : Blocked(false) { }
};
#pragma pack(pop)
struct CDefendingCombatantInfo {
    struct CProng : CProngHead {
        long                ProngBlockageTimer;
        CIntelligentPointer POccupier;
        CProng() : ProngBlockageTimer(0) { }
        CProng(const CProng& other);
    };
};
CDefendingCombatantInfo::CProng::CProng(const CProng& other)
    : CProngHead(other), ProngBlockageTimer(other.ProngBlockageTimer), POccupier(other.POccupier)
{
}
int main()
{
    typedef CDefendingCombatantInfo::CProng CProng;
    CProng a;
    a.Blocked = true;
    a.Prong.X = 1.5f; a.Prong.Y = -2.0f; a.Prong.Z = 3.0f;
    a.ProngBlockageTimer = 40;
    a.POccupier.m_pItem = reinterpret_cast<CBaseObject*>(0x80);
    CProng b(a);
    bool ok = b.Blocked && b.Prong.X == 1.5f && b.Prong.Y == -2.0f && b.Prong.Z == 3.0f && b.ProngBlockageTimer == 40
           && b.POccupier.GetPItem() == reinterpret_cast<CBaseObject*>(0x80) && g_links == 1;
    if (!ok) { std::printf("006eff60_TEST FAIL\n"); return 1; }
    std::printf("006eff60_TEST PASS\n");
    return 0;
}