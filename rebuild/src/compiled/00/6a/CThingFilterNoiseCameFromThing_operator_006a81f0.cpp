// CThingFilter_NoiseCameFromThing::operator()(CThing* thing) -- true when the thing's interface-0x37 component names the filter's source thing (component accessor == Thing)
// Leaf: ??RCThingFilter_NoiseCameFromThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A81F0, length 79 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, the component accessor @ 0x007498D0
// (manifest "_Cons_val<...>" is a propagated label; it returns the source thing).
// Layout: PDB CThingFilter_NoiseCameFromThing { CThing* Thing; } (+0); interface 0x37 -> bitset word 1 bit 23.
// Shape: `cmp eax,[edi]` -- the call result is the left operand; byte-constant exits.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;           // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
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