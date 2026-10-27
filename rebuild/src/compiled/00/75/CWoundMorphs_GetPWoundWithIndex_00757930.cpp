// CWoundMorphs::GetPWoundWithIndex(long) const -- sort the morph map if dirty, then return the CEntry at that position (0 if out of range)
// Leaf: ?GetPWoundWithIndex@CWoundMorphs@@QBEPBVCEntry@1@J@Z
// Retail: Fable.exe @ 0x00757930, 86 bytes, thiscall `ret 4`.
// Identity (proven): Ego_r.pdb `?GetPWoundWithIndex@CWoundMorphs@@QBEPBVCEntry@1@J@Z`; the oracle row already carries
// the name, and the body sits between the landed CWoundMorphs::GetPRandomWoundForLocation 0x00757870 and
// CWoundMorphs::TransferBinaryIn 0x00757990; it indexes the same pair<EBodyLocation, CEntry> map (0x2C entries).
// Callees: sort<pair<EBodyLocation,CEntry>*, CKeyPairCompareLess> 0x007597C0 (__fastcall, landed CWoundMorphs_SortMap).
// Layout (Ego_r = retail): Morphs = CVectorMap {first +0, last +4, end_of_storage +8, Compare +0xC, Dirty +0xD}.
// Shape: the mutable re-sort passes the 1-byte comparator by value and clears Dirty after the call; the range test is
// `index >= 0` (signed) then `index < size()` (size_t, so `jae`), size() being the signed /0x2C pointer distance.
enum EBodyLocation { BODY_LOCATION_NONE = 0 };

namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
}

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                       // +0x00
    value_type* Last;                                        // +0x04
    value_type* EndOfStorage;                                // +0x08
    C           Compare;                                     // +0x0C
    bool        Dirty;                                       // +0x0D
    unsigned int size() const { return Last - First; }
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
    const CEntry* GetPWoundWithIndex(long index) const;
};

typedef std::pair<EBodyLocation, CWoundMorphs::CEntry> TWoundPair;
void __fastcall sort(TWoundPair* first, TWoundPair* last, CWoundMorphs::TCompare comp);   // 0x007597C0

const CWoundMorphs::CEntry* CWoundMorphs::GetPWoundWithIndex(long index) const
{
    if (Morphs.Dirty) {
        sort(Morphs.First, Morphs.Last, Morphs.Compare);
        Morphs.Dirty = false;
    }
    if (index >= 0 && index < Morphs.size())
        return &Morphs.First[index].second;
    return 0;
}