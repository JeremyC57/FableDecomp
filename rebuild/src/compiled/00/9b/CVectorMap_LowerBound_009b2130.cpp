// CVectorMap::LowerBound(const unsigned long& key) const -- (CVectorMap<unsigned long, CDefClassBase::CSubDefInfo >) binary search of the sorted 12-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@KVCSubDefInfo@CDefClassBase@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@@QBEPBU?$pair@KVCSubDefInfo@CDefClassBase@@@std@@ABK@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B2130, 82 bytes (+ int3 padding to 96), thiscall `ret 4`; the STLport
//   iterator is a plain pointer returned in eax.
// Identity (proven): CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<...> >::LowerBound(const unsigned long&) const; evidence: CParentDefClassBase::SubDefInfoMap (as 0x009B20D0); called by GetPSubDefInfo(const CCharString&) const 0x009AD4A0 (0x009AD4DD; Ego_r.pdb ?GetPSubDefInfo@CParentDefClassBase@@UBEPBVCSubDefInfo@CDefClassBase@@ABVCCharString@@@Z) -- the const overload.
// Label: the manifest's "LowerBound" is propagated (family J008: six byte-identical bodies, unsigned key (`jae`); retail has no identical-code
//   folding, so each address is one instantiation and one const-ness: both PDBs name a non-const LowerBound returning an
//   iterator and a const one returning a const_iterator, and the two compile to the same bytes).
// Body: count = (last - first) / 12; while (count > 0) { half = count / 2; if (first[half].first < key) { first += half + 1;
//   count -= half + 1; } else count = half; } return first; -- the key read once before the loop, `count -= half + 1` as
//   `count += -1 - half`, the signed `cdq/sub/sar` halving.
// Layout (retail CVectorMap 0x10): CArray<pair<K, V> > Data +0 (STLport vector: _M_start, _M_finish, _M_end_of_storage),
//   the empty comparator +0xC, Dirty +0xD.
// Model: the landed 0x00447C80 body (J006) with an unsigned key.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
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

template class CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo > >;