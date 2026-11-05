// CVectorMap::LowerBound(const long& key) -- find the first entry whose key is at least key.
// Leaf: ?LowerBound@?$CVectorMap@JVCRegisteredMusicEntry@CMusicManager@@V?$CKeyPairCompareLess@JVCRegisteredMusicEntry@CMusicManager@@@@@@QAEPAU?$pair@JVCRegisteredMusicEntry@CMusicManager@@@std@@ABJ@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: proven; CMusicManager::RegisteredMusic at+28 (Ego_r); UpdateCurrentMusicOffset0x6BD8A0 and start-music0x6BD900 callers modify entries; stride24 matching nested20-byte record.
#include "engine/CCountedPointer.h"
class CRegionConnectionNode;
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

class CWideString { public: const unsigned short* PData; };
class CMusicManager { public: class CRegisteredMusicEntry { public: CWideString SrcFileName, DstFileName; float Gain, Offset; bool CacheToHD; }; };
template class CVectorMap<long, CMusicManager::CRegisteredMusicEntry, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry > >;