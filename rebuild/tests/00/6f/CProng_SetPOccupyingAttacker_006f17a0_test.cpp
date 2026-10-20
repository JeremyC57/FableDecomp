// Behaviour test for 006f17a0: the model below is a copy of src/006f17a0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
static int g_ctors = 0, g_dtors = 0, g_sets = 0, g_gets = 0;
struct CBaseObject {};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctors; }
    ~CBaseIntelligentPointer() { ++g_dtors; }
    CBaseObject* GetPItem() { ++g_gets; return m_pItem; }
    void SetPItem(CBaseObject* item) { ++g_sets; m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
struct CThingCreatureBase : CBaseObject {
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;
    unsigned char m_91_1 : 7;
};
long g_framesPerSecond = 30;
struct CProng {
    bool                    Blocked;
    unsigned char           Prong[0xc];
    unsigned char           _pad0d[3];
    long                    ProngBlockageTimer;
    CBaseIntelligentPointer POccupier;
    __forceinline void SetPOccupierItem(CBaseObject* item)
    {
        CIntelligentPointer pointer;
        pointer.SetPItem(item);
        CBaseObject* occupier = pointer.GetPItem();
        POccupier.SetPItem(occupier);
    }
    CThingCreatureBase* GetPOccupier() { return (CThingCreatureBase*)POccupier.GetPItem(); }
    void SetBlocked(bool blocked) { Blocked = blocked; }
    void SetPOccupyingAttacker(CThingCreatureBase* attacker);
};

void CProng::SetPOccupyingAttacker(CThingCreatureBase* attacker)
{
    SetPOccupierItem(0);
    SetPOccupierItem((CBaseObject*)attacker);
}

int main()
{
    bool ok = true;
    CThingCreatureBase attacker; std::memset(&attacker, 0, sizeof(attacker));
    CProng prong; std::memset((void*)&prong.Blocked, 0, sizeof(bool)); prong.ProngBlockageTimer = 0;
    prong.POccupier.m_pItem = (CBaseObject*)0x1234;
    g_ctors = g_dtors = g_sets = g_gets = 0;
    prong.SetPOccupyingAttacker(&attacker);
    ok = ok && prong.POccupier.m_pItem == &attacker;        // the attacker occupies the prong
    ok = ok && g_ctors == 2 && g_dtors == 2;                // two temporaries (null, attacker), both destroyed
    ok = ok && g_sets == 4 && g_gets == 2;                  // each temporary set and read back once, the member set twice
    prong.SetPOccupyingAttacker(0);
    ok = ok && prong.POccupier.m_pItem == 0;                // released
    if (!ok) { std::printf("006f17a0_TEST FAIL\n"); return 1; }
    std::printf("006f17a0_TEST PASS\n");
    return 0;
}