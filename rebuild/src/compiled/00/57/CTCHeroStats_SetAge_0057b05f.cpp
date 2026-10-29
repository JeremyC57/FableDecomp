// CTCHeroStats::SetAge(float) -- clamps the age to the def's [min,max], stores it and passes the normalised age to the hero morph
// Leaf: SetAge
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

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

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
    CPresenceBits m_present;                         // +0x20
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
    if (ParentThing->GetInterface(&morph, 3))
        morph->SetAge((Age - minAge) / (maxAge - minAge));
}