// std::vector<CHeroMarriageInfo>::_M_allocate_and_copy(unsigned int __n, CHeroMarriageInfo* __first, CHeroMarriageInfo* __last) -- a fresh block of n elements holding copies of [first, last)
// Leaf: ??$_M_allocate_and_copy@PAVCHeroMarriageInfo@@@?$vector@VCHeroMarriageInfo@@V?$allocator@VCHeroMarriageInfo@@@std@@@std@@IAEPAVCHeroMarriageInfo@@IPAV2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057F8A7, 42 bytes, thiscall, `ret 0Ch`, ebp frame.
// Identity (proven): its only caller is vector<CHeroMarriageInfo>::reserve 0x005806DF (mutable source range); callees bound ?allocate@?$_STLP_alloc_proxy@PAVCHeroMarriageInfo@@V1@V?$allocator@VCHeroMarriageInfo@@@std@@@std@@... and ??$__uninitialized_copy@PAVCHeroMarriageInfo@@PAV1@@std@@....
// Callees: _STLP_alloc_proxy<T*, T, allocator<T> >::allocate(unsigned int, const void*) 0x0057EFF0, std::__uninitialized_copy
//   0x0057F24E (`__fastcall`).
// Shape (family M141, representative 0x0057F8A7): STLport 4.6 `pointer __result = _M_end_of_storage.allocate(__n);
//   __uninitialized_copy(__first, __last, __result, _IsPODType()); return __result;` (no /EHsc: the try/unwind vanish) --
//   `optimize("y", off)` + `optimize("s", on)`; the tag temporary sits in the top byte of the dead n slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CHeroMarriageInfo { public: unsigned long Members[6]; };
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
typedef std::vector<CHeroMarriageInfo> TVec;
template TVec::pointer TVec::_M_allocate_and_copy<CHeroMarriageInfo*>(unsigned int, CHeroMarriageInfo*, CHeroMarriageInfo*);