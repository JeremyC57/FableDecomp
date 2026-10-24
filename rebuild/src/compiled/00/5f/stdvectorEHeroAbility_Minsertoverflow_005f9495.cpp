// std::vector<EHeroAbility>::_M_insert_overflow(pointer position, const EHeroAbility& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport grow-and-insert for a 4-byte enum vector: allocate max(2*size, size+fill_len) slots, copy the prefix, place fill_len copies of x, copy the suffix unless appending, free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F9495, 180 bytes, `ret 14h`; callers: _M_fill_insert 0x005F9633 (this
//   chunk) and push_back 0x005F9BCD (landed callers declare it as vector<EHeroAbility>::push_back).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the inline allocate / deallocate), __uninitialized_copy(first, last,
//   result, const __false_type&) 0x005F79BD (landed, `/Gr` `__fastcall`).
// Label correction: the catalog `Vector_CopyAndReallocate_UnsignedInt` is behavioural; the body is STLport 4.x
//   `_M_insert_overflow` (the landed 0x005B3275 template) for an element STLport does not treat as POD (an enum: the
//   placement null tests and the __false_type helper), with `__uninitialized_fill_n` and `_M_clear` inlined (the element's
//   destructor is trivial, so `_M_clear` is only the deallocate's `if (p) free(p)`).
// Shape: frame-pointer size TU (`#pragma optimize("y", off)` + `("s", on)`); the empty tags are homed in the high byte of
//   the `atend` slot (`lea eax,[ebp+1Bh]`) thanks to the same-TU `__uninitialized_copy` body; `max` works on references
//   (`lea ecx,[ebp+14h]` / `lea ecx,[ebp-4]`), the old size's frame word is reused for the new block.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
#include <new>

extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* p);              // 0x00BFEA14

enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };

namespace std {

struct __false_type { __false_type() { } };

// Same-TU stand-in for the out-of-line helper (its STLport body, tag unnamed and untouched): with it visible VC7.1
// homes every empty tag over the dead `atend` byte, as retail.
#pragma auto_inline(off)
EHeroAbility* __fastcall __uninitialized_copy(const EHeroAbility* __first, const EHeroAbility* __last,
                                              EHeroAbility* __result, const __false_type&)            // 0x005F79BD
{
    EHeroAbility* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) EHeroAbility(*__first);
    return __cur;
}
#pragma auto_inline(on)

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        new (__cur) _Tp(__x);
    return __cur;
}

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> __forceinline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct allocator {
    __forceinline _Tp* allocate(size_t __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    __forceinline void deallocate(_Tp* __p, size_t) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp*                                                _M_start;             // +0x00
    _Tp*                                                _M_finish;            // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> >       _M_end_of_storage;    // +0x08
    size_t size() const { return size_t(_M_finish - _M_start); }
    __forceinline void _M_clear() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                            bool __atend = false);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                                     bool __atend)
{
    const size_t __old_size = size();
    const size_t __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = _M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<EHeroAbility>::_M_insert_overflow(EHeroAbility*, const EHeroAbility&, const __false_type&, size_t, bool);

}