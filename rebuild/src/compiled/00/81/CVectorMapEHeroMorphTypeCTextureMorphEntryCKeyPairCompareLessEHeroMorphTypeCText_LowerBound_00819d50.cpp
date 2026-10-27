// CVectorMap<EHeroMorphType, CTextureMorphEntry, CKeyPairCompareLess<EHeroMorphType, CTextureMorphEntry> >::LowerBound(const EHeroMorphType& key) const -- binary search of the sorted 24-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@W4EHeroMorphType@@VCTextureMorphEntry@@V?$CKeyPairCompareLess@W4EHeroMorphType@@VCTextureMorphEntry@@@@@@QBEPBU?$pair@W4EHeroMorphType@@VCTextureMorphEntry@@@std@@ABW4EHeroMorphType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00819d50, 83 bytes, thiscall `ret 4`; the STLport iterator is a plain entry pointer in eax.
// Identity (proven): CTextureMorphs::Morphs is CVectorMap<EHeroMorphType,CTextureMorphEntry,CKeyPairCompareLess<...> > (Ego_r.pdb layout, 24-byte pair = the body's stride `lea [eax+eax*2]` x8); sole caller CTextureMorphs::GetAllMorphsOfType 0x0081B0A0 (Ego_r.pdb QBE, const) -- the const overload.
// Body: count = (last - first) / 24; while (count > 0) { half = count / 2; if (first[half].first < key) { first += half + 1;
//   count -= half + 1; } else count = half; } -- signed key compare (`jge`), the key read once before the loop, `count -= half + 1`
//   as `or -1 ; sub ; add`.
// Shape (family L069): the landed CVectorMap<unsigned long, CSmallVector<...> >::LowerBound 0x006FAF80 with a signed key.
enum EHeroMorphType { EHeroMorphType_FIRST };
class CTextureMorphEntry {                          // Ego_r.pdb class, 20 bytes: MorphType, TextureLayer, ReplacingTextureIndex, BankIndex, Blend
public:
    EHeroMorphType MorphType;                         // +0x00
    long           TextureLayer;                      // +0x04
    long           ReplacingTextureIndex;             // +0x08
    long           BankIndex;                         // +0x0C
    long           Blend;                             // +0x10 (ECompositeBlendType)
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

template class CVectorMap<EHeroMorphType, CTextureMorphEntry, CKeyPairCompareLess<EHeroMorphType, CTextureMorphEntry > >;