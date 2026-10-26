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
class CTCBase;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
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
class CThingCreatureBase;
class CCombatActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void*   m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CCreatureActionBase { public:                    // retail sizeof 0xa8 (Ego_r.pdb 0xb4)
    virtual void v00();
    virtual void FinishAction();                // 0x00694430 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0xa8 - 0x10];
    __forceinline CThingCreatureBase& Creature() const { return *static_cast<CThingCreatureBase*>(PCreature.GetPItem()); }
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    CBaseIntelligentPointer PTarget;            // +0xa8 CIntelligentPointer<CThing>
    unsigned char           _padb0[0x114 - 0xb0];
    ~CCombatActionBase();                       // 0x0062E900
    int m_kind; long m_arg;           // test-only record
};
struct CCreatureAction_KrakenRoarLoop : CCombatActionBase {   // vtable 0x01291494
    long LoopNum;                               // +0x114 (PDB +0x12c)
    CCreatureAction_KrakenRoarLoop(CThingCreatureBase& creature, long loopNum);   // 0x00925920
};
struct CCreatureAction_KrakenRoarInto : CCombatActionBase {   // vtable 0x0129133C
    virtual void FinishAction();
};

void CCreatureAction_KrakenRoarInto::FinishAction()
{
    CCreatureActionBase::FinishAction();
    Creature().SetCurrentAction(CCreatureAction_KrakenRoarLoop(Creature(), 0));
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

bool CThingCreatureBase::SetCurrentAction(const CCombatActionBase& action) { Log(action.m_kind); Log(action.m_arg); return true; }
CCombatActionBase::~CCombatActionBase() { Log(50); }
void CCreatureActionBase::v00() { } void CCreatureActionBase::FinishAction() { Log(3); }
CCreatureAction_KrakenRoarLoop::CCreatureAction_KrakenRoarLoop(CThingCreatureBase& creature, long loopNum) { m_kind = 1; m_arg = loopNum; }
int main()
{
    CThingCreatureBase creature; std::memset(&creature, 0, sizeof(creature));
    CCreatureAction_KrakenRoarInto* a = new CCreatureAction_KrakenRoarInto;
    a->PCreature.m_pItem = &creature;
    a->FinishAction();
    int want[] = { 3, 1, 0, 50 };
    if (!LogIs(want, 4)) FAIL("00924b40");
    PASS("00924b40");
}