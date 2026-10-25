// std::vector::erase(iterator first, iterator last) -- (vector<CAbilityButtonGuiDef >) STLport range erase: assign the tail [last, finish) down onto first (T::operator= per element), run each vacated element's virtual destructor through __destroy_aux, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@VCAbilityButtonGuiDef@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00476672, 80 bytes (size-optimised TU, frame pointer, odd entry), thiscall, `ret 8`, returns first.
// Identity: std::vector<CAbilityButtonGuiDef >::erase(iterator, iterator) -- proven; evidence: its only caller is the vector<CAbilityButtonGuiDef >::resize
//   0x0047662A (n < size() branch; 0x5C-byte elements: the /0x5C divide), called from CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
//   0x0047652B (from CPlayerGuiDef::Transfer 0x004736C4 (twice: Ego_r AbilityButtons / SpecialAbilityButtons)), which builds its default element inline (vtable 0x0123610C).
//   Family G067 (seven byte-identical-shape bodies, one COMDAT per T). Catalogue label "Vector_ShiftAbilityButtonsRight" is behavioural; the
//   landed callers 0x0047662A filed as "CArray<std::pair<unsigned long..>>::push_back" are vector<T>::resize(n, x).
// Callees: CAbilityButtonGuiDef::operator=(const CAbilityButtonGuiDef&) 0x0046F43E; std::__destroy_aux<CAbilityButtonGuiDef*>(first, last, const __false_type&) 0x00450A58
//   (`__fastcall`: first in ecx, last in edx, the tag reference on the stack; the same-TU body below never touches the
//   tag, which lets VC7.1 home it in the live `first` slot ([ebp+0Bh]) as retail does).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` -- /Os keeps __copy only under __forceinline (offset-register loop, count in the dead
//   `last` slot) and leaves __destroy_aux out of line; the tags have user constructors (no zero-store).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
class CAbilityButtonGuiDef {                                  // retail sizeof 0x5C, polymorphic (virtual destructor in slot 0)
public:
    virtual ~CAbilityButtonGuiDef();
    CAbilityButtonGuiDef& operator=(const CAbilityButtonGuiDef& other);      // 0x0046F43E
    char _data[0x5C - 4];
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
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
#pragma auto_inline(off)
template <class _ForwardIterator>                                                        // 0x00450A58 (same-TU body)
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)
template <class _Tp> __forceinline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
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
typedef CAbilityButtonGuiDef _Elem;
template vector<_Elem>::iterator vector<_Elem>::erase(vector<_Elem>::iterator, vector<_Elem>::iterator);
}