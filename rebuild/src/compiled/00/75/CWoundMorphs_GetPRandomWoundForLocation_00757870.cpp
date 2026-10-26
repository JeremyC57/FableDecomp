// CWoundMorphs::GetPRandomWoundForLocation(EBodyLocation location, unsigned long& seed, long& index) const -- sort the morph map if dirty, count the entries for the location, pick one with the world LCG and return its CEntry (index = its position in the map)
// Leaf: GetPRandomWoundForLocation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00757870, 180 bytes (+12 int3), `ret 0Ch`. Ego_r.pdb
//   `?GetPRandomWoundForLocation@CWoundMorphs@@QBEPBVCEntry@1@W4EBodyLocation@@AAKAAJ@Z`; the landed caller
//   CTCWound::AddWound (0x00757B30) cites this address under that name.
// Callees: sort<pair<EBodyLocation,CEntry>*, CKeyPairCompareLess> @ 0x007597C0 (__fastcall, landed),
//   CVectorMap<...>::UpperBound(const EBodyLocation&) @ 0x00758570 and ::LowerBound @ 0x00758470 (landed, __fastcall,
//   the STLport iterator returned in eax), GFROR13(unsigned long) @ 0x00497890 (__fastcall rotate right by 13).
// Label: the manifest's "CVectorMap_FindRandomEntry" is descriptive; the PDB name is used.
// Layout (retail, Ego_r.pdb = retail here): CWoundMorphs::Morphs = CVectorMap {STLport vector first +0, last +4,
//   end_of_storage +8; Compare +0xC (empty functor, 1 byte); Dirty +0xD}; entries are pair<EBodyLocation, CEntry> (0x2C).
// Shape: the inline mutable re-sort passes the 1-byte comparator by value (`xor eax,eax ; mov al,[esi+0Ch] ; push eax`)
//   and clears Dirty after the call; `UpperBound - LowerBound` is evaluated left to right (upper bound kept in edi);
//   the 0x24A1/0x24DF LCG + GFROR13 is the inline GFRandom with its `range ? .. : 0` folded by the zero-count test;
//   the result is a local initialised to 0 BEFORE the dirty test (retail's `xor ebp,ebp` at the top and the separate
//   `mov eax,ebp` exit; an early `return 0` gives `xor eax,eax`); the picked entry is LowerBound + r and the result
//   (`lea ecx,[eax+4]`, the entry's second member) is formed BEFORE the index (the distance from the first entry).
enum EBodyLocation { BODY_LOCATION_NONE = 0 };

long __fastcall GFROR13(unsigned long);       // 0x00497890 (rotate right by 13)

__forceinline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
}

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

class CWoundMorphs;

template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                       // +0x00
    value_type* Last;                                        // +0x04
    value_type* EndOfStorage;                                // +0x08
    C           Compare;                                     // +0x0C
    bool        Dirty;                                       // +0x0D
    value_type* LowerBound(const K& key) const;              // 0x00758470
    value_type* UpperBound(const K& key) const;              // 0x00758570
};

class CWoundMorphs { public:
    class CEntry { public:                                          // 40 bytes
        EBodyLocation Location;
        long          ReplacingTextureIndex;
        long          ReplacingBumpMapIndex;
        long          WoundTextureIndex;
        long          WoundBumpMapIndex;
        long          ScarTextureIndex;
        long          ScarBumpMapIndex;
        long          FadeToScarDuration;
        long          TotalDuration;
        float         PermanentScarStrength;
    };
    typedef CKeyPairCompareLess<EBodyLocation, CEntry> TCompare;
    typedef CVectorMap<EBodyLocation, CEntry, TCompare> TMap;
    mutable TMap Morphs;                                     // +0x00
    const CEntry* GetPRandomWoundForLocation(EBodyLocation location, unsigned long& seed, long& index) const;
};

typedef std::pair<EBodyLocation, CWoundMorphs::CEntry> TWoundPair;
void __fastcall sort(TWoundPair* first, TWoundPair* last, CWoundMorphs::TCompare comp);   // 0x007597C0

const CWoundMorphs::CEntry* CWoundMorphs::GetPRandomWoundForLocation(EBodyLocation location, unsigned long& seed,
                                                                     long& index) const
{
    const CEntry* result = 0;
    if (Morphs.Dirty) {
        sort(Morphs.First, Morphs.Last, Morphs.Compare);
        Morphs.Dirty = false;
    }
    long count = Morphs.UpperBound(location) - Morphs.LowerBound(location);
    if (count != 0) {
        unsigned long r = GFRandom(count, &seed);
        const TWoundPair* entry = Morphs.LowerBound(location) + r;
        result = &entry->second;
        index = entry - Morphs.First;
    }
    return result;
}