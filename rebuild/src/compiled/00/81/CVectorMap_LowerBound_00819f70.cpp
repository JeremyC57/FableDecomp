// CVectorMap::LowerBound(const unsigned long& key) const -- (CVectorMap<unsigned long, CSkeletalMorphs::CEntry >) binary search of the sorted 12-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@KVCEntry@CSkeletalMorphs@@V?$CKeyPairCompareLess@KVCEntry@CSkeletalMorphs@@@@@@QBEPBU?$pair@KVCEntry@CSkeletalMorphs@@@std@@ABK@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00819F70, 82 bytes (+ int3 padding to 96), thiscall `ret 4`; the STLport
//   iterator is a plain pointer returned in eax.
// Identity (proven): CVectorMap<unsigned long, CSkeletalMorphs::CEntry, CKeyPairCompareLess<...> >::LowerBound(const unsigned long&) const; evidence: its only caller is CSkeletalMorphs::GetAllMorphsOfType 0x0081AE50 (0x0081AE59; Ego_r.pdb ?GetAllMorphsOfType@CSkeletalMorphs@@QBE_NKAAV?$vector@VCEntry@CSkeletalMorphs@@...@Z, const) over Morphs, Ego_r.pdb CVectorMap<unsigned long, CSkeletalMorphs::CEntry, ...> at +0 (CEntry 8 bytes) -- the const overload, the only one FableWin.pdb names for this map.
// Label: the manifest's "LowerBound" is propagated (family J008: six byte-identical bodies, unsigned key (`jae`); retail has no identical-code
//   folding, so each address is one instantiation and one const-ness: both PDBs name a non-const LowerBound returning an
//   iterator and a const one returning a const_iterator, and the two compile to the same bytes).
// Body: count = (last - first) / 12; while (count > 0) { half = count / 2; if (first[half].first < key) { first += half + 1;
//   count -= half + 1; } else count = half; } return first; -- the key read once before the loop, `count -= half + 1` as
//   `count += -1 - half`, the signed `cdq/sub/sar` halving.
// Layout (retail CVectorMap 0x10): CArray<pair<K, V> > Data +0 (STLport vector: _M_start, _M_finish, _M_end_of_storage),
//   the empty comparator +0xC, Dirty +0xD.
// Model: the landed 0x00447C80 body (J006) with an unsigned key.
class CDefString { public: long TablePos; };                 // Ego_r.pdb, 4 bytes
class CSkeletalMorphs {
public:
    class CEntry {                                            // Ego_r.pdb, 8 bytes
    public:
        CDefString MorphName;                                 // +0x0
        short      Priority;                                  // +0x4
        bool       SupressesLowerMorphs;                      // +0x6
    };
};
#include "engine/CArray.h"


template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
};

template <class K, class V, class TCompare> class CVectorMap {   // sorted-on-demand vector map, retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::iterator iterator;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C (empty comparator)
    bool               Dirty;                                 // +0x0D
    const_iterator LowerBound(const K& key) const;
};

template <class K, class V, class TCompare>
typename CVectorMap<K, V, TCompare>::const_iterator CVectorMap<K, V, TCompare>::LowerBound(const K& key) const
{
    const_iterator first = Data.begin();
    int count = Data.end() - Data.begin();
    while (count > 0)
    {
        int half = count / 2;
        const_iterator middle = first + half;
        if (Compare(*middle, key))
        {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}

template class CVectorMap<unsigned long, CSkeletalMorphs::CEntry, CKeyPairCompareLess<unsigned long, CSkeletalMorphs::CEntry > >;