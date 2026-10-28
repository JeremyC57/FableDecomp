#include <stdio.h>
class CCPPointerInfo;
template <class T> class CCountedPointer {                   // 8 bytes: object, shared count block
public:
    T* Data;                                                  // +0x0
    CCPPointerInfo* Info;                                     // +0x4
};
class CRegionConnectionNode;
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

class CWideString { public: const unsigned short* PData; };
class CMusicManager { public: class CRegisteredMusicEntry { public: CWideString SrcFileName, DstFileName; float Gain, Offset; bool CacheToHD; }; };
template class CVectorMap<long, CMusicManager::CRegisteredMusicEntry, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry > >;

typedef CVectorMap<long, CMusicManager::CRegisteredMusicEntry, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry > > Map;

int main() {
 Map m; Map::value_type entries[5]; int i; long keys[5]={-8,0,0,6,19};
 for(i=0;i<5;++i) entries[i].first=keys[i];
 m.Data._M_start=entries; m.Data._M_finish=entries+5; m.Data._M_end_of_storage._M_data=entries+5;
 long probes[8]={-10,-8,-1,0,1,6,19,20}; int wanted[8]={0,0,1,1,3,3,4,5};
 for(i=0;i<8;++i) { const Map& cm=m; if(m.LowerBound(probes[i])!=entries+wanted[i]) { puts("006be5e0_TEST FAIL"); return 1; } }
 m.Data._M_finish=entries; long key=5; if(m.LowerBound(key)!=entries) { puts("006be5e0_TEST FAIL"); return 1; }
 puts("006be5e0_TEST PASS"); return 0;
}