// std::vector::_M_fill_insert(CTCInventoryMap::CUIRegionInfo* position, unsigned int n, const CUIRegionInfo& x) -- STLport insert of n copies of a 40-byte region info record: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00618990, 320 bytes (314 without padding), thiscall `ret 0Ch`. Callers: the
//   insert(pos, n, x) jmp thunk 0x00618F70 and resize(n, x) 0x006190C0 (`call` at 0x00619119); the overflow helper
//   0x00618420 is also called by CTCInventoryMap::ConstructRegionDescription, so the 40-byte element is the PDB's
//   CTCInventoryMap::CUIRegionInfo (UIRegionsInfo, ten longs).
// Callees (`__fastcall`, COMDAT-folded with other 40-byte types, hence their propagated labels):
//   __uninitialized_copy(.., const __false_type&) 0x006127D0, __copy_backward_ptrs(.., const __false_type&) 0x00614970,
//   fill 0x006131E0, uninitialized_fill_n(first, n, x) 0x00613210, _M_insert_overflow 0x00618420 (atend false: `push 0`).
// Label correction: the catalog "Vector_InsertRange_40byte" -- this is STLport 4.6 `_M_fill_insert`.
// Shape: STLport 4.6 `_M_fill_insert` verbatim; x_copy is the implicit memberwise copy (`rep movsd` x10 into the frame);
//   in this TU __copy_backward_ptrs and uninitialized_fill_n stay out of line (same-TU noinline bodies, auto_inline off),
//   so the empty tags and `old_finish - n` / `n - elems_after` live in the dead n argument slot as retail does.
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

class CTCInventoryMap {
public:
    class CUIRegionInfo {
    public:
        long Region;                                                             // +0x00
        long Shops;                                                              // +0x04
        long BarberShops;                                                        // +0x08
        long TitleShops;                                                         // +0x0C
        long TatooShops;                                                         // +0x10
        long ShopsForSale;                                                       // +0x14
        long HousesForSale;                                                      // +0x18
        long ShopsOwned;                                                         // +0x1C
        long HousesOwned;                                                        // +0x20
        long Taverns;                                                            // +0x24
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                 // 0x006127D0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp>
__declspec(noinline) _Tp* __fastcall __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{                                                                                 // 0x00614970
    for (int __n = int(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                 // 0x00613210
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x006131E0
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x00618420
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<CTCInventoryMap::CUIRegionInfo>::_M_fill_insert(CTCInventoryMap::CUIRegionInfo*, unsigned int, const CTCInventoryMap::CUIRegionInfo&);
}