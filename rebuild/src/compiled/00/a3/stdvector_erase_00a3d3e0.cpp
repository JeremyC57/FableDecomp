// std::vector::erase(iterator first, iterator last) -- (vector<C3DAnimationStateAlpha >) STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, run each vacated element's virtual destructor, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3D3E0, 77 bytes, thiscall, `ret 8`, returns first.
// Identity: vector<C3DAnimationStateAlpha > -- proven; evidence: its callers are C3DAnimationBlendState::ClearCombinationBlends 0x00A3C330 (landed), CSharedData::~CSharedData 0x00A3D460 and a resize 0x00A3D500: Ego_r.pdb C3DAnimationBlendState::CSharedData::CombinationBlends is CArray<C3DAnimationStateAlpha> (a vector base) and AddCombinationBlend takes a const C3DAnimationStateAlpha&; 20-byte elements (`add esi,14h`, the /20 divide of its __copy 0x00A3CF20) = Ego_r.pdb sizeof(C3DAnimationStateAlpha); RTTI vtable 0x01265128 .?AVC3DAnimationStateAlpha@@.
// Label: the manifest's "Vector_Erase_Single" is propagated across byte-identical bodies; this is STLport 4.6
//   `vector::erase(iterator, iterator)` (20-byte elements: `add esi,14h`).
// Callees: std::__copy<C3DAnimationStateAlpha*, C3DAnimationStateAlpha*, int> @ 0x00A3CF20 (`__fastcall`: last in ecx, finish in edx; first, the
//   `random_access_iterator_tag` reference and `(int*)0` on the stack; retail homes the empty tag in the LIVE `first`
//   slot, which VC7.1 does only when it sees the same-TU __copy body never touches the tag, so the body is given here), then C3DAnimationStateAlpha's virtual destructor (vtable slot 0, `push 0 ; call [edx]`) per destroyed element.
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` with __copy_ptrs(__false_type) = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)` and _Destroy = the __destroy_aux(__false_type) loop of `p->~T()`.
#include <stddef.h>
class C3DAnimationStateAlpha {                                  // 20 bytes, vtable 0x01265128 (RTTI .?AVC3DAnimationStateAlpha@@)
public:
    virtual ~C3DAnimationStateAlpha();                       // slot 0
    C3DAnimationStateAlpha& operator=(const C3DAnimationStateAlpha& other);
    char _data[16];
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
template <class _RandomAccessIter, class _OutputIter, class _Distance>                  // 0x00A3CF20 (same-TU body)
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
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
typedef C3DAnimationStateAlpha _Elem;
template vector<_Elem>::iterator vector<_Elem>::erase(vector<_Elem>::iterator, vector<_Elem>::iterator);
}