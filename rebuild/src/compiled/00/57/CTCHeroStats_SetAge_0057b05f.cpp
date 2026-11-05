// CTCHeroStats::SetAge(float) -- clamps the age to the def's [min,max], stores it and passes the normalised age to the hero morph
// Leaf: SetAge
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057B05F, 152 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: CTCBase::GetDefinitionManager() @ 0x006869D0 (twice; +0xDC the CHeroStatsDef pointer, def +0x68 / +0x6C the
//          minimum / maximum age), GFLimitReturn<float> @ 0x0057EF86 (__fastcall), std::bitset::test @ 0x00410DE0,
//          CVectorMap::LowerBound @ 0x0040F020, CTCHeroMorph::SetAge(float) @ 0x0071BFA0.
// Shape: the two bounds are named locals (min at [ebp-4] declared first, max at [ebp-8]) because they are used again
// in `(Age - min) / (max - min)` after the clamp; the GetInterface key gets its own slot ([ebp-0xC]).
// The def is read through an inline GetHeroStatsDef() accessor: reading GetDefinitionManager()->PHeroStatsDef in the body
// swaps the esi/edi roles of this and the parent thing.
// Layout: retail Age +0x30 (PDB +0x38).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

template <class T> const T& __fastcall GFLimitReturn(const T& v, const T& lo, const T& hi);  // 0x0057EF86

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCHeroMorph { public:
    void SetAge(float normalisedAge);                // 0x0071BFA0
};

class CHeroStatsDef { public:
    unsigned char _pad_0x00[0x68];
    float         MinAge;                            // +0x68
    float         MaxAge;                            // +0x6C
};

class CGameDefinitionManager { public:
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC CDefPointer<const CHeroStatsDef>::Object
};

class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

class CTCHeroStats : public CTCBase { public:
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x28];
    float         Age;                               // +0x30 (PDB +0x38)
    float         SunTan;                            // +0x34 (PDB +0x3C)
    float         Fatness;                           // +0x38 (PDB +0x40)

    const CHeroStatsDef* GetHeroStatsDef() const { return GetDefinitionManager().PHeroStatsDef; }
    void SetAge(float age);
};

void CTCHeroStats::SetAge(float age)
{
    float minAge = GetHeroStatsDef()->MinAge;
    float maxAge = GetHeroStatsDef()->MaxAge;
    Age = GFLimitReturn(age, minAge, maxAge);
    CTCHeroMorph* morph;
    if (ParentThing->GetTC(TCI_APPEARANCE_MORPH, &morph))
        morph->SetAge((Age - minAge) / (maxAge - minAge));
}