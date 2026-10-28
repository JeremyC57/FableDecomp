#ifndef _MSC_VER
#define __forceinline inline
#endif
#include <cstdio>
class CTCBase;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
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
    CThing* m_pItem;                                // +0x04
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
};

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCCreatureModeManager { public:                     // interface 0x31
    void RemoveMode(unsigned long mode);            // 0x0073ABA0
};

struct CTCHitState_8b {                             // interface 0x8B component (class not identified)
    unsigned char _pad00[0x10];
    long CurrentHitFrame;                           // +0x10
    unsigned char _pad14[4];
    bool Reset;                                     // +0x18
    unsigned char _pad19[3];
    long ResetHitFrame;                             // +0x1C
};

class CTCUnholyPower { public:
    virtual void v00();
    void ResetCreatureHitState(CIntelligentPointerCreature creature);
};

void CTCUnholyPower::ResetCreatureHitState(CIntelligentPointerCreature creature)
{
    if (IS_THING_ALIVE(creature)) {
        CTCCreatureModeManager* modes;
        if (creature.GetPItem()->GetInterface(&modes, 0x31))
            modes->RemoveMode(0x5f);
        CTCHitState_8b* hitState;
        if (creature.GetPItem()->GetInterface(&hitState, 0x8b)) {
            hitState->Reset = true;
            hitState->ResetHitFrame = hitState->CurrentHitFrame;
        }
    }
}

// ---- test stubs for the retail callees ----
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static int g_dtors, g_removed;
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerCreature::v00() {}
void CTCCreatureModeManager::RemoveMode(unsigned long mode) { g_removed = (int)mode; }
void CTCUnholyPower::v00() {}

int main()
{
    int ok = 1;
    CTCCreatureModeManager modes;
    CTCHitState_8b hit; hit.CurrentHitFrame = 77; hit.Reset = false; hit.ResetHitFrame = 0;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x31; entries[0].component = (CTCBase*)&modes;
    entries[1].type = 0x8b; entries[1].component = (CTCBase*)&hit;
    CThing c;
    for (unsigned i = 0; i < sizeof(c); ++i) ((unsigned char*)&c)[i] = 0;
    c.m_interfaces.first = entries; c.m_interfaces.last = entries + 2;
    c.m_presentBits[1] = 1u << 17; c.m_presentBits[4] = 1u << 11;
    CTCUnholyPower power;
    {
        CIntelligentPointerCreature p; p.m_pItem = &c;
        g_dtors = 0; g_removed = 0;
        power.ResetCreatureHitState(p);
        if (g_removed != 0x5f || !hit.Reset || hit.ResetHitFrame != 77 || g_dtors != 1) ok = 0;
    }
    {   // dead creature: nothing touched
        hit.Reset = false; hit.ResetHitFrame = 0; c.m_flags91 = 1;
        CIntelligentPointerCreature p; p.m_pItem = &c;
        g_removed = 0;
        power.ResetCreatureHitState(p);
        if (g_removed != 0 || hit.Reset || hit.ResetHitFrame != 0) ok = 0;
        c.m_flags91 = 0;
    }
    {   // no mode manager, hit state still re-armed
        c.m_presentBits[1] = 0; hit.CurrentHitFrame = 5;
        CIntelligentPointerCreature p; p.m_pItem = &c;
        g_removed = 0;
        power.ResetCreatureHitState(p);
        if (g_removed != 0 || !hit.Reset || hit.ResetHitFrame != 5) ok = 0;
    }
    {   // null pointer
        CIntelligentPointerCreature p; p.m_pItem = 0;
        power.ResetCreatureHitState(p);
    }
    if (!ok) { std::printf("00793e00_TEST FAIL\n"); return 1; }
    std::printf("00793e00_TEST PASS\n");
    return 0;
}