// std::vector<CHeroMarriageInfo>::_M_fill_insert(CHeroMarriageInfo* position, unsigned int n, const CHeroMarriageInfo& x) -- insert n copies of x: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: _M_fill_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x00580928, 227 bytes, `ret 0Ch`.
// Callees (STLport `__fastcall` helpers of this TU, first in ecx, second in edx): __uninitialized_copy(first, last,
// result, const __false_type&) @ 0x0057F24E (landed), __uninitialized_fill_n(first, n, x, const __false_type&)
// @ 0x0057F2D1 (landed), __copy_backward(first, last, result, const random_access_iterator_tag&, int*) @ 0x0057F338,
// fill(first, last, x) @ 0x0057F279; CHeroMarriageInfo's copy ctor @ 0x0057A3A2 (for `__x_copy`);
// vector::_M_insert_overflow(position, x, const __false_type&, n, atend) @ 0x0058044A.
// Callers: 0x0058109A (the tail jump of insert(pos, n, x)) and 0x0058122F (the landed resize 0x005811F1 names this
// address as its Insert); 24-byte CHeroMarriageInfo records of the CTCHeroStats TU.
// Label: the catalog "Vector_InsertRangeInit" is descriptive: the body is STLport 4.x `_M_fill_insert` verbatim,
// the twin of the landed 0x0048608C (a 28-byte record there): `_Tp __x_copy = __x;` through the out-of-line copy
// ctor into the frame, elems_after kept in the dead `x` slot, old_finish - n in the same slot in the first arm,
// every empty tag in the high byte of the `position` slot (`lea [ebp+0Bh]`), the two final fill calls tail-merged.
// The same-TU helper bodies below (auto_inline off) give VC7.1 their register effects, as in the landed twin; the
// copy ctor's real memberwise body (0x0057A3A2: five dwords and the bool) is what keeps `_M_finish` in ebx across
// the `__x_copy` construction (declared only, VC7.1 re-reads it after the call).
// Model: CHeroMarriageInfo modelled locally (24 bytes, PDB members; its copy ctor is out of line).
// Shape: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <new>

class CHeroMarriageInfo { public:                          // 0x18 bytes (Ego_r.pdb)
    unsigned __int64 WifeUID;                       // +0x00
    unsigned __int64 MaritalHomeUID;                // +0x08
    long             State;                         // +0x10 EMarriageState
    bool             GayMarriage;                   // +0x14
    CHeroMarriageInfo(const CHeroMarriageInfo& other);   // 0x0057A3A2
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

#pragma auto_inline(off)
}
CHeroMarriageInfo::CHeroMarriageInfo(const CHeroMarriageInfo& other)                                         // 0x0057A3A2
    : WifeUID(other.WifeUID), MaritalHomeUID(other.MaritalHomeUID), State(other.State), GayMarriage(other.GayMarriage)
{
}
namespace std {
CHeroMarriageInfo* __fastcall __uninitialized_copy(CHeroMarriageInfo* __first, CHeroMarriageInfo* __last,
                                                   CHeroMarriageInfo* __result, const __false_type&)       // 0x0057F24E
{
    CHeroMarriageInfo* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) CHeroMarriageInfo(*__first);
    return __cur;
}
CHeroMarriageInfo* __fastcall __uninitialized_fill_n(CHeroMarriageInfo* __first, unsigned int __n,
                                                     const CHeroMarriageInfo& __x, const __false_type&)    // 0x0057F2D1
{
    for (; __n > 0; --__n, ++__first)
        new (__first) CHeroMarriageInfo(__x);
    return __first;
}
CHeroMarriageInfo* __fastcall __copy_backward(CHeroMarriageInfo* __first, CHeroMarriageInfo* __last,
                                              CHeroMarriageInfo* __result, const random_access_iterator_tag&,
                                              int*)                                                         // 0x0057F338
{
    for (int __n = int(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
void __fastcall fill(CHeroMarriageInfo* __first, CHeroMarriageInfo* __last, const CHeroMarriageInfo& __x)  // 0x0057F279
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

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                           // 0x0058044A
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

template void vector<CHeroMarriageInfo>::_M_fill_insert(CHeroMarriageInfo*, unsigned int, const CHeroMarriageInfo&);

}