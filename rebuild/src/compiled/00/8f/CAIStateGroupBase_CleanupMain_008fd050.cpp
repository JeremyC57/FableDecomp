// CAIStateGroupBase::CleanupMain() -- OnReplace, Cleanup, then tells the creature's interface-7 component the state group is gone
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FD050, length 77 bytes.
// Callees: slot 10 OnReplace (+0x28) and slot 9 Cleanup (+0x24) on `this` (rtti-vtables.tsv),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (landed callers 0x007BEA08 ..),
// and the component's method @ 0x006CB6B0 (thiscall, no arguments: a forwarder to its +0xc
// object's slot 18; the manifest's "CScriptThing::operator==" label is propagated and rejected).
// engine/CAIStateGroupBase.h: PBrain +0x04; engine/CAIBrain.h: Creature +0x18. The creature's
// component presence bitset lives at +0x20 (type 7 -> word 0, bit 7: retail tests the sign of
// the byte) and its sorted interface map at +0x44 -- the inline GetInterface shape of the landed
// 0x007BEA08 family (lower bound, end or larger key -> the end entry). The type key lives in the
// `push ecx` frame slot; the map pointer is a named local (esi) so the end is re-read through it.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCBase { public:
    void OnStateGroupRemoved();             // 0x006CB6B0
};

class CThingAICreature { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }   // the raw masked word: a bool result adds a shift
};

class CAIBrain { public:
    void*             __vftable;            // +0x00
    unsigned char     _pad[0x14];           // +0x04..+0x18
    CThingAICreature* Creature;             // +0x18
};

class CAIStateGroupBase { public:                  // vptr +0x00
    CAIBrain* PBrain;                       // +0x04
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08();
    virtual void Cleanup();                 // slot 9 (+0x24)
    virtual void OnReplace();               // slot 10 (+0x28)
    void CleanupMain();
};

void CAIStateGroupBase::CleanupMain()
{
    OnReplace();
    Cleanup();
    CThingAICreature* creature = PBrain->Creature;
    if (creature->HasInterface(7)) {
        ETCInterfaceType type = (ETCInterfaceType)(7);
        CInterfaceMap* map = &creature->m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || 7 < it->first)
            it = map->end();
        it->second->OnStateGroupRemoved();
    }
}