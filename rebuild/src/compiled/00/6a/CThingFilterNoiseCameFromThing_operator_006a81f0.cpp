// CThingFilter_NoiseCameFromThing::operator()(CThing* thing) -- true when the thing's interface-0x37 component names the filter's source thing (component accessor == Thing)
// Leaf: ??RCThingFilter_NoiseCameFromThing
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A81F0, length 79 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, the component accessor @ 0x007498D0
// (manifest "_Cons_val<...>" is a propagated label; it returns the source thing).
// Layout: PDB CThingFilter_NoiseCameFromThing { CThing* Thing; } (+0); interface 0x37 -> bitset word 1 bit 23.
// Shape: `cmp eax,[edi]` -- the call result is the left operand; byte-constant exits.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

struct CTCInterface37 {                             // interface 0x37 component (class not established)
    CThing* GetSourceThing();                       // 0x007498D0
};

class CThingFilter_NoiseCameFromThing { public:
    CThing* Thing;                                  // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_NoiseCameFromThing::operator()(CThing* thing) const
{
    CTCInterface37* component;
    if (thing->GetInterface(0x37, &component) && component->GetSourceThing() == Thing)
        return true;
    return false;
}