// std::vector::erase(iterator first, iterator last) -- (vector<CQuaternion >) STLport range erase of 16-byte records: copy the tail [last, finish) down onto first element by element, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@VCQuaternion@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A549D0, 81 bytes (+ int3 padding), thiscall, `ret 8`, returns first.
// Identity: vector<CQuaternion >::erase(iterator, iterator) -- probable; evidence: its three callers are the clear 0x00A54F50 and the resize(n, x) 0x00A55620 / resize(n) 0x00A55770 of the same vector, whose fill-insert 0x00A55190 C3DAnimationSequenceData::InitialiseFromChunk 0x00A4EFC0 runs on its local 16-byte rotation-track array (Ego_r.pdb CQuaternion* RotationTrack); FableWin.pdb holds vector<CQuaternion>::erase / resize / clear and no other 16-byte vector of that module (as the G035 __copy_ptrs<const CQuaternion*> 0x00A535F0).
// Label: the manifest's "_gapscan::sub_00a549d0" is propagated across byte-identical bodies; family G040 (8 erase bodies of
//   16-byte trivially destroyed elements).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` -- the default __type_traits give __false_type, so __copy_ptrs is the
//   random-access __copy loop (both inlined: `__forceinline`), and __destroy_aux over the trivial destructor is empty.
#include <stddef.h>
class CQuaternion {                                        // 16 bytes (Ego_r.pdb)
public:
    float X, Y, Z, W;
};
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                 _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
__forceinline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
}
template std::vector<CQuaternion >::iterator std::vector<CQuaternion >::erase(std::vector<CQuaternion >::iterator, std::vector<CQuaternion >::iterator);