// CThingFilter_IsAffectedByTraps::operator()(CThing* thing) -- false only when the thing's interface-0x31 component reports an object of type 0x5F; true otherwise (a frame-pointer, size-optimised twin of 0x0080E480)
// Leaf: ??RCThingFilter_IsAffectedByTraps
// Naming wave 18: the interface-presence stand-in CInterfaceBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007892EA, length 87 bytes.
// Callees: std::bitset<..>::test @ 0x00410DE0 (out of line in this TU), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0, the component's object accessor @ 0x0073A6E0 (propagated label "CPixelShader::CountReferences"),
// then that object's virtual slot 1 (a type id).
// Shape: the TU is size-optimised with a frame pointer (`push ebp ; mov ebp,esp` and `push dword` style) =
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`: the bitset test is not inlined, the key is homed in the
// dead `thing` slot ([ebp+8]), `add esi,44h` re-uses the thing register for the map, and the two exits share one tail.
#pragma optimize("y", off)
#pragma optimize("s", on)
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

namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}

class CThing { public:
    unsigned char _pad0[0x20];
    std::bitset<274> m_presentBits;                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits.test(type)) {
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

struct CTypedObject {
    virtual void v00();
    virtual long GetType();                         // slot 1
};

struct CTCInterface31 {                             // interface 0x31 component (class not established)
    CTypedObject* GetObject();                      // 0x0073A6E0
};

struct CThingFilter_IsAffectedByTraps {
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsAffectedByTraps::operator()(CThing* thing) const
{
    CTCInterface31* component;
    if (thing->GetInterface(0x31, &component) && component->GetObject()->GetType() == 0x5F)
        return false;
    return true;
}