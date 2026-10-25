// std::vector::erase(iterator first, iterator last) -- (vector<std::vector<NThingMap::CThingMapCell> >) STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, free each vacated element's block, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@V?$vector@VCThingMapCell@NThingMap@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085EA40, 84 bytes (+ int3 padding), thiscall, `ret 8`, returns first.
// Identity: vector<std::vector<NThingMap::CThingMapCell> >::erase(iterator, iterator) -- proven; evidence: its one caller is the vector<vector<NThingMap::CThingMapCell> >::resize 0x0085ED20 that CThingMap::InitMapwhoLevels 0x0085D180 runs on Ego_r.pdb CThingMap::Cells (vector<vector<NThingMap::CThingMapCell> >); FableWin.pdb holds that erase and resize.
// Label: the manifest's "std::vector::std::vector::Reallocate_pair_copyback" is propagated across byte-identical bodies; family G041 (8 erase bodies whose
//   12-byte element owns a malloc block at +0: `add esi,Ch`).
// Callees: std::__copy<std::vector<NThingMap::CThingMapCell>*, std::vector<NThingMap::CThingMapCell>*, int> 0x0085E470 (`__fastcall`: last in ecx, finish in edx; first, the
//   `random_access_iterator_tag` reference and `(int*)0` on the stack; retail homes the empty tag in the LIVE `first`
//   slot, which VC7.1 does only when it sees the same-TU __copy body never touches the tag, so the body is given here),
//   free 0x00BFEA14 (each vacated element's inlined `_Vector_base` destructor).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` with __copy_ptrs(__false_type) = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)` and _Destroy = the __destroy_aux(__false_type) loop of `p->~T()`.
#include <stddef.h>
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
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
    ~_Vector_base() { if (_M_start) free(_M_start); }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>                  // 0x0085E470 (same-TU body)
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
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
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
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
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
}
namespace NThingMap {
class CMapwhoNode;
class CThingMapCell { public: CMapwhoNode* PFirstNode; };
}
template std::vector<std::vector<NThingMap::CThingMapCell> >::iterator std::vector<std::vector<NThingMap::CThingMapCell> >::erase(std::vector<std::vector<NThingMap::CThingMapCell> >::iterator, std::vector<std::vector<NThingMap::CThingMapCell> >::iterator);