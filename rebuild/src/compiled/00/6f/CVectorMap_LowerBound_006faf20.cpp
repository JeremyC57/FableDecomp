// CVectorMap::LowerBound(const unsigned long& key) -- (CVectorMap<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> >) binary search of the sorted 12-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@V?$CKeyPairCompareLess@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@@@@QAEPAU?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@std@@ABK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FAF20, 82 bytes (+ int3 padding to 96), thiscall `ret 4`; the STLport
//   iterator is a plain pointer returned in eax.
// Identity (proven): CVectorMap<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32>, CKeyPairCompareLess<...> >::LowerBound(const unsigned long&); evidence: NAnimationEvents::CManager::AnimToExtraEventsMap (Ego_r.pdb CVectorMap<unsigned long, CSmallVector<NAnimationEvents::CEvent,32>, ...>, retail +0x2C after the 36-byte retail CSymbolMap); called by CManager::AddEventToAnim 0x006FA3F0 (0x006FA402; Ego_r.pdb ?AddEventToAnim@CManager@NAnimationEvents@@QAEXKABVCEvent@2@@Z, non-const) -- the non-const overload; its byte twin 0x006FAF80 serves EventExists (QBE), AnimHasExtraEvent / GetExtraEventNames / GetExtraEventsBetweenTimes (ABE). FableWin.pdb names both overloads.
// Label: the manifest's "LowerBound" is propagated (family J008: six byte-identical bodies, unsigned key (`jae`); retail has no identical-code
//   folding, so each address is one instantiation and one const-ness: both PDBs name a non-const LowerBound returning an
//   iterator and a const one returning a const_iterator, and the two compile to the same bytes).
// Body: count = (last - first) / 12; while (count > 0) { half = count / 2; if (first[half].first < key) { first += half + 1;
//   count -= half + 1; } else count = half; } return first; -- the key read once before the loop, `count -= half + 1` as
//   `count += -1 - half`, the signed `cdq/sub/sar` halving.
// Layout (retail CVectorMap 0x10): CArray<pair<K, V> > Data +0 (STLport vector: _M_start, _M_finish, _M_end_of_storage),
//   the empty comparator +0xC, Dirty +0xD.
// Model: the landed 0x00447C80 body (J006) with an unsigned key.
namespace NAnimationEvents { class CEvent; }
template <class T, int N> class CSmallVector {                 // Ego_r.pdb CSmallVector<NAnimationEvents::CEvent,32>, 8 bytes
public:
    T*  Start;                                                // +0x0
    int Size;                                                 // +0x4
};
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

template class CVectorMap<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32>, CKeyPairCompareLess<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > >;