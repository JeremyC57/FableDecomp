// CVectorMap::LowerBound(const unsigned long& key) const -- (CVectorMap<unsigned long, CAnimationEntry>) binary search of the sorted 20-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@KVCAnimationEntry@@V?$CKeyPairCompareLess@KVCAnimationEntry@@@@@@QBEPBU?$pair@KVCAnimationEntry@@@std@@ABK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DF960, 83 bytes, thiscall, `ret 4`; the STLport const_iterator is a plain pointer.
// Identity (proven): its callers are the landed CAnimationSet::FindFirstAnimWithName 0x005DC020 (Ego_r.pdb QBE, returning the map's const_iterator; CAnimationSet::Anims is CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<...> >) and NCreatureMode::GetRandomAnimationFromCreature 0x00861E60; 20-byte stride = 4 + the 16-byte CAnimationEntry; no byte twin (Ego_r.pdb has only the const LowerBound of this map).
//   The manifest's "CVectorMap<unsigned long::LowerBound" is right in kind.
// Shape (family M158, representative 0x005DF960): the landed const LowerBound 0x006FAF80 (family J008) with a 20-byte
//   entry -- count = (last - first) / 20; halve; `first[half].first < key` (unsigned `jae`); return first. Speed TU.
namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {   // the end-of-storage pointer
    _Value _M_data;
};
template <class _Tp, class _Alloc> class vector {           // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

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