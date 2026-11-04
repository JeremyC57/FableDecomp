// CVectorMap::LowerBound(const unsigned long& key) const -- (CVectorMap<unsigned long, CAnimationEntry>) binary search of the sorted 20-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@KVCAnimationEntry@@V?$CKeyPairCompareLess@KVCAnimationEntry@@@@@@QBEPBU?$pair@KVCAnimationEntry@@@std@@ABK@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DF960, 83 bytes, thiscall, `ret 4`; the STLport const_iterator is a plain pointer.
// Identity (proven): its callers are the landed CAnimationSet::FindFirstAnimWithName 0x005DC020 (Ego_r.pdb QBE, returning the map's const_iterator; CAnimationSet::Anims is CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<...> >) and NCreatureMode::GetRandomAnimationFromCreature 0x00861E60; 20-byte stride = 4 + the 16-byte CAnimationEntry; no byte twin (Ego_r.pdb has only the const LowerBound of this map).
//   The manifest's "CVectorMap<unsigned long::LowerBound" is right in kind.
// Shape (family M158, representative 0x005DF960): the landed const LowerBound 0x006FAF80 (family J008) with a 20-byte
//   entry -- count = (last - first) / 20; halve; `first[half].first < key` (unsigned `jae`); return first. Speed TU.
#include "engine/CArray.h"


template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
};
class CAnimationEntry {                                   // Ego_r.pdb, 16 bytes: BankIndex, AnimName, GroupName, Components
public:
    unsigned long Members[4];
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
template class CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry > >;