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
class CCharString { public: void* PStringData; };
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };
class CContainmentVolume;

class C3DMeshVolume {
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};

namespace std {
    template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
    template <class _Tp> class allocator { public: };
    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
    template <class _Tp, class _Alloc> class _Vector_base {
    public:
        _Tp* _M_start;
        _Tp* _M_finish;
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    };
    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : public _Vector_base<_Tp, _Alloc> {
    public:
        typedef const _Tp* const_iterator;
        const_iterator begin() const { return this->_M_start; }
        const_iterator end() const { return this->_M_finish; }
    };
}

template <class T> class CArray : public std::vector<T> { };

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