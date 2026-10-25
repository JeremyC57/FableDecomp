// std::vector::erase(iterator first, iterator last) -- (vector<EActionRegisteredType >) STLport range erase for a 4-byte element whose type traits are not specialised: assign the tail [last, finish) down onto first one element at a time, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@W4EActionRegisteredType@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046CBC2, 47 bytes, thiscall, `ret 8`, returns first; no calls.
// Identity: vector<EActionRegisteredType > -- proven; evidence: its resize 0x0046CB84 is called only by GFSerialiseVectorBinaryIn 0x0046CAF4 <- TransferBinary 0x0046CA94, which CCombatSequenceDef::Transfer 0x0046C6F0 calls on this+0x54 = Ego_r.pdb CCombatSequenceDef::ActionList (vector<EActionRegisteredType>, PDB +88 less 4); the PDB name set holds ?erase@?$vector@W4EActionRegisteredType@@ (a Dinkumware
//   spelling, the element type only).
// Label: the manifest's "std::vector_copy_4byte_norealloc" is propagated across byte-identical bodies (its only caller is the size TU's
//   vector<EActionRegisteredType >::resize(n, x) 0x0046CB84, which the catalog calls CArray<std::pair<long, ...> >::push_back).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` -- __type_traits<EActionRegisteredType> is the unspecialised template (all __false_type), so
//   __copy_ptrs takes the __copy(random_access_iterator_tag) element loop instead of memmove, and the destroy loop of a
//   trivially destructible element folds away.
// Shape: a size-optimised def TU (retail packs these functions back to back at odd addresses): `optimize("s", on)`,
//   under which VC7.1 keeps STLport's plain-inline helpers out of line, so __copy_ptrs / __copy / _Destroy are
//   `__forceinline` (a plain-inline __copy_ptrs is a call, graded).
#pragma optimize("s", on)
#include <stddef.h>
enum EActionRegisteredType { };
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> struct __type_traits {
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
};
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
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
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
__forceinline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
template <class _Tp> __forceinline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
__forceinline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> __forceinline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _TrivialAss;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
typedef EActionRegisteredType _Elem;
template vector<_Elem>::iterator vector<_Elem>::erase(vector<_Elem>::iterator, vector<_Elem>::iterator);
}