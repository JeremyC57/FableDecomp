// std::vector<CUnprovenEnum4_00ae4fe0>::resize(unsigned int __new_size, const CUnprovenEnum4_00ae4fe0& __x) -- shrink by erasing the tail, or grow by filling copies of x at the end
// Leaf: ?resize@?$vector@W4CUnprovenEnum4_00ae4fe0@@V?$allocator@W4CUnprovenEnum4_00ae4fe0@@@std@@@std@@QAEXIABW4CUnprovenEnum4_00ae4fe0@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE7830, 76 bytes, thiscall, `ret 8`.
// Identity (descriptive): its fill-insert is the landed vector<CUnprovenEnum4_00ae4fe0>::_M_fill_insert 0x00AE6F90 (element not recoverable); callers CParticleProgram::ReadFromFile 0x00AE3C30 and the one-argument resize 0x00AE7AA0. The manifest's "vector<unsigned int>::resize" is propagated.
// Callees: vector<CUnprovenEnum4_00ae4fe0>::_M_fill_insert(pointer, size_type, const value_type&) 0x00AE6F90.
// Shape (family M168, representative 0x00619000): STLport 4.6 `if (__new_size < size()) erase(begin() + __new_size, end());
//   else insert(end(), __new_size - size(), __x);` -- erase is `__copy_ptrs(__last, _M_finish, __first, __false_type())` (the
//   counted __copy loop inline: an enum has no __type_traits specialisation), a no-op destroy, `_M_finish = __i`. Speed TU.
enum CUnprovenEnum4_00ae4fe0 { };                      // a 4-byte element (enum or one-word class) of an unidentified vector
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp> inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return size_type(_M_finish - _M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss());
        _Destroy(__i, _M_finish);
        _M_finish = __i;
        return __first;
    }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // 0x00AE6F90
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};
}
template void std::vector<CUnprovenEnum4_00ae4fe0>::resize(unsigned int, const CUnprovenEnum4_00ae4fe0&);