// CTCHeroStats::SetFatness(float) -- clamps the fatness to [0,1], stores it and forwards it to the hero-morph component
// Leaf: SetFatness
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

struct CTCBase;

template <class T> const T& __fastcall GFLimitReturn(const T& v, const T& lo, const T& hi);  // 0x0057EF86

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCHeroMorph {
    void SetFatness(float fatness);                  // 0x0071C060
};

struct CTCHeroStats {
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