// std::__uninitialized_copy(CGraphicInfo* first, CGraphicInfo* last, CGraphicInfo* result, const __false_type&) -- STLport placement copy-construct each 44-byte record (the implicit copy as rep movsd) and return the advanced result
// Leaf: ??$__uninitialized_copy@PAVCGraphicInfo@@PAV1@@std@@YIPAVCGraphicInfo@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009ffeb0, 47 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (proven): bound ??$__uninitialized_copy@PAVCGraphicInfo@@PAV1@@std@@...; callers the landed std::vector<CGraphicInfo>::_M_fill_insert 0x00A01290 (twice).
// Callees: none.
// Shape (family L021): STLport 4.6 `__uninitialized_copy(first, last, result, const __false_type&)` verbatim (every user
//   class is non-POD to STLport); the record's implicit copy constructor is `rep movsd` of 11 dwords under the null test.
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
class CGraphicInfo {                                      // Ego_r.pdb, 44 bytes (implicit copy)
public:
    long Data[11];
};
template CGraphicInfo* __fastcall std::__uninitialized_copy<CGraphicInfo*, CGraphicInfo*>(CGraphicInfo*, CGraphicInfo*, CGraphicInfo*, const std::__false_type&);