// std::vector<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry>::erase(iterator first, iterator last) -- STLport range erase: assign the tail [last, finish) down onto first, destroy the vacated (trivial) elements, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@VCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@V?$allocator@VCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@@std@@@std@@QAEPAVCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@PAV34@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00bcbf20, 81 bytes, thiscall, `ret 8`, returns first.
// Identity (proven): bound ?erase@?$vector@VCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@...@Z in reloc_bindings; landed callers CEngineInternalPrimitiveSpriteTrail::Update 0x00BCBC00 and vector<CSpriteTrailEntry>::resize 0x00BCC3B0 (the Trail array at this+0x68); 16-byte stride.
// Callees: none (everything inline).
// Shape (family L001, representative 0x00515BC0): STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first,
//   _TrivialAss()); _Destroy(__i, _M_finish); _M_finish = __i; return __first;` -- every user class is non-POD to STLport
//   (__false_type), so the copy is the random-access __copy loop `for (n = last - first; n > 0; --n) *result++ = *first++`
//   with the element's implicit memberwise operator= (four dword moves); the trivial destructors leave no loop. VC7.1
//   only expands __copy_ptrs/__copy here when they are __forceinline (plain `inline` leaves two out-of-line calls).
#include <stddef.h>
class C3DVector {                                           // 12 bytes
public:
    float X;                                              // +0x00
    float Y;                                              // +0x04
    float Z;                                              // +0x08
};
class CEngineInternalPrimitiveSpriteTrail {
public:
    class CSpriteTrailEntry {                             // Ego_r.pdb, 16 bytes
    public:
        C3DVector Position;                               // +0x00
        float Width;                                      // +0x0C
    };
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
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
}

template CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry* std::vector<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry>::erase(CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*, CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*);