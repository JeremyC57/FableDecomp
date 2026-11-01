// CTCHeroStats::SetFatness(float) -- clamps the fatness to [0,1], stores it and forwards it to the hero-morph component
// Leaf: SetFatness
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057B169, 114 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: GFLimitReturn<float>(const float&, const float&, const float&) @ 0x0057EF86 (__fastcall, returns the
//          reference to whichever bound or value wins), std::bitset::test @ 0x00410DE0 (thing interface presence,
//          +0x20), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (+0x44), CTCHeroMorph::SetFatness
//          @ 0x0071C060 (manifest label "SetAge" is propagated).
// The catalog names both this row and its twin 0x0057B0F7 "SetFatness"; this one is the real SetFatness. Retail CTCHeroStats keeps Age at
// +0x30, SunTan at +0x34 and Fatness at +0x38 (PDB +0x38/+0x3C/+0x40, 8 bytes lower: the map and the vector before
// them are 4 bytes smaller each in retail). OnInitialActivate passes [+0x38] to this row and [+0x34] to 0x0057B0F7 (SetSunTan).
// Shape: `GFLimitReturn(v, 0.0f, 1.0f)` with two float temporaries (1.0 stored first: arguments evaluate right to
// left); the parent thing's inline out-parameter GetInterface(&morph, 3) (interface type 3 = CTCHeroMorph) with the
// key in the dead lower-bound temporary slot; no null test of the component.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

template <class T> const T& __fastcall GFLimitReturn(const T& v, const T& lo, const T& hi);  // 0x0057EF86

namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    // the row lays the 9 words (36 bytes) out itself: the stand-in was an empty class
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || wanted < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCHeroMorph { public:
    void SetFatness(float fatness);                  // 0x0071C060
};

class CTCHeroStats { public:
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x28];
    float         Age;                               // +0x30 (PDB +0x38)
    float         SunTan;                            // +0x34 (PDB +0x3C)
    float         Fatness;                           // +0x38 (PDB +0x40)

    void SetFatness(float fatness);
};

void CTCHeroStats::SetFatness(float fatness)
{
    Fatness = GFLimitReturn(fatness, 0.0f, 1.0f);
    CTCHeroMorph* morph;
    if (ParentThing->GetInterface(&morph, 3))
        morph->SetFatness(Fatness);
}