// std::vector<NInventory::CItem>::_M_allocate_and_copy(unsigned int __n, const NInventory::CItem* __first, const NInventory::CItem* __last) -- a fresh block of n elements holding copies of [first, last)
// Leaf: ??$_M_allocate_and_copy@PBVCItem@NInventory@@@?$vector@VCItem@NInventory@@V?$allocator@VCItem@NInventory@@@std@@@std@@IAEPAVCItem@NInventory@@IPBV23@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C144A, 42 bytes, thiscall, `ret 0Ch`, ebp frame.
// Identity (proven): its only caller is vector<NInventory::CItem>::operator= 0x005C1F42 (the const source range of the other vector); the const-source __uninitialized_copy twin 0x005C10CC; allocate 0x005C0436 as for the reserve instance 0x005C10F7.
// Callees: _STLP_alloc_proxy<T*, T, allocator<T> >::allocate(unsigned int, const void*) 0x005C0436, std::__uninitialized_copy
//   0x005C10CC (`__fastcall`).
// Shape (family M141, representative 0x0057F8A7): STLport 4.6 `pointer __result = _M_end_of_storage.allocate(__n);
//   __uninitialized_copy(__first, __last, __result, _IsPODType()); return __result;` (no /EHsc: the try/unwind vanish) --
//   `optimize("y", off)` + `optimize("s", on)`; the tag temporary sits in the top byte of the dead n slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NInventory { class CItem { public: unsigned long Members[5]; }; }
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value allocate(unsigned int __n, const void* = 0);
    _Value _M_data;
};
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef __false_type _IsPODType;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
protected:
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = _M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
};
}
typedef std::vector<NInventory::CItem> TVec;
template TVec::pointer TVec::_M_allocate_and_copy<const NInventory::CItem*>(unsigned int, const NInventory::CItem*, const NInventory::CItem*);