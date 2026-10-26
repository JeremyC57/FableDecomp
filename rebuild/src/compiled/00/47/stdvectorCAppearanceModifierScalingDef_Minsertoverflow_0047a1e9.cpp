// std::vector<CAppearanceModifierScalingDef>::_M_insert_overflow(CAppearanceModifierScalingDef* position, const CAppearanceModifierScalingDef& x, const __false_type&, size_t fill_len, bool atend) -- STLport grow-and-insert: allocate size + max(size, n), copy the head, insert n copies of x, copy the tail, release the old storage
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047A1E9, 230 bytes, `ret 14h`.
// Callees: malloc @ 0x00BFEA0E, CAppearanceModifierScalingDef::CAppearanceModifierScalingDef(const CAppearanceModifierScalingDef&) @ 0x004784B3
// (landed; every placement copy), vector::_M_clear() @ 0x00478469 (destroy + free the old block, out of line).
// Shape: STLport 4.x `_M_insert_overflow` for a non-POD element, verbatim: `__len = __old_size + (max)(__old_size,
// __fill_len)` through the const-reference `max` (`a < b ? b : a`: the old size is stored to a frame word for its
// address), allocate, `__uninitialized_copy(_M_start, position, new_start, __false_type())`, then `if (__fill_len
// == 1) { _Construct(new_finish, x); ++new_finish; } else new_finish = __uninitialized_fill_n(new_finish,
// fill_len, x, __false_type());`, the tail copy unless `atend` (the old finish is parked in the dead `atend`
// slot), `_M_clear()` and `_M_set(start, finish, start + len)`. Placement new gives each copy its null test; the
// 0x48 element size is `lea [n+n*8] ; shl 3` / `lea [p+n*8]`. Frame-pointer size TU (`optimize("y", off)` +
// `("s", on)`), the inline STLport helpers `__forceinline`.
// Byte-identical to the CAbilityTrackLevelDef instantiation 0x00478DD6 apart from the two callees.
// Label correction: the catalog calls this row "CAbilityTrackLevelDef_vector_Reallocate" too (propagated across
// the identical body); its element copy 0x004784B3 is the landed CAppearanceModifierScalingDef copy constructor
// and its _M_clear 0x00478469 sits in that def's TU (callers 0x0047A1DE and a reference at 0x00CAC41A).
// Leaf: _M_insert_overflow
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <new>

extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E

class CAppearanceModifierScalingDef { public:
    unsigned char _body[0x48];
    CAppearanceModifierScalingDef(const CAppearanceModifierScalingDef& other);                  // 0x004784B3
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                  const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear();                                                            // 0x00478469
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                            unsigned int __fill_len, bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = _M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template class vector<CAppearanceModifierScalingDef>;

}