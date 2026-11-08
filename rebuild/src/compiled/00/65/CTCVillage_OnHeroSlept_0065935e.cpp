// CTCVillage::OnHeroSlept() -- after the hero sleeps: clear each live villager's 0xC3-interface timer, re-run the brain reset of every villager whose 0x1F interface is inactive, re-place those that need auto-placement, then reset the crier
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad24[0x20] removed.
// Naming wave 18: the interface-presence stand-in CBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065935E, length 349 bytes (`leave`, plain `ret`): frame-pointer size TU,
//   `#pragma optimize("y", off)` + `#pragma optimize("s", on)` as the landed siblings (`push 1fh ; pop ebx`).
// Callees: IS_THING_ALIVE(const CIntelligentPointer<CThingAICreature>&) 0x004AB8F0 (`__fastcall`), CBaseIntelligentPointer::
//   GetPItem 0x00A01B50, std::bitset<>::test 0x00410DE0, CVectorMap LowerBound 0x0040F020 (inlined GetTC), CTCBase::
//   IsActive 0x004C70F0, the creature's brain forwarders 0x006D5AB0 / 0x006D5AC0 (both `mov ecx,[ecx+1a8h] ; jmp`, to
//   0x0088BB60 / CBrain InitCreaturePosition 0x0088BBB0; manifest "_Cons_val<...>" is propagated),
//   CThingAICreature::RequiresAutoPlacement 0x00833710 (landed), CTCVillage::ResetCrier 0x00658E73 (landed).
// Layout: retail CTCVillage VillagePeople +0x2c (STLport list, bare head-node pointer; as the landed ~CTCVillage
//   0x0065B598); node {next, prev, CIntelligentPointer<CThingAICreature> +8}; the 0xC3 interface's float +0x10
//   (descriptive).
// Shape: three walks re-reading end() through the spilled `this`; the second and third test the person fetched up
//   front, then fetch it again for the call.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
class CBaseObject;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCBase {
public:
    bool IsActive() const;                      // 0x004C70F0
};
struct CTCSleepTimer {                          // the 0xC3 interface (descriptive)
    unsigned char _pad00[0x10];
    float         Timer;                        // +0x10
};
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274>       m_present;                    // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    template <class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return (T*)it->second;
    }
};
class CThingAICreature : public CThing {
public:
    void ResetBrain();                          // 0x006D5AB0 (forwards to the brain at +0x1a8)
    void InitCreaturePosition();                // 0x006D5AC0 (forwards to the brain at +0x1a8)
    bool RequiresAutoPlacement();               // 0x00833710
};

template <class T> class CIntelligentPointer { public:   // vptr + item (+ link words)
    void* _vfptr;
    T*    m_pItem;
    T* GetPItem() const;                        // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThingAICreature>& thing);   // 0x004AB8F0

struct SPersonNode {                            // STLport list node: next, prev, value
    SPersonNode* _M_next;
    SPersonNode* _M_prev;
    CIntelligentPointer<CThingAICreature> _M_data;
};
struct CCreaturePointerList {                   // list<CIntelligentPointer<CThingAICreature>>
    SPersonNode* _M_node;
};

class CTCVillage { public:
    unsigned char        _pad_0x00[0x2c];
    CCreaturePointerList VillagePeople;         // +0x2c
    void ResetCrier();                          // 0x00658E73
    void OnHeroSlept();
};

void CTCVillage::OnHeroSlept()
{
    SPersonNode* node;
    for (node = VillagePeople._M_node->_M_next; node != VillagePeople._M_node; node = node->_M_next) {
        if (IS_THING_ALIVE(node->_M_data)) {
            CThingAICreature* person = node->_M_data.GetPItem();
            if (person->HasTC(0xc3))
                person->GetTC<CTCSleepTimer>(0xc3)->Timer = 0.0f;
        }
    }
    for (node = VillagePeople._M_node->_M_next; node != VillagePeople._M_node; node = node->_M_next) {
        CThingAICreature* person = node->_M_data.GetPItem();
        if (person->HasTC(0x1f) && !person->GetTC<CTCBase>(0x1f)->IsActive())
            node->_M_data.GetPItem()->ResetBrain();
    }
    for (node = VillagePeople._M_node->_M_next; node != VillagePeople._M_node; node = node->_M_next) {
        CThingAICreature* person = node->_M_data.GetPItem();
        if (person->HasTC(0x1f) && !person->GetTC<CTCBase>(0x1f)->IsActive()
            && node->_M_data.GetPItem()->RequiresAutoPlacement())
            node->_M_data.GetPItem()->InitCreaturePosition();
    }
    ResetCrier();
}