// std::vector<CContextSensitiveItem>::push_back(const value_type& x) -- append one context-sensitive item record, growing the storage when full
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594782, 52 bytes, `ret 4` (thiscall).
// Element: CTCInventory::CContextSensitiveItem (12 bytes, PDB {ThingDefIndex, ButtonIndex, Priority}); the only
// caller is the landed CTCInventory::AddContextSensitivePreferredItem 0x0058502D on the static list @ 0x013B8B40.
// Callee: _M_insert_overflow(position, x, const __false_type&, fill_len, atend) @ 0x00593F39 (this chunk).
// Label: the catalog "Vector_PushBack_Uint" is a propagated guess: the body is STLport 4.x `vector::push_back`
// verbatim: `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x); ++_M_finish; } else
// _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- the placement null test around three `movsd`, the
// `__false_type` temporary homed in the high byte of the dead `x` slot (`lea eax,[ebp+0Bh]`).
// Shape: `_Construct` must be `__forceinline` (plain `inline` stays an out-of-line call in this size TU).
// Frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
#include <new>

struct CContextSensitiveItem {                       // CTCInventory::CContextSensitiveItem, 12 bytes (Ego_r.pdb)
    long ThingDefIndex;                              // +0x00
    long ButtonIndex;                                // +0x04
    long Priority;                                   // +0x08
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct allocator {
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    typedef __false_type _IsPODType;
    _Tp*                                                _M_start;             // +0x00
    _Tp*                                                _M_finish;            // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> >       _M_end_of_storage;    // +0x08
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                            bool __atend = false);                            // 0x00593F39
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, _IsPODType(), 1UL, true);
    }
};

template void vector<CContextSensitiveItem>::push_back(const CContextSensitiveItem&);

}