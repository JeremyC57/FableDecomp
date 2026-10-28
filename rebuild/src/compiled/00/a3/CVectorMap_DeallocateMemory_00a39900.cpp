// CVectorMap::DeallocateMemory() -- (CVectorMap<unsigned long, long, CKeyPairCompareLess<unsigned long, long> >) clear the entries, give the array's block back and mark the map clean
// Leaf: ?DeallocateMemory@?$CVectorMap@KJV?$CKeyPairCompareLess@KJ@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A39900, 80 bytes. Name: FableWin/Ego_r PDB
//   ?DeallocateMemory@?$CVectorMap@KJV?$CKeyPairCompareLess@KJ@@@@QAEXXZ -- called from CCRCSymbolMap's Clear-like 0x00A38550
//   and LoadFromFile 0x00A39010 on SLONGMap (`lea ecx,[esi+4]`; Ego_r.pdb CCRCSymbolMap SLONGMap = CVectorMap<unsigned long,
//   long> at +4); the byte at +0xD is the map's Dirty flag (CVectorMap: Data +0, Compare +0xC, Dirty +0xD). The catalog label
//   "CActiveFile::OnReadFinished" is propagated. Callee: free @ 0x00BFEA14.
// Shape: STLport clear() (the counted __copy of the empty tail and the finish store), a swap with an empty local array
//   (zero stores), Dirty = false, then the local's destructor frees the old block at the end of the scope.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (MSVCR71 import thunk)
namespace std {
struct __false_type { __false_type() { } };
struct __true_type { __true_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {          // STLport pair: explicit copy constructor next to the converting one
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

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
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {   // the end-of-storage pointer over the empty allocator
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    void swap(vector<_Tp, _Alloc>& __x)
    {
        std::swap(this->_M_start, __x._M_start);
        std::swap(this->_M_finish, __x._M_finish);
        std::swap(this->_M_end_of_storage, __x._M_end_of_storage);
    }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > {   // Fable's CArray over the STLport vector
public:
    void DeallocateMemory()
    {
        if (this->capacity() > 0) {
            this->clear();
            CArray<T> empty;
            this->swap(empty);
        }
    }
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // sorted-on-demand vector map, retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                            // +0x00
    TCompare           Compare;                         // +0x0c (empty comparator)
    bool               Dirty;                           // +0x0d
    void DeallocateMemory()
    {
        Data.clear();
        CArray<value_type> empty;
        Data.swap(empty);
        Dirty = false;
    }
};

template void CVectorMap<unsigned long, long, CKeyPairCompareLess<unsigned long, long> >::DeallocateMemory();