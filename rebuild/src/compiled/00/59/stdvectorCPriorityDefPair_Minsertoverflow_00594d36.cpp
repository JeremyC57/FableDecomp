// std::vector<CPriorityDefPair>::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, unsigned int fill_len, bool atend) -- grow the (priority, item def) pair vector and insert fill_len copies of x
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594D36, 172 bytes, `ret 14h`.
// Element: std::pair<long, CDefPointer<const CContextSensitiveItemDef> > (8 bytes; the Ego_r.pdb sort/heap
// instantiations' element) with out-of-line copy ctor 0x00592DFB and destructor 0x005930F3. Only caller: its
// push_back @ 0x00594E15 (called at 0x00591C6E / 0x00591CFE in the CTCInventory TU).
// Callees: malloc @ 0x00BFEA0E (the inline allocate), __uninitialized_copy(first, last, result, const __false_type&)
// @ 0x00593800 and __uninitialized_fill_n(first, n, x, const __false_type&) @ 0x0059382B (both `/Gr` `__fastcall`),
// vector::_M_clear() @ 0x00594C10 (out of line here: `_Destroy(_M_start, _M_finish)` through 0x00594697, then free).
// Label: the catalog "Vector_Reallocate_PairAbilityDef" is propagated (0x00592DFB's manifest label names a
// CHeroAbilityDef pair, an ICF-folded twin): the body is STLport 4.x `_M_insert_overflow` for a class element, the same
// template as 0x00594880 (this chunk): len = old_size + (max)(old_size, fill_len) through the const-reference max,
// new_start in the old-size frame word, copy the head, `_Copy_Construct` one element (the placement null test around
// the copy-ctor call) or fill_n several, copy the tail unless `atend`, `_M_clear()` (the destructor is non-trivial,
// so it stays a call) and `_M_set`. Every empty tag lives in the high byte of the `atend` slot (`lea eax,[ebp+1Bh]`),
// which needs the helpers' bodies visible (same-TU stand-ins below).
// Shape: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
#include <new>

extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E

class CContextSensitiveItemDef;

template <class T> class CDefPointer {
public:
    T* PDef;                                         // the reference-counted def (released by ~pair 0x005930F3)
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& other);                         // 0x00592DFB
    ~pair();                                         // 0x005930F3
};
}

typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > CPriorityDefPair;

namespace std {

struct __false_type { __false_type() { } };

// Same-TU stand-ins for the out-of-line helpers (their STLport bodies, tags unnamed and untouched): with them
// visible VC7.1 homes every empty tag over the dead `atend` byte, as retail.
#pragma auto_inline(off)
CPriorityDefPair* __fastcall __uninitialized_copy(const CPriorityDefPair* __first, const CPriorityDefPair* __last,
                                                  CPriorityDefPair* __result, const __false_type&)     // 0x00593800
{
    CPriorityDefPair* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) CPriorityDefPair(*__first);
    return __cur;
}
CPriorityDefPair* __fastcall __uninitialized_fill_n(CPriorityDefPair* __first, size_t __n, const CPriorityDefPair& __x,
                                                    const __false_type&)                               // 0x0059382B
{
    for (; __n > 0; --__n, ++__first)
        new (__first) CPriorityDefPair(__x);
    return __first;
}
#pragma auto_inline(on)

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> __forceinline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct allocator {
    __forceinline _Tp* allocate(size_t __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
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
    void _M_clear();                                                          // 0x00594C10
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

template void vector<CPriorityDefPair>::_M_insert_overflow(CPriorityDefPair*, const CPriorityDefPair&, const __false_type&,
                                                           size_t, bool);

}