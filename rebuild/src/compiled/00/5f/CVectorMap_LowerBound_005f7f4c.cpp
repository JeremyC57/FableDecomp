// CVectorMap::LowerBound(const long& key) -- (CVectorMap<long, EHeroAbility>) binary search of the sorted 8-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@JW4EHeroAbility@@V?$CKeyPairCompareLess@JW4EHeroAbility@@@@@@QAEPAU?$pair@JW4EHeroAbility@@@std@@ABJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F7F4C, 61 bytes, thiscall, `ret 4`; the STLport iterator is a plain pointer.
// Identity (proven): its callers are the non-const CTCInventoryAbilities::OnSpecialAbilityButtonPressed 0x005EED4C, SetAbilityToButton 0x005F6423 (landed: "LowerBound(const long&) @ 0x005F7F4C (non-const copy)"), SetAbilityAsAvailable 0x005F6BD6 and OnSerialise 0x005F6E40, all on AbilityButtons (Ego_r.pdb CVectorMap<long, EHeroAbility, CKeyPairCompareLess<long, EHeroAbility> >). The manifest's "CVectorMap<long::LowerBound" is right in kind.
// Shape (family M165, representative 0x005F7F4C): the landed LowerBound 0x006A29B0 (family J006, signed `jge`) with an
//   8-byte entry (`sar ecx,3`, `lea edx,[esi+eax*8]`). Size TU: /O2 folds the middle into the compare's address mode.
#pragma optimize("s", on)
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
enum EHeroAbility { HERO_ABILITY_NULL = 0 };
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
template class CVectorMap<long, EHeroAbility, CKeyPairCompareLess<long, EHeroAbility > >;