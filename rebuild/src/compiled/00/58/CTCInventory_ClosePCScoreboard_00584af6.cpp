// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCInventory::ClosePCScoreboard() -- closes the owner's PC scoreboard component (interface type 0x10D)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00584AF6, length 76 bytes; size-optimised TU with a frame pointer
// (`optimize("y",off)` + `("s",on)`, `leave`).
// Callees: std::bitset::test 0x00410DE0 on the thing's interface-presence bits (+0x20), CVectorMap::LowerBound
//          0x0040F020 on its sorted {type, component} map (+0x44), then the component's close routine 0x00562E70
//          (non-virtual; manifest label "~CParticleEffect" is propagated -- the body clears and releases the
//          member at +0x0c; its other caller is 0x00587900).
// Ego_r PDB: ?ClosePCScoreboard@CTCInventory@@QAEXXZ.
// The lookup is the thing's inlined GetInterface template (the landed out-of-line copies, e.g. 0x007BEA08, have
// the same bitset-then-lower-bound shape with a by-address key in the frame slot).
// Retail layout: CTCBase ParentThing at +0x04 (the PDB header layout of CTCInventory is wider), so `this` is
// modelled locally; the scoreboard class and its close routine are named by use.
// Shape: the result of GetInterface is ignored and the close is called unconditionally: when the interface is
// absent the out variable is never written, and VC7.1 gave it the same frame slot as the lookup key (`mov eax,
// [ebp-4]` on the absent path). 0x10D does not fit a byte immediate, so it lives in edi for the push, the key
// store and the compare. A size TU only inlines the lookup when it is `__forceinline`.
class CTCBase;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(long wanted, T** out)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || wanted < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

struct CTCPCScoreboard {
    void Close();                                    // 0x00562E70
};

class CTCInventory { public:
    virtual void v00();
    CThing* ParentThing;                             // +0x04
    void ClosePCScoreboard();
};

void CTCInventory::ClosePCScoreboard()
{
    CTCPCScoreboard* scoreboard;
    ParentThing->GetInterface(0x10d, &scoreboard);
    scoreboard->Close();
}