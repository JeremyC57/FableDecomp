// std::vector<CDateAndTime>::_M_fill_insert(CDateAndTime* position, size_t n, const CDateAndTime& x) -- STLport fill-insert of n copies of x: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048608C, 224 bytes, `ret 0Ch`.
// Identity: callers are vector<CDateAndTime>::resize(n, x) (0x004861F7, call 0x00486234; its own caller resize(n)
// 0x0048623F builds the value with CDateAndTime::CDateAndTime 0x009E9AB0, and is called from CTCInGameMenu
// GetFileNameList and CFrontEndManager GetSavedGameNames) and a tail jump at 0x004861BE. Ego_r.pdb CDateAndTime is 28
// bytes (Year, Month, Day, Hour, Minute, Second, Millisecond) = the 7-dword record copied here; the earlier
// CDateAndTime label is 24 bytes in the PDB.
// Callees (all `__fastcall` STLport helpers, first in ecx, second in edx): __uninitialized_copy(first, last, result,
// const __false_type&) @ 0x0040DED0, __uninitialized_fill_n(first, n, x, const __false_type&) @ 0x0040DF00,
// __copy_backward(first, last, result, const random_access_iterator_tag&, int*) @ 0x00485372, fill(first, last,
// x) @ 0x0048521A (all landed); vector::_M_insert_overflow(position, x, const __false_type&, n, atend) @ 0x004125A0.
// Shape: STLport 4.x `_M_fill_insert` verbatim for a 28-byte record (STLport has no type traits for it, so every
// helper takes the `__false_type` path): `_Tp __x_copy = __x;` (a `rep movsd` of 7 dwords into the frame), the
// elems_after / n comparison, `__copy_backward_ptrs` inlined to the counted `__copy_backward(..., (int*)0)`, the
// public 3-argument `uninitialized_fill_n` in the second arm. Both arms start with the tag push, which VC7.1
// hoists above the `jbe`; the two final fill calls are tail-merged; old_finish reuses the dead `x` slot and every
// empty tag lives in the high byte of the `position` slot (`lea [ebp+0Bh]`, which needs the same-TU helper
// bodies below). Frame-pointer size TU (`optimize("y", off)` + `("s", on)`).
// Leaf: _M_fill_insert
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <new>

class CDateAndTime {                        // Ego_r.pdb, 28 bytes (copied as 7 dwords)
public:
    long          Year;                     // +0x00
    unsigned long Month;                    // +0x04
    unsigned long Day;                      // +0x08
    unsigned long Hour;                     // +0x0C
    unsigned long Minute;                   // +0x10
    unsigned long Second;                   // +0x14
    unsigned long Millisecond;              // +0x18
    CDateAndTime();                         // 0x009E9AB0
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

#pragma auto_inline(off)
// Same-TU stand-ins for the out-of-line helpers (the landed bodies' effect; none touches its empty tag), written as
// explicit specialisations of the STLport templates so the call targets carry retail's template symbol names. With
// them visible VC7.1 homes every tag temporary over the high byte of the live `position` slot, as retail. The
// __uninitialized_copy stand-in is written as the counted loop (`n = last - first` divides, so edx is clobbered):
// retail re-reads old_finish from its slot after that call (`push dword ptr [ebp+10h]`, `mov edx,[ebp+10h]`),
// i.e. the TU's own instantiation did not preserve edx; the `first != last` form lets VC7.1 keep it in edx.
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*);
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);

template <>
CDateAndTime* __fastcall __uninitialized_copy<CDateAndTime*, CDateAndTime*>(CDateAndTime* __first, CDateAndTime* __last,
                                                   CDateAndTime* __result, const __false_type&)       // 0x0040DED0
{
    for (int __n = int(__last - __first); __n > 0; --__n, ++__first, ++__result)
        new (__result) CDateAndTime(*__first);
    return __result;
}
template <>
CDateAndTime* __fastcall __uninitialized_fill_n<CDateAndTime*, unsigned int, CDateAndTime>(CDateAndTime* __first,
                                                     unsigned int __n, const CDateAndTime& __x, const __false_type&)    // 0x0040DF00
{
    for (; __n > 0; --__n, ++__first)
        new (__first) CDateAndTime(__x);
    return __first;
}
template <>
CDateAndTime* __fastcall __copy_backward<CDateAndTime*, CDateAndTime*, int>(CDateAndTime* __first, CDateAndTime* __last,
                                              CDateAndTime* __result, const random_access_iterator_tag&,
                                              int*)                                                         // 0x00485372
{
    for (int __n = int(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <>
void __fastcall fill<CDateAndTime*, CDateAndTime>(CDateAndTime* __first, CDateAndTime* __last, const CDateAndTime& __x)  // 0x0048521A
{
    for (; __first != __last; ++__first)
        *__first = __x;
}
#pragma auto_inline(on)

__forceinline CDateAndTime* __copy_backward_ptrs(CDateAndTime* __first, CDateAndTime* __last,
                                                      CDateAndTime* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

__forceinline CDateAndTime* uninitialized_fill_n(CDateAndTime* __first, unsigned int __n,
                                                      const CDateAndTime& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp> class allocator;

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                           // 0x004125A0
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if ((unsigned int)(_M_end_of_storage - _M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = _M_finish - __position;
            pointer __old_finish = _M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(_M_finish - __n, _M_finish, _M_finish, __false_type());
                _M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
                _M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, _M_finish, __false_type());
                _M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template void vector<CDateAndTime>::_M_fill_insert(CDateAndTime*, unsigned int, const CDateAndTime&);

}