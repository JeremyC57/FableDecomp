// Behaviour test for retail 0x00845bc0: the candidate source below is embedded verbatim (from src/00845bc0.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CCreatureAction_StopSneaking::PerformAction() -- the creature leaves creature mode 7 (sneaking) through its mode manager
//   (interface 0x31) and clears the active flag (+0x0f) of its interface-0x36 component
// Retail: Fable.exe (TLC, VC7.1) @ 0x00845BC0, 160 bytes, plain `ret`; slot 3 of vtable 0x01258F24 =
//   CCreatureAction_StopSneaking (rtti-vtables.tsv).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020,
//   CTCCreatureModeManager::RemoveMode(ulong) @ 0x0073ABA0 (manifest).
// Shape: two out-parameter GetInterface calls (void template, out pointers uninitialised so the absent path reads the
//   shared dead frame word); the RemoveMode call is tail-duplicated after the second lookup.
// Layout: CCreatureActionBase (retail 0xa8 bytes) with PCreature at +8; nothing else of the class is touched. The
//   interface-0x36 component is modelled only by the byte it clears (CTCSneaking is a descriptive name).
class CTCBase;

struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template <class T> __forceinline void GetInterface(T** ppOut, long interfaceType)   // leaves *ppOut alone when absent
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

struct CBaseIntelligentPointer {            // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                      // +0x00
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

class CTCCreatureModeManager { public:             // interface type 0x31 component
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];
};

struct CTCSneaking { unsigned char _pad[0xf]; unsigned char Active; };

class CCreatureAction_StopSneaking : public CCreatureActionBase { public:
    virtual void PerformAction();
};

void CCreatureAction_StopSneaking::PerformAction()
{
    CTCCreatureModeManager* mode;
    PCreature.GetPItem()->GetInterface(&mode, 0x31);
    CTCSneaking* sneak;
    PCreature.GetPItem()->GetInterface(&sneak, 0x36);
    mode->RemoveMode(7);
    sneak->Active = 0;
}
// ===== harness =====
#include <stdio.h>
#include <string.h>
static int g_removed = -1, g_removes; static void* g_remove_this; static int g_lookups;
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* k) { ++g_lookups; SEntry* e = first; while (e != last && e->type < *k) ++e; return e; }
void CTCCreatureModeManager::RemoveMode(unsigned long m) { g_remove_this = this; g_removed = (int)m; ++g_removes; }
void CCreatureActionBase::v00() {} void CCreatureActionBase::v01() {} void CCreatureActionBase::v02() {} void CCreatureActionBase::PerformAction() {}
int main() {
    static CTCSneaking sneak; static CTCCreatureModeManager* modes = (CTCCreatureModeManager*)0x5150;
    static CInterfaceMap::SEntry entries[3] = { {0x31, (CTCBase*)modes}, {0x36, (CTCBase*)&sneak}, {0x40, 0} };
    static CThing creature; memset(&creature, 0, sizeof creature);
    creature.m_presentBits[0x31 >> 5] |= 1UL << (0x31 & 31); creature.m_presentBits[0x36 >> 5] |= 1UL << (0x36 & 31);
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 3;
    sneak.Active = 1;
    CCreatureAction_StopSneaking a; a.PCreature.m_pItem = &creature;
    CCreatureActionBase* base = &a; base->PerformAction();          /* through the real vtable */
    int ok = g_removes == 1 && g_removed == 7 && g_remove_this == (void*)modes && sneak.Active == 0 && g_lookups == 2;
    printf(ok ? "00845bc0_TEST PASS\n" : "00845bc0_TEST FAIL\n"); return ok ? 0 : 1;
}