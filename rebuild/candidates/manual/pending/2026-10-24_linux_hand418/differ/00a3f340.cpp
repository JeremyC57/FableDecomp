// CSoundManagerLH::vfunc_41() -- empty the (name, name) object map, call slot 0 of every object in the name map, then empty the name map; each map re-sorts itself if it was left dirty
// Grade: DIFFER(183v184) -- register swap only: in the inline NameMap erase retail keeps the copy source (end) in edi and the result in ebp (ours: begin in edi, end in ebp), so retail re-reads [esi+68h] for the final sort where ours forwards edi; 4 grades (template __copy_ptrs, named first/last locals)
// Leaf: vfunc_41
// Retail: Fable.exe @ 0x00A3F340, 184 bytes, plain `ret`. Slot 41 of the CSoundManagerLH vtable 0x0129D2C4
//   (rtti-vtables.tsv; the only reference to the function).
// Callees: vector<pair<pair<CCharString, CCharString>, T*> >::erase(first, last) 0x00A40C10 (out of line), its sort
//   0x00A42060, CCharString::operator= 0x0099EFB0 and ~CCharString 0x0099EAE0 (the name map's erase inline), the
//   name map's sort 0x00A41E20, slot 0 of each mapped object.
// Label: catalog "CParticleMorphs_CommitChanges" is a fold label; the audio layer is absent from the PDBs, so the slot
//   keeps the lane's vfunc_N name.
// Layout: the two retail 16-byte CVectorMaps of the landed FindByName / FindByNamePair rows (0x00A3ED30 / 0x00A3EE50):
//   NamePairMap +0x54 (Comp +0x60, Dirty +0x61), NameMap +0x64 (Comp +0x70, Dirty +0x71).
// Shape: each clear is STLport `erase(begin(), end())` (copy the tail down, destroy the rest, store the new finish)
//   followed by the inline CVectorMap::Update (`if (Dirty) { sort(begin, end, Comp); Dirty = false; }`, the empty
//   comparator copied by value as one byte); VC7.1 keeps the 12-byte-entry erase out of line and inlines the
//   8-byte-entry one; the slot-0 loop re-reads end() after every call.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00A42060 / 0x00A41E20

template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, _M_finish, __first, __false_type());
        _Destroy(__i, _M_finish);
        _M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}

template <class K, class V> class CKeyPairCompareLess { };

template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap {   // retail 16 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef value_type*      iterator;
    std::vector<value_type> Data;                          // +0x00
    C                        Comp;                          // +0x0C (empty comparator)
    bool                     Dirty;                         // +0x0D
    iterator Begin() { return Data.begin(); }
    iterator End() { return Data.end(); }
    void Sort()
    {
        std::sort(Data.begin(), Data.end(), Comp);
        Dirty = false;
    }
    void Update()
    {
        if (Dirty)
            Sort();
    }
    void Clear()
    {
        Data.clear();
        Update();
    }
};

class CSoundBankLH {                                        // descriptive: the named sound objects of the two maps
public:
    virtual void Unload();                                  // slot 0 (+0x00), no arguments
};

class CSoundManagerLH {                                     // vtable 0x0129D2C4
public:
    virtual void v00();
    unsigned char _04[0x50];                                // +0x04..+0x53
    CVectorMap<std::pair<CCharString, CCharString>, CSoundBankLH*> NamePairMap;   // +0x54
    CVectorMap<CCharString, CSoundBankLH*>                          NameMap;       // +0x64
    void vfunc_41();
};

void CSoundManagerLH::vfunc_41()
{
    NamePairMap.Clear();
    for (CVectorMap<CCharString, CSoundBankLH*>::iterator it = NameMap.Begin(); it != NameMap.End(); ++it)
        it->second->Unload();
    NameMap.Clear();
}
