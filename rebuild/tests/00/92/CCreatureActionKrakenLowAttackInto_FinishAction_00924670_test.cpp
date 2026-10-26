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

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void FinishAction();                // 0x00859A20 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8 CIntelligentPointer<CThing> (Ego_r.pdb +0xb4)
    unsigned char           _padb0[0x114 - 0xb0];
    ~CCombatActionBase();                       // 0x0062E900
    int m_kind; long m_arg;           // test-only record
    __forceinline CThingCreatureBase& Creature() const { return *static_cast<CThingCreatureBase*>(PCreature.GetPItem()); }
    __forceinline bool IsTargetAlive() const    // two fetches: present, then not dead
    {
        return PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1);
    }
};
struct CCreatureAction_KrakenLowAttackLoop : CCombatActionBase {    // vtable 0x01290C64
    bool Interrupt;                             // +0x114 (PDB +0x12c)
    long LoopNum;                               // +0x118 (PDB +0x130)
    CCreatureAction_KrakenLowAttackLoop(CThingCreatureBase& creature, CThing& target, long loopNum, bool interrupt);   // 0x009255E0
};
struct CCreatureAction_KrakenLowAttackOutOf : CCombatActionBase {   // vtable 0x01290DBC
    CCreatureAction_KrakenLowAttackOutOf(CThingCreatureBase& creature);   // 0x00925680
};
struct CCreatureAction_KrakenLowAttackInto : CCombatActionBase {    // vtable 0x01290B0C
    virtual void FinishAction();
};

void CCreatureAction_KrakenLowAttackInto::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IsTargetAlive())
        Creature().SetCurrentAction(CCreatureAction_KrakenLowAttackLoop(Creature(), *PTarget.GetPItem(), 0, false));
    else
        Creature().SetCurrentAction(CCreatureAction_KrakenLowAttackOutOf(Creature()));
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
void CCombatActionBase::v00() { } void CCombatActionBase::FinishAction() { Log(4); }
CCreatureAction_KrakenLowAttackLoop::CCreatureAction_KrakenLowAttackLoop(CThingCreatureBase& creature, CThing& target, long loopNum, bool interrupt) { m_kind = 1; m_arg = (&target != 0 ? 10 : 0) + loopNum + (interrupt ? 100 : 0); }
CCreatureAction_KrakenLowAttackOutOf::CCreatureAction_KrakenLowAttackOutOf(CThingCreatureBase& creature) { m_kind = 2; m_arg = 0; }
int main()
{
    CThingCreatureBase creature; std::memset(&creature, 0, sizeof(creature));
    CThing target; std::memset(&target, 0, sizeof(target));
    CCreatureAction_KrakenLowAttackInto* a = new CCreatureAction_KrakenLowAttackInto;
    a->PCreature.m_pItem = &creature;
    a->PTarget.m_pItem = &target;
    a->FinishAction();
    int want1[] = { 4, 1, 10, 50 };
    if (!LogIs(want1, 4)) FAIL("00924670");
    LogReset(); target.m_flags91 = 1;
    a->FinishAction();
    int want2[] = { 4, 2, 0, 50 };
    if (!LogIs(want2, 4)) FAIL("00924670");
    LogReset(); a->PTarget.m_pItem = 0;
    a->FinishAction();
    if (!LogIs(want2, 4)) FAIL("00924670");
    PASS("00924670");
}