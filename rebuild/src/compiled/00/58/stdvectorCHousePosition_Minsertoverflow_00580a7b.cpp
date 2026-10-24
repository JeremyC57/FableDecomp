// std::vector<CHousePosition>::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, unsigned int fill_len, bool atend) -- grow the house-position vector and insert fill_len copies of x
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x00580A7B, 180 bytes, `ret 14h`.
// Element: std::pair<const CRegion*, C3DVector> (16 bytes; CTCHeroStats::HousePositions, retail +0x160): the only
// caller is its push_back @ 0x00581132 (called by the landed CTCHeroStats::AddHousePosition 0x0057CB4B).
// Callees: malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (the inline allocate / `_M_clear` deallocate),
// __uninitialized_copy(first, last, result, const __false_type&) @ 0x0057F658, the pair copy ctor @ 0x0057F092
// (manifest label "GFToScreen" is propagated; four dword copies), __uninitialized_fill_n(first, n, x, const
// __false_type&) @ 0x0057F683 (both `/Gr` `__fastcall`, landed as UninitCopy/UninitFillN_CElement16).
// Label: the catalog "Vector_ReallocateForCArray" is descriptive: the body is STLport 4.x `_M_insert_overflow` for a
// non-trivial element: len = old_size + (max)(old_size, fill_len) through the const-reference max (the `lea`
// select between the `fill_len` slot and the old-size frame word), new_start in that frame word, copy the head,
// `_Copy_Construct` one element (the placement null test) or fill_n several, copy the tail unless `atend`, release
// the old block (the pair has a trivial destructor) and `_M_set`. Every empty tag lives in the high byte of the
// `atend` slot (`lea eax,[ebp+1Bh]`).
// Shape: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
#include <new>

extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);          // 0x00BFEA14

class CRegion;

struct C3DVector {
    float X, Y, Z;
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o);                          // 0x0057F092
};

typedef pair<const CRegion*, C3DVector> CHousePosition;

// Same-TU stand-ins for the out-of-line helpers (their landed bodies' effect, tags unnamed and untouched): with
// them visible VC7.1 homes every empty tag over the dead `atend` byte, as retail.
#pragma auto_inline(off)
CHousePosition* __fastcall __uninitialized_copy(const CHousePosition* __first, const CHousePosition* __last,
                                                CHousePosition* __result, const __false_type&)        // 0x0057F658
{
    CHousePosition* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) CHousePosition(*__first);
    return __cur;
}
CHousePosition* __fastcall __uninitialized_fill_n(CHousePosition* __first, size_t __n, const CHousePosition& __x,
                                                  const __false_type&)                                // 0x0057F683
{
    for (; __n > 0; --__n, ++__first)
        new (__first) CHousePosition(__x);
    return __first;
}
#pragma auto_inline(on)

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

template void vector<CHousePosition>::_M_insert_overflow(CHousePosition*, const CHousePosition&, const __false_type&,
                                                         size_t, bool);

}