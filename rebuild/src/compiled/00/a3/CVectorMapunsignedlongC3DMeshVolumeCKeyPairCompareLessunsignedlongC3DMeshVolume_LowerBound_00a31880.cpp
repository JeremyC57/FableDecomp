// CVectorMap<unsigned long, C3DMeshVolume, CKeyPairCompareLess<unsigned long, C3DMeshVolume> >::LowerBound(const unsigned long&) const -- first entry not below the key
// Retail: Fable.exe 0x00A31880, 70 bytes: binary search over the sorted 0x10-byte pairs [Data._M_start, _M_finish):
// count = (end - begin) / 16; step = count / 2 (`cdq ; sub ; sar`); if (mid->first < key) { first = mid + 1;
// count -= step + 1; } else count = step; returns first; `ret 4`.
// Identity: called only from 0x00A2E980, the one-line C3DMeshStats wrapper `MeshVolumes.LowerBound(key)` (ecx + 0x58,
// the retail offset of Ego_r.pdb C3DMeshStats::MeshVolumes); 0x00A318D0 right after it is the UpperBound twin (called
// from 0x00A2E990). Ego_r.pdb: ?LowerBound@?$CVectorMap@KVC3DMeshVolume@@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@@QBE
// ... ABK@Z (const; retail's STLport const_iterator is a const pair pointer, the spelling the landed CVectorMap
// LowerBound rows use). CVectorMap retail layout: Data (CArray = STLport vector) +0, Compare +0xC, Dirty +0xD.
// Leaf: ?LowerBound@?$CVectorMap@KVC3DMeshVolume@@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@@QBEPBU?$pair@KVC3DMeshVolume@@@std@@ABK@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
class CCharString { public: void* PStringData; };
#include "engine/CCountedPointer.h"
class CContainmentVolume;

class C3DMeshVolume {
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};

#include "engine/CArray.h"


template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const K& b) const { return a.first < b; }
    bool operator()(const K& a, const std::pair<K, V>& b) const { return a < b.first; }
};

template <class K, class V, class TCompare> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    const_iterator LowerBound(const K& key) const;
    CArray<value_type> Data;                            // +0x00
    TCompare           Compare;                         // +0x0C
    bool               Dirty;                           // +0x0D
};

template <class K, class V, class TCompare>
typename CVectorMap<K, V, TCompare>::const_iterator CVectorMap<K, V, TCompare>::LowerBound(const K& key) const
{
    const_iterator first = Data.begin();
    long count = Data.end() - first;
    while (count > 0) {
        long step = count / 2;
        const_iterator it = first + step;
        if (Compare(*it, key)) {
            first = it + 1;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}

template class CVectorMap<unsigned long, C3DMeshVolume, CKeyPairCompareLess<unsigned long, C3DMeshVolume> >;