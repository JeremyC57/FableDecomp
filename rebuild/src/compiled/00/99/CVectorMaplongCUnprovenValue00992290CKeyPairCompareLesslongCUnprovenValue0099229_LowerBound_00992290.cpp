// CVectorMap<long, CUnprovenValue_00992290, CKeyPairCompareLess<long, CUnprovenValue_00992290> >::LowerBound(const long& key) const -- binary search of the sorted 36-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@JVCUnprovenValue_00992290@@V?$CKeyPairCompareLess@JVCUnprovenValue_00992290@@@@@@QBEPBU?$pair@JVCUnprovenValue_00992290@@@std@@ABJ@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00992290, 83 bytes, thiscall `ret 4`; the STLport iterator is a plain entry pointer in eax.
// Identity (descriptive): its callers 0x00991840 (a find on the map at +0x30 of the global 0x013B8394 returning entry->second's first dword) and 0x00992590 use a long-keyed CVectorMap with 36-byte entries (`0x38e38e39 / sar 3`, `lea [eax+eax*8]` x4); the landed caller's binding CVectorMap<long, CASoundBank*> cannot hold a 36-byte entry, so the value type is unproven.
// Body: count = (last - first) / 36; while (count > 0) { half = count / 2; if (first[half].first < key) { first += half + 1;
//   count -= half + 1; } else count = half; } -- signed key compare (`jge`), the key read once before the loop, `count -= half + 1`
//   as `or -1 ; sub ; add`.
// Shape (family L069): the landed CVectorMap<unsigned long, CSmallVector<...> >::LowerBound 0x006FAF80 with a signed key.
class CUnprovenValue_00992290 {                     // descriptive: a 32-byte value whose first dword is the looked-up object pointer
public:
    void*         Object;                            // +0x00
    unsigned char _rest[28];                         // +0x04
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

template class CVectorMap<long, CUnprovenValue_00992290, CKeyPairCompareLess<long, CUnprovenValue_00992290 > >;