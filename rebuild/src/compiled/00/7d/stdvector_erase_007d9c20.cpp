// std::vector::erase(iterator first, iterator last) -- (vector<KnickKnack >) STLport range erase of 16-byte records: copy the tail [last, finish) down onto first element by element, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@UKnickKnack@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D9C20, 81 bytes (+ int3 padding), thiscall, `ret 8`, returns first.
// Identity: vector<KnickKnack >::erase(iterator, iterator) -- proven; evidence: the landed vector<KnickKnack>::resize(unsigned int) 0x007D9FC0 (Ego_r.pdb CTCTavernGameSpotTheAddition::KnickKnacks) shrinks through it, as does the fill-insert path 0x007D9F10; Ego_r.pdb struct KnickKnack is 16 bytes (Pos, Thing).
// Label: the manifest's "vector_Erase_16byte_Range" is propagated across byte-identical bodies; family G040 (8 erase bodies of
//   16-byte trivially destroyed elements).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` -- the default __type_traits give __false_type, so __copy_ptrs is the
//   random-access __copy loop (both inlined: `__forceinline`), and __destroy_aux over the trivial destructor is empty.
#include <stddef.h>
class C3DVector {                                          // 12 bytes
public:
    C3DVector() { }
    float X, Y, Z;
};
class CThingObject;
struct KnickKnack {                                         // 16 bytes (Ego_r.pdb struct)
    KnickKnack() { }
    C3DVector    Pos;
    CThingObject* Thing;
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
template std::vector<KnickKnack >::iterator std::vector<KnickKnack >::erase(std::vector<KnickKnack >::iterator, std::vector<KnickKnack >::iterator);