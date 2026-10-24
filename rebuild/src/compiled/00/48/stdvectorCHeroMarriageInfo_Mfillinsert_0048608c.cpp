// std::vector<CHeroMarriageInfo>::_M_fill_insert(CHeroMarriageInfo* position, size_t n, const CHeroMarriageInfo& x) -- STLport fill-insert of n copies of x: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048608C, 224 bytes, `ret 0Ch`.
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
// Label: the catalog's "Std_Vector_Insert_Copies"; callers 0x004861BE (a tail jump from insert(pos, n, x)) and 0x00486234.
// Leaf: _M_fill_insert
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <new>

struct CHeroMarriageInfo {                  // 28-byte record (copied as 7 dwords)
    unsigned long m[7];
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

#pragma auto_inline(off)
// Same-TU stand-ins for the out-of-line helpers (the landed bodies' effect; none touches its empty tag). With
// them visible VC7.1 homes every tag temporary over the high byte of the live `position` slot, as retail. The
// __uninitialized_copy stand-in is written as the counted loop (`n = last - first` divides, so edx is clobbered):
// retail re-reads old_finish from its slot after that call (`push dword ptr [ebp+10h]`, `mov edx,[ebp+10h]`),
// i.e. the TU's own instantiation did not preserve edx; the `first != last` form lets VC7.1 keep it in edx.
CHeroMarriageInfo* __fastcall __uninitialized_copy(CHeroMarriageInfo* __first, CHeroMarriageInfo* __last,
                                                   CHeroMarriageInfo* __result, const __false_type&)       // 0x0040DED0
{
    for (int __n = int(__last - __first); __n > 0; --__n, ++__first, ++__result)
        new (__result) CHeroMarriageInfo(*__first);
    return __result;
}
CHeroMarriageInfo* __fastcall __uninitialized_fill_n(CHeroMarriageInfo* __first, unsigned int __n,
                                                     const CHeroMarriageInfo& __x, const __false_type&)    // 0x0040DF00
{
    for (; __n > 0; --__n, ++__first)
        new (__first) CHeroMarriageInfo(__x);
    return __first;
}
CHeroMarriageInfo* __fastcall __copy_backward(CHeroMarriageInfo* __first, CHeroMarriageInfo* __last,
                                              CHeroMarriageInfo* __result, const random_access_iterator_tag&,
                                              int*)                                                         // 0x00485372
{
    for (int __n = int(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
void __fastcall fill(CHeroMarriageInfo* __first, CHeroMarriageInfo* __last, const CHeroMarriageInfo& __x)  // 0x0048521A
{
    for (; __first != __last; ++__first)
        *__first = __x;
}
#pragma auto_inline(on)

__forceinline CHeroMarriageInfo* __copy_backward_ptrs(CHeroMarriageInfo* __first, CHeroMarriageInfo* __last,
                                                      CHeroMarriageInfo* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

__forceinline CHeroMarriageInfo* uninitialized_fill_n(CHeroMarriageInfo* __first, unsigned int __n,
                                                      const CHeroMarriageInfo& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                           // 0x004125A0
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::_M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x)
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

template void vector<CHeroMarriageInfo>::_M_fill_insert(CHeroMarriageInfo*, unsigned int, const CHeroMarriageInfo&);

}