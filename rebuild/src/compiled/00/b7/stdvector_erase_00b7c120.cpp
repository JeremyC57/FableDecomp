// std::vector::erase(iterator first, iterator last) -- (vector<CPreTransposedBoneMatrix >) STLport range erase of 48-byte trivially copied records: copy the tail [last, finish) down onto first element by element (`rep movsd` of 12 dwords), set finish to the copy's end, return first
// Leaf: ?erase@?$vector@UCPreTransposedBoneMatrix@@V?$allocator@UCPreTransposedBoneMatrix@@@std@@@std@@QAEPAUCPreTransposedBoneMatrix@@PAU3@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7C120, 99 bytes (+ int3 padding to 112), thiscall, `ret 8`, returns first.
// Identity (proven): std::vector<CPreTransposedBoneMatrix >::erase(iterator, iterator); evidence: its one caller is the landed vector<CPreTransposedBoneMatrix>::resize(size_type, const value_type&) 0x00B7EDF0 (run by the CEngineWaterRenderer render passes); 48-byte struct elements (`mov ecx, 0Ch`); the catalog binds this exact STLport name here.
// Label: the manifest's "Vector_Erase_30Byte" is propagated (family J015: six erase bodies of trivially destroyed elements of 36..72
//   bytes; retail has no identical-code folding, so each address is one element type).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` -- the default __type_traits give __false_type, so __copy_ptrs is the
//   random-access __copy loop (both inlined: `__forceinline`), and __destroy_aux over the trivial destructor is empty.
// Model: agent951 G040 (0x006CAD70, vector<CCompositeBlendDesc>).
#include <stddef.h>
struct CPreTransposedBoneMatrix {                                 // 48 bytes (Ego_r.pdb; struct key U)
    float Data[3][4];
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
template std::vector<CPreTransposedBoneMatrix >::iterator std::vector<CPreTransposedBoneMatrix >::erase(std::vector<CPreTransposedBoneMatrix >::iterator, std::vector<CPreTransposedBoneMatrix >::iterator);