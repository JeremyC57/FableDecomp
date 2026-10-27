// std::__uninitialized_copy(CTCInventoryMap::CUIRegionInfo* first, CTCInventoryMap::CUIRegionInfo* last, CTCInventoryMap::CUIRegionInfo* result, const __false_type&) -- STLport placement copy-construct each 40-byte record (the implicit copy as rep movsd) and return the advanced result
// Leaf: ??$__uninitialized_copy@PAVCUIRegionInfo@CTCInventoryMap@@PAV12@@std@@YIPAVCUIRegionInfo@CTCInventoryMap@@PAV12@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006127d0, 47 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (proven): bound ??$__uninitialized_copy@PAVCUIRegionInfo@CTCInventoryMap@@PAV12@@std@@...; callers the landed vector<CTCInventoryMap::CUIRegionInfo>::_M_fill_insert 0x00618990 (twice); 40-byte stride = Ego_r.pdb sizeof(CUIRegionInfo).
// Callees: none.
// Shape (family L021): STLport 4.6 `__uninitialized_copy(first, last, result, const __false_type&)` verbatim (every user
//   class is non-POD to STLport); the record's implicit copy constructor is `rep movsd` of 10 dwords under the null test.
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}
class CTCInventoryMap {
public:
    class CUIRegionInfo {                                  // Ego_r.pdb, 40 bytes (Region, Shops, BarberShops, ...; implicit copy)
    public:
        long Data[10];
    };
};
template CTCInventoryMap::CUIRegionInfo* __fastcall std::__uninitialized_copy<CTCInventoryMap::CUIRegionInfo*, CTCInventoryMap::CUIRegionInfo*>(CTCInventoryMap::CUIRegionInfo*, CTCInventoryMap::CUIRegionInfo*, CTCInventoryMap::CUIRegionInfo*, const std::__false_type&);