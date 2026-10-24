// std::vector<CContextSensitiveItem>::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, unsigned int fill_len, bool atend) -- grow the context-sensitive item list and insert fill_len copies of x
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x00593F39, 191 bytes, `ret 14h`.
// Element: CTCInventory::CContextSensitiveItem (12 bytes, PDB {ThingDefIndex, ButtonIndex, Priority}); the only
// caller is push_back @ 0x00594782 (this chunk), called by the landed CTCInventory::AddContextSensitivePreferredItem
// 0x0058502D on the static list @ 0x013B8B40.
// Callees: allocator::allocate(n, hint) @ 0x00592260 (out of line here: thiscall on the `_M_end_of_storage` proxy,
// malloc(n * 12) or null), __uninitialized_copy(first, last, result, const __false_type&) @ 0x005924D0 and
// __uninitialized_fill_n(first, n, x, const __false_type&) @ 0x005924F4 (this chunk; both `/Gr` `__fastcall`),
// free @ 0x00BFEA14 (the inline `_M_clear` deallocate).
// Label: the catalog "Vector_ReallocateForUint" is descriptive: the body is STLport 4.x `_M_insert_overflow` for a
// class element, the same template as the CTCHeroStats-TU vector<CHousePosition> one @ 0x00580A7B: len = old_size +
// (max)(old_size, fill_len) through the const-reference max, copy the head, `_Copy_Construct` one element (the
// placement null test around three `movsd`, with esi/edi saved in the frame) or fill_n several, copy the tail unless
// `atend`, release the old block and `_M_set`. Every empty tag lives in the high byte of the `atend` slot
// (`lea ecx,[ebp+1Bh]`), which needs the helpers' bodies visible in the TU (same-TU stand-ins below).
// Shape: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
#include <new>

extern "C" void __cdecl free(void* block);          // 0x00BFEA14

struct CContextSensitiveItem {                       // CTCInventory::CContextSensitiveItem, 12 bytes (Ego_r.pdb)
    long ThingDefIndex;                              // +0x00
    long ButtonIndex;                                // +0x04
    long Priority;                                   // +0x08
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> __forceinline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

// Same-TU stand-ins for the out-of-line helpers (the bodies of 0x005924D0 / 0x005924F4): with them visible VC7.1
// homes every empty tag over the dead `atend` byte, as retail.
#pragma auto_inline(off)
CContextSensitiveItem* __fastcall __uninitialized_copy(const CContextSensitiveItem* __first,
                                                       const CContextSensitiveItem* __last,
                                                       CContextSensitiveItem* __result, const __false_type&)   // 0x005924D0
{
    CContextSensitiveItem* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Copy_Construct(__cur, *__first);
    return __cur;
}
CContextSensitiveItem* __fastcall __uninitialized_fill_n(CContextSensitiveItem* __first, size_t __n,
                                                         const CContextSensitiveItem& __x, const __false_type&)   // 0x005924F4
{
    CContextSensitiveItem* __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Copy_Construct(__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp> struct allocator {
    _Tp* allocate(size_t __n, const void* __hint = 0);                      // 0x00592260
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

template void vector<CContextSensitiveItem>::_M_insert_overflow(CContextSensitiveItem*, const CContextSensitiveItem&,
                                                                const __false_type&, size_t, bool);

}