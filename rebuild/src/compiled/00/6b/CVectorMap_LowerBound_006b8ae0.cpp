// CVectorMap::LowerBound(const unsigned long& key) const -- (CVectorMap<unsigned long, CCountedPointer<CFaction>>) binary search of the sorted 12-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@KV?$CCountedPointer@VCFaction@@@@V?$CKeyPairCompareLess@KV?$CCountedPointer@VCFaction@@@@@@@@QBEPBU?$pair@KV?$CCountedPointer@VCFaction@@@@@std@@ABK@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B8AE0, 82 bytes, thiscall, `ret 4`; the STLport const_iterator is a plain pointer.
// Identity (proven): its callers are CFactionManager::GetPFaction(unsigned long) const 0x006B93A0 and GetPFaction(const CCharString&) 0x006B93D0 (Ego_r.pdb: both QBE) on CFactionManager::Factions (Ego_r.pdb CVectorMap<unsigned long, CCountedPointer<CFaction>, ...>); its non-const byte twin 0x006B8A80 serves CFaction loading / CWorld_InitFactions.
//   The manifest's "CVectorMap<unsigned long::LowerBound" is right in kind.
// Shape (family M158, representative 0x005DF960): the landed const LowerBound 0x006FAF80 (family J008) with a 12-byte
//   entry -- count = (last - first) / 12; halve; `first[half].first < key` (unsigned `jae`); return first. Speed TU.
#include "engine/CArray.h"


template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
};
class CFaction;
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T* Data;
    void* Info;
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
template class CVectorMap<unsigned long, CCountedPointer<CFaction>, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> > >;