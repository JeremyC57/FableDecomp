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
struct CTCKraken {                              // interface type 0xe2 component
    bool HasTargetBeenHitByKraken();            // 0x007FB500
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void FinishAction();                // slot 4 (+0x10)
    virtual void FrameUpdate();                 // 0x00859D80 (slot 56)
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    __forceinline CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.GetPItem()->GetInterface(&kraken, 0xe2);
        return kraken;
    }
};
struct CCreatureAction_KrakenHighAttackLoop : CCombatActionBase {   // vtable 0x0129107C
    bool Interrupt;                             // +0x114 (PDB +0x12c)
    long LoopNum;                               // +0x118 (PDB +0x130)
    virtual void FrameUpdate();
};

void CCreatureAction_KrakenHighAttackLoop::FrameUpdate()
{
    if (!Interrupt && GetKraken()->HasTargetBeenHitByKraken()) {
        if (!Interrupt)
            FinishAction();
    }
    CCombatActionBase::FrameUpdate();
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

bool g_hit = false;
bool CTCKraken::HasTargetBeenHitByKraken() { Log(3); return g_hit; }
void CCombatActionBase::v00() { } void CCombatActionBase::v01() { } void CCombatActionBase::v02() { } void CCombatActionBase::v03() { }
void CCombatActionBase::FinishAction() { Log(4); }
void CCombatActionBase::FrameUpdate() { Log(5); }
int main()
{
    CTCKraken kraken;
    CThing thing; std::memset(&thing, 0, sizeof(thing));
    CInterfaceMap::SEntry e[1] = { { 0xe2, (CTCBase*)&kraken } };
    AddInterface(thing, e, 1);
    CCreatureAction_KrakenHighAttackLoop* a = new CCreatureAction_KrakenHighAttackLoop;
    a->PCreature.m_pItem = &thing;
    a->Interrupt = false;
    a->FrameUpdate();
    int want1[] = { 3, 5 };
    if (!LogIs(want1, 2)) FAIL("009242d0");
    LogReset(); g_hit = true;
    a->FrameUpdate();
    int want2[] = { 3, 4, 5 };
    if (!LogIs(want2, 3)) FAIL("009242d0");
    LogReset(); a->Interrupt = true;
    a->FrameUpdate();
    int want3[] = { 5 };
    if (!LogIs(want3, 1)) FAIL("009242d0");
    PASS("009242d0");
}