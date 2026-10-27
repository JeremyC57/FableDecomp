// std::__uninitialized_copy(CVertexGroupBlockOptimiser::CBlockInfo* first, CVertexGroupBlockOptimiser::CBlockInfo* last, CVertexGroupBlockOptimiser::CBlockInfo* result, const __false_type&) -- copy-construct [first, last) into raw storage at result, return the end
// Leaf: ??$__uninitialized_copy@PAVCBlockInfo@CVertexGroupBlockOptimiser@@PAV12@@std@@YIPAVCBlockInfo@CVertexGroupBlockOptimiser@@PAV12@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ada7e0, 56 bytes, /Gr `__fastcall` (first ecx, last edx, result and the tag on the stack), `ret 8`.
// Identity (proven): callers the landed vector<CVertexGroupBlockOptimiser::CBlockInfo>::reserve 0x00ADB960 and that vector's grow insert 0x00ADBD50 (head and tail copies); one retail instantiation serves both, so its iterator is the non-const CBlockInfo* (_M_insert_overflow passes _M_start / position) -- the landed reserve's binding ??$__uninitialized_copy@PBVCBlockInfo@CVertexGroupBlockOptimiser@@... (const first) is a second spelling for this address.
// Body: STLport 4.6 `for (; first != last; ++first, ++cur) _Construct(&*cur, *first); return cur;` -- the element's implicit copy
//   constructor is a three-dword block copy (ebx through esi/edi), the placement new null-tests the destination.
// Shape (family L079).
class CVertexGroupBlockOptimiser {
public:
    class CBlockInfo {                                      // Ego_r.pdb, 12 bytes
    public:
        unsigned long BlockIndex;                           // +0x00
        long          Cost;                                 // +0x04
        long          Benefit;                              // +0x08
    };
};
inline void* operator new(unsigned int, void* __p) { return __p; }
namespace std {
struct __false_type { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new ((void*)__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template CVertexGroupBlockOptimiser::CBlockInfo* __fastcall __uninitialized_copy(CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*, const __false_type&);
}   // namespace std