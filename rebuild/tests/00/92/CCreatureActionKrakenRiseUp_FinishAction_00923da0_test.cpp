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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CTCCreatureModeManager { public:                 // interface type 0x31 component
    bool AddMode(unsigned long mode);           // 0x0073AE10
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CTCInterface9 {                          // the owner's interface-9 component (descriptive, as landed 0x008CF610)
    unsigned char _pad[0x10];
    bool          m_flag10;                     // +0x10
};
class CThingAICreature : public CThing { public:
    unsigned char _padb8[0x1c8 - 0xb8];
    bool PerformingVillageTaskFlag : 1;         // +0x1c8 bit 0 (Ego_r.pdb +0x1f8)
    bool AISuspended : 1;
    bool HasInformation : 1;
    bool WanderWithInformation : 1;
    bool WaveWithInformation : 1;
    bool ContinueAIWithInformation : 1;
    bool ImmuneToAttack : 1;                    // bit 6
    bool EnableCreatureAutoPlacing : 1;         // bit 7
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void FinishAction();                // 0x00859A20 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    __forceinline CTCInterface9* GetInterface9() const
    {
        CTCInterface9* component;
        PCreature.GetPItem()->GetInterface(&component, 9);
        return component;
    }
    __forceinline CThingAICreature* GetAICreature() const { return static_cast<CThingAICreature*>(PCreature.GetPItem()); }
};
struct CCreatureAction_KrakenRiseUp : CCombatActionBase {   // vtable 0x01290594, sizeof 0x114
    virtual void FinishAction();
};

void CCreatureAction_KrakenRiseUp::FinishAction()
{
    GetInterface9()->m_flag10 = true;
    CThingAICreature* creature = GetAICreature();   // named local: the bit-field update addresses [eax+1c8h] directly
    creature->ImmuneToAttack = false;
    CCombatActionBase::FinishAction();
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

void CCombatActionBase::v00() { } void CCombatActionBase::FinishAction() { Log(4); }
int main()
{
    CTCInterface9 flags; std::memset(&flags, 0, sizeof(flags));
    CThingAICreature thing; std::memset(&thing, 0, sizeof(thing));
    thing.ImmuneToAttack = true; thing.EnableCreatureAutoPlacing = true;
    CInterfaceMap::SEntry e[1] = { { 9, (CTCBase*)&flags } };
    AddInterface(thing, e, 1);
    CCreatureAction_KrakenRiseUp* a = new CCreatureAction_KrakenRiseUp;
    a->PCreature.m_pItem = static_cast<CThing*>(&thing);
    a->FinishAction();
    int want[] = { 4 };
    if (!LogIs(want, 1) || !flags.m_flag10 || thing.ImmuneToAttack || !thing.EnableCreatureAutoPlacing) FAIL("00923da0");
    PASS("00923da0");
}