// std::vector<CFadeInFadeOutBase>::_M_fill_insert(CFadeInFadeOutBase* position, unsigned int n, const CFadeInFadeOutBase& x) -- insert n copies of a fade record: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: _M_fill_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594A73, 217 bytes, `ret 0Ch`.
// Callees (STLport `__fastcall` helpers of this TU, first in ecx, second in edx): __uninitialized_copy(first, last,
// result, const __false_type&) @ 0x00593851 (landed), __uninitialized_fill_n(first, n, x, const __false_type&)
// @ 0x00592FD5 (landed), __copy_backward(first, last, result, const random_access_iterator_tag&, int*) @ 0x005927AB,
// fill(first, last, x) @ 0x0059264A (both `rep movsd` assignments); CFadeInFadeOutBase's copy ctor @ 0x00592D97
// (landed; for `__x_copy`); vector::_M_insert_overflow(position, x, const __false_type&, n, atend) @ 0x00594880
// (this chunk).
// Callers: 0x00594C0B (the tail jump of insert(pos, n, x)) and 0x00594D2B (resize(n, x) 0x00594CEE), on the
// CTCInventory-TU vector<CFadeInFadeOutBase> resized at 0x00590EAF.
// Label: the catalog "Vector_InsertRange_CFadeInFadeOut" is descriptive: the body is STLport 4.x `_M_fill_insert`
// verbatim, the same template as the CTCHeroStats-TU vector<CHeroMarriageInfo> one @ 0x00580928: `_Tp __x_copy =
// __x;` through the out-of-line copy ctor into the frame, every empty tag in the high byte of the `position` slot
// (`lea [ebp+0Bh]`), the two final fill calls tail-merged. Here the copy ctor is only declared (unlike 0x00580928,
// whose visible copy-ctor body keeps `_M_finish` in ebx): VC7.1 then re-reads `_M_finish` after the `__x_copy`
// construction and keeps old_finish in the dead `x` slot, elems_after in ebx, as retail.
// Model: a local CFadeInFadeOutBase (the engine header's layout) because the header cannot declare the out-of-line
// copy constructor.
// Shape: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <new>

class CFadeInFadeOutBase {                           // 0x1C bytes (Ego_r.pdb; engine/CFadeInFadeOutBase.h has no copy ctor)
public:
    bool          Active;                            // +0x00
    float         FadeInTime;                        // +0x04
    float         FadeOutTime;                       // +0x08
    bool          Closing;                           // +0x0c
    bool          Opening;                           // +0x0d
    float         OpenTimer;                         // +0x10
    float         CloseTimer;                        // +0x14
    unsigned char ToColour[0x4];                     // +0x18 CRGBColour
    CFadeInFadeOutBase(const CFadeInFadeOutBase& other);   // 0x00592D97
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

#pragma auto_inline(off)
CFadeInFadeOutBase* __fastcall __uninitialized_copy(CFadeInFadeOutBase* __first, CFadeInFadeOutBase* __last,
                                                   CFadeInFadeOutBase* __result, const __false_type&)       // 0x00593851
{
    CFadeInFadeOutBase* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) CFadeInFadeOutBase(*__first);
    return __cur;
}
CFadeInFadeOutBase* __fastcall __uninitialized_fill_n(CFadeInFadeOutBase* __first, unsigned int __n,
                                                     const CFadeInFadeOutBase& __x, const __false_type&)    // 0x00592FD5
{
    for (; __n > 0; --__n, ++__first)
        new (__first) CFadeInFadeOutBase(__x);
    return __first;
}
CFadeInFadeOutBase* __fastcall __copy_backward(CFadeInFadeOutBase* __first, CFadeInFadeOutBase* __last,
                                              CFadeInFadeOutBase* __result, const random_access_iterator_tag&,
                                              int*)                                                         // 0x005927AB
{
    for (int __n = int(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
void __fastcall fill(CFadeInFadeOutBase* __first, CFadeInFadeOutBase* __last, const CFadeInFadeOutBase& __x)  // 0x0059264A
{
    for (; __first != __last; ++__first)
        *__first = __x;
}
#pragma auto_inline(on)

__forceinline CFadeInFadeOutBase* __copy_backward_ptrs(CFadeInFadeOutBase* __first, CFadeInFadeOutBase* __last,
                                                      CFadeInFadeOutBase* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                           // 0x00594880
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
                __uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy, __false_type());
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

template void vector<CFadeInFadeOutBase>::_M_fill_insert(CFadeInFadeOutBase*, unsigned int, const CFadeInFadeOutBase&);

}