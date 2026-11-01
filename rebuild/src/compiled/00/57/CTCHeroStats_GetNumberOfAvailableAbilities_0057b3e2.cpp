// CTCHeroStats::GetNumberOfAvailableAbilities() const -- how many of the abilities 1..19 the hero's abilities inventory has at some level
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057B3E2, 93 bytes.
// Callees: std::bitset<>::test(size_t) @ 0x00410DE0 (the thing's component-presence bits),
//          CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound(const ETCInterfaceType&) @ 0x004365B0,
//          CTCInventoryAbilities::GetAbilityLevel(EHeroAbility) const @ 0x005EDE93 (-1 = not owned).
//
// Shape: a frame-pointer size TU (#pragma optimize("y", off) + optimize("s", on)). The parent thing's inline
// out-parameter GetInterface(&abilities, 0x6F) -- the landed 0x007BEA08/0x007BEA53 family body inlined: interface type
// held in edi (`push 6Fh ; pop edi`), presence test, the key stored in the `push ecx` frame word, the map advanced once
// (`add esi,44h`), end / larger key -> the end entry, the component into the out pointer (ebx) -- with the out
// pointer null-initialised (`xor ebx,ebx` before the test). Under the y-off/size pragma pair the template must be
// `__forceinline` (plain inline is emitted out of line and called). Then the counting loop `ability = 1 .. ability != 20`
// (`cmp esi,14h ; jne`) bumps the count for every level that is not -1.
// Layout: CTCBase::ParentThing +0x04; retail CThing: UsingComponents bitset +0x20, Components map {first, last} +0x44
// (PDB +0x48).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

class CTCInventoryAbilities { public:
    long GetAbilityLevel(long ability) const;  // 0x005EDE93
};

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
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                         // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;      // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> UsingComponents;             // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap Components;                  // +0x44 (PDB +0x48)

    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (UsingComponents.test(type)) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

struct CTCHeroStats_Retail {
    void*   __vftable;                         // +0x00
    CThing* ParentThing;                       // +0x04 CTCBase

    long GetNumberOfAvailableAbilities() const;
};

long CTCHeroStats_Retail::GetNumberOfAvailableAbilities() const
{
    CTCInventoryAbilities* abilities = 0;
    ParentThing->GetInterface(&abilities, 0x6F);
    long available = 0;
    for (long ability = 1; ability != 20; ++ability) {
        if (abilities->GetAbilityLevel(ability) != -1)
            ++available;
    }
    return available;
}