// CThingOwner_007504e0::SetInterface9Flag(bool value) -- writes a flag byte (+0x10) on the owned thing's interface-9 component when the thing has that interface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007504E0, length 63 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (landed callers).
// The manifest label "CAIStateGroupBase::SetPushableByHero" is propagated and rejected: here
// +0x04 of `this` is the thing itself (CAIStateGroupBase::PBrain would need another hop to the
// creature, as 0x0097E330 does). The body is the thing's inline GetInterface(type, &out) of the
// landed 0x007BEA08 family: the presence bitset at +0x20 is tested on the raw masked word
// (type 9 -> word 0, bit 9: `test ch,2`), the sorted interface map at +0x44 is searched with
// LowerBound (end or a larger key -> the end entry), the component goes to the out pointer (eax)
// and `true` folds the caller's `if`. The type key lives in the `push ecx` frame slot and the
// map pointer is a named local (esi). A direct `it->component->flag = value` allocates the
// component in ecx instead; the out-parameter form is what keeps it in eax.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCBase { public:
    unsigned char _pad[0x10];
    bool          m_flag;                   // +0x10
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    bool GetInterface(long type, CTCBase** out)   // inline: the landed GetInterface_0x.. family shape
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = it->second;
            return true;
        }
        return false;
    }
};

struct CThingOwner_007504e0 {
    void*   __vftable;                      // +0x00
    CThing* m_pThing;                       // +0x04
    void SetInterface9Flag(bool value);
};

void CThingOwner_007504e0::SetInterface9Flag(bool value)
{
    CTCBase* component;
    if (m_pThing->GetInterface(9, &component))
        component->m_flag = value;
}