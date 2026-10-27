// std::__uninitialized_copy(const NGraphicAppearance::CMeshInfo* first, const NGraphicAppearance::CMeshInfo* last, NGraphicAppearance::CMeshInfo* result, const __false_type&) -- copy-construct a range of 12-byte elements into raw storage
// Leaf: ??$__uninitialized_copy@PBVCMeshInfo@NGraphicAppearance@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C2AD0, 56 bytes, `__fastcall` (/Gr: first in ecx, last in edx), `ret 8`.
// Identity (proven): reloc_bindings ?__uninitialized_copy@std@@YIPAVCMeshInfo@NGraphicAppearance@@PBV23@0PAV23@ABU__false_type@1@@Z; callers vector<NGraphicAppearance::CMeshInfo>::reserve 0x004C46C0 (landed).
// Shape (family K113, representative 0x004C2AD0): STLport 4.6 __uninitialized_copy(..., const __false_type&): the null-tested
//   placement copy (`test eax,eax ; je`), the trivially copied element as three dword moves.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
namespace NGraphicAppearance {
class CMeshInfo {                                       // Ego_r.pdb 12 bytes
public:
    long Index;                                          // +0x00
    float SizeX;                                         // +0x04
    unsigned char PrimaryMesh : 1;                       // +0x08 bit 0
};
}
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
template NGraphicAppearance::CMeshInfo* __fastcall __uninitialized_copy<const NGraphicAppearance::CMeshInfo*, NGraphicAppearance::CMeshInfo*>(const NGraphicAppearance::CMeshInfo*, const NGraphicAppearance::CMeshInfo*, NGraphicAppearance::CMeshInfo*, const __false_type&);
}