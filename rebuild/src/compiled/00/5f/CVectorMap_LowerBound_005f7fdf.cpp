// CVectorMap::LowerBound(const long& key) -- (CVectorMap<long, long>) binary search of the sorted 8-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@JJV?$CKeyPairCompareLess@JJ@@@@QAEPAU?$pair@JJ@std@@ABJ@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F7FDF, 61 bytes, thiscall, `ret 4`; the STLport iterator is a plain pointer.
// Identity (proven): its callers are CTCInventoryAbilities::SetExpressionToButton 0x005F66D9 (Ego_r.pdb QAE, twice) on ExpressionButtons (Ego_r.pdb CVectorMap<long, long, CKeyPairCompareLess<long, long> >). The manifest's "CVectorMap<long::LowerBound" is right in kind.
// Shape (family M165, representative 0x005F7F4C): the landed LowerBound 0x006A29B0 (family J006, signed `jge`) with an
//   8-byte entry (`sar ecx,3`, `lea edx,[esi+eax*8]`). Size TU: /O2 folds the middle into the compare's address mode.
#pragma optimize("s", on)
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
    iterator LowerBound(const K& key);
};
template <class K, class V, class TCompare>
typename CVectorMap<K, V, TCompare>::iterator CVectorMap<K, V, TCompare>::LowerBound(const K& key)
{
    iterator first = Data.begin();
    int count = Data.end() - Data.begin();
    while (count > 0)
    {
        int half = count / 2;
        iterator middle = first + half;
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
template class CVectorMap<long, long, CKeyPairCompareLess<long, long > >;