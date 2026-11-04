// CTCHeroStats::SetSunTan(float) -- clamps the sun tan to [0,1], stores it and forwards it to the hero-morph component
// Leaf: SetSunTan
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057B0F7, 114 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: GFLimitReturn<float>(const float&, const float&, const float&) @ 0x0057EF86 (__fastcall, returns the
//          reference to whichever bound or value wins), std::bitset::test @ 0x00410DE0 (thing interface presence,
//          +0x20), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (+0x44), CTCHeroMorph::SetSunTan
//          @ 0x0071C0C0 (manifest label "SetAge" is propagated).
// Label correction: the catalog calls this row (and 0x0057B169) "SetFatness"; retail CTCHeroStats keeps Age at
// +0x30, SunTan at +0x34 and Fatness at +0x38 (PDB +0x38/+0x3C/+0x40, 8 bytes lower: the map and the vector before
// them are 4 bytes smaller each in retail). OnInitialActivate passes [+0x34] to this row and [+0x38] to 0x0057B169.
// Shape: `GFLimitReturn(v, 0.0f, 1.0f)` with two float temporaries (1.0 stored first: arguments evaluate right to
// left); the parent thing's inline out-parameter GetInterface(&morph, 3) (interface type 3 = CTCHeroMorph) with the
// key in the dead lower-bound temporary slot; no null test of the component.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

template <class T> const T& __fastcall GFLimitReturn(const T& v, const T& lo, const T& hi);  // 0x0057EF86

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
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

class CTCHeroMorph { public:
    void SetSunTan(float tan);                       // 0x0071C0C0
};

class CTCHeroStats { public:
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x28];
    float         Age;                               // +0x30 (PDB +0x38)
    float         SunTan;                            // +0x34 (PDB +0x3C)
    float         Fatness;                           // +0x38 (PDB +0x40)

    void SetSunTan(float tan);
};

void CTCHeroStats::SetSunTan(float tan)
{
    SunTan = GFLimitReturn(tan, 0.0f, 1.0f);
    CTCHeroMorph* morph;
    if (ParentThing->GetInterface(&morph, 3))
        morph->SetSunTan(SunTan);
}