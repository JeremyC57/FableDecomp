// CVectorMap<EHeroMorphType, CParticleMorphs::CEntry, CKeyPairCompareLess<EHeroMorphType, CParticleMorphs::CEntry> >::LowerBound(const EHeroMorphType& key) const -- binary search of the sorted 40-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@W4EHeroMorphType@@VCEntry@CParticleMorphs@@V?$CKeyPairCompareLess@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@@@@QBEPBU?$pair@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@std@@ABW4EHeroMorphType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00819e60, 83 bytes, thiscall `ret 4`; the STLport iterator is a plain entry pointer in eax.
// Identity (proven): CParticleMorphs::Morphs is CVectorMap<EHeroMorphType,CParticleMorphs::CEntry,...> (Ego_r.pdb; 40-byte pair = the body's `0x66666667 / sar 4` stride); sole caller CParticleMorphs::GetAllMorphsOfType 0x0081B160 (Ego_r.pdb QBE, const).
// Body: count = (last - first) / 40; while (count > 0) { half = count / 2; if (first[half].first < key) { first += half + 1;
//   count -= half + 1; } else count = half; } -- signed key compare (`jge`), the key read once before the loop, `count -= half + 1`
//   as `or -1 ; sub ; add`.
// Shape (family L069): the landed CVectorMap<unsigned long, CSmallVector<...> >::LowerBound 0x006FAF80 with a signed key.
enum EHeroMorphType { EHeroMorphType_FIRST };
enum EMeshLocationPoint { EMeshLocationPoint_FIRST };
class CDefString { public: long TablePos; };
class CParticleMorphs {
public:
    class CEntry {                                     // Ego_r.pdb CParticleMorphs::CEntry, 36 bytes
    public:
        EHeroMorphType      MorphType;                 // +0x00
        bool                Blending;                  // +0x04
        EMeshLocationPoint  Location;                  // +0x08
        CDefString          LocationName;              // +0x0C
        long                LocationIndex;             // +0x10
        long                ParticleIndex;             // +0x14
        long                SecondParticleIndex;       // +0x18
        float               StartAtMorphStrength;      // +0x1C
        float               EndAtMorphStrength;        // +0x20
    };
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

template class CVectorMap<EHeroMorphType, CParticleMorphs::CEntry, CKeyPairCompareLess<EHeroMorphType, CParticleMorphs::CEntry > >;