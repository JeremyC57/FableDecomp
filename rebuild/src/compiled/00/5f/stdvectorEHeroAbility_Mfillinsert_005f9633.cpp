// std::vector<EHeroAbility>::_M_fill_insert(EHeroAbility* position, unsigned int n, const EHeroAbility& x) -- STLport insert of n copies of x: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F9633, 225 bytes, `ret 0Ch`; callers: the tail jump of insert(pos, n, x)
//   0x005F9C4A and resize(n, x) 0x005F9DF8 (landed stdvector<EHeroAbility>::resize).
// Callees (STLport `__fastcall` helpers of this /Gr TU): __uninitialized_copy(first, last, result, const __false_type&)
//   0x005F79BD (landed), __copy_backward(first, last, result, const random_access_iterator_tag&, int*) 0x005F7C13 (this
//   chunk), uninitialized_fill_n(first, n, x) 0x005F8220 (landed 3-argument front end), _M_insert_overflow(position, x,
//   const __false_type&, fill_len, atend) 0x005F9495 (this chunk). Both `fill` loops are inlined.
// Label correction: the catalog `std_Vector_InsertSingleRealloc` is behavioural; the body is STLport 4.x
//   `_M_fill_insert` verbatim (the landed 0x00594A73 template): `_Tp __x_copy = __x;` lives in the dead n slot,
//   elems_after in the x slot and old_finish in the position slot; every empty tag in the high byte of the x slot
//   (`lea eax,[ebp+13h]`).
// Shape: frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`; same-TU helper bodies (auto_inline off)
//   let VC7.1 keep `__x_copy` in the dead n slot and home the tags over the dead x byte, as retail (declarations only:
//   226 v 225, `__x_copy` gets a `push ecx` frame word). VC7.1 also uses the clobber set of those compiled bodies: the
//   `__uninitialized_copy` stand-in counts down in edx so edx is not assumed to survive the call -- retail re-reads
//   `old_finish - n` and `old_finish` from their slots after it (a `first != last` stand-in keeps them in edx: 222 v 225).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <new>

enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

#pragma auto_inline(off)
EHeroAbility* __fastcall __uninitialized_copy(EHeroAbility* __first, EHeroAbility* __last, EHeroAbility* __result,
                                              const __false_type&)                                       // 0x005F79BD
{                                                        // stand-in (clobbers edx like an unknown callee)
    EHeroAbility* __cur = __result;
    for (int __n = int(__last - __first); __n > 0; --__n, ++__first, ++__cur)
        new (__cur) EHeroAbility(*__first);
    return __cur;
}
EHeroAbility* __fastcall __copy_backward(EHeroAbility* __first, EHeroAbility* __last, EHeroAbility* __result,
                                         const random_access_iterator_tag&, int*)                        // 0x005F7C13
{
    for (int __n = int(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
EHeroAbility* __fastcall uninitialized_fill_n(EHeroAbility* __first, unsigned int __n, const EHeroAbility& __x)  // 0x005F8220
{
    EHeroAbility* __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        new (__cur) EHeroAbility(__x);
    return __cur;
}
#pragma auto_inline(on)

__forceinline EHeroAbility* __copy_backward_ptrs(EHeroAbility* __first, EHeroAbility* __last, EHeroAbility* __result,
                                                 const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Tp>
__forceinline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                       // 0x005F9495
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

template void vector<EHeroAbility>::_M_fill_insert(EHeroAbility*, unsigned int, const EHeroAbility&);

}