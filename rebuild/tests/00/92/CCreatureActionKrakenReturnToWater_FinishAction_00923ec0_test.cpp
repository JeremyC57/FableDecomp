#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif
static int g_log[64];
static int g_logn = 0;
static void Log(int v) { if (g_logn < 64) g_log[g_logn++] = v; }
static bool LogIs(const int* v, int n) { if (n != g_logn) return false; for (int i = 0; i < n; ++i) if (g_log[i] != v[i]) return false; return true; }
static void LogReset() { g_logn = 0; }
struct CTCBase;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0xb4 - 0x92];
    float         m_health;                     // +0xb4
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTCBoss {                                // interface type 0xad component
    void IncrementBossPhase();                  // 0x007AED20
};
struct CTCKraken {                              // interface type 0xe2 component
    unsigned char _pad00[0x5c];
    bool          ReturnHeadToWater;            // +0x5c (PDB +0x6c)
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    virtual void FinishAction();                // 0x00859A20 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    __forceinline CTCBoss* GetBoss() const
    {
        CTCBoss* boss;
        PCreature.GetPItem()->GetInterface(&boss, 0xad);
        return boss;
    }
    __forceinline CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.GetPItem()->GetInterface(&kraken, 0xe2);
        return kraken;
    }
};
struct CCreatureAction_KrakenReturnToWater : CCombatActionBase {   // vtable 0x012906F4, sizeof 0x114
    virtual void FinishAction();
};

void CCreatureAction_KrakenReturnToWater::FinishAction()
{
    CCombatActionBase::FinishAction();
    GetBoss()->IncrementBossPhase();
    GetKraken()->ReturnHeadToWater = false;
}


CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pItem; }
void CBaseIntelligentPointer::v00() { }
static void AddInterface(CThing& thing, CInterfaceMap::SEntry* entries, int n)
{
    for (int i = 0; i < n; ++i)
        thing.m_presentBits[entries[i].type >> 5] |= 1UL << (entries[i].type & 31);
    thing.m_interfaces.first = entries;
    thing.m_interfaces.last = entries + n;
}
#define FAIL(a) do { std::printf(a "_TEST FAIL\n"); return 1; } while (0)
#define PASS(a) do { std::printf(a "_TEST PASS\n"); return 0; } while (0)

void CTCBoss::IncrementBossPhase() { Log(8); }
void CCombatActionBase::v00() { } void CCombatActionBase::FinishAction() { Log(4); }
int main()
{
    CTCBoss boss; CTCKraken kraken; std::memset(&kraken, 0, sizeof(kraken)); kraken.ReturnHeadToWater = true;
    CThing thing; std::memset(&thing, 0, sizeof(thing));
    CInterfaceMap::SEntry e[2] = { { 0xad, (CTCBase*)&boss }, { 0xe2, (CTCBase*)&kraken } };
    AddInterface(thing, e, 2);
    CCreatureAction_KrakenReturnToWater* a = new CCreatureAction_KrakenReturnToWater;
    a->PCreature.m_pItem = &thing;
    a->FinishAction();
    int want[] = { 4, 8 };
    if (!LogIs(want, 2) || kraken.ReturnHeadToWater) FAIL("00923ec0");
    PASS("00923ec0");
}