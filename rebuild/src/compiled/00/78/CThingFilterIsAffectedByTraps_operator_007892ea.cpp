// CThingFilter_IsAffectedByTraps::operator()(CThing* thing) -- false only when the thing's interface-0x31 component reports an object of type 0x5F; true otherwise (a frame-pointer, size-optimised twin of 0x0080E480)
// Leaf: ??RCThingFilter_IsAffectedByTraps
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); map pointer made const (the row grades the const LowerBound).
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
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;


class CThing { public:
    unsigned char _pad0[0x20];
    std::bitset<274> m_presentBits;                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits.test(type)) {
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