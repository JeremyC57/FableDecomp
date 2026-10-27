// std::__uninitialized_copy(CHeightMapCell* first, CHeightMapCell* last, CHeightMapCell* result, const __false_type&) -- copy-construct a range of 12-byte elements into raw storage
// Leaf: ??$__uninitialized_copy@PAVCHeightMapCell@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F6800, 56 bytes, `__fastcall` (/Gr: first in ecx, last in edx), `ret 8`.
// Identity (proven): reloc_bindings ??$__uninitialized_copy@PAVCHeightMapCell@@PAV1@@std@@YIPAVCHeightMapCell@@PAV1@00ABU__false_type@0@@Z; callers the vector's _M_fill_insert 0x008F6C90 (landed) and 0x008F6BB0.
// Shape (family K113, representative 0x004C2AD0): STLport 4.6 __uninitialized_copy(..., const __false_type&): the null-tested
//   placement copy (`test eax,eax ; je`), the trivially copied element as three dword moves.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
class CHeightMapCell {                                   // Ego_r.pdb 12 bytes
public:
    float Height;                                        // +0x00
    unsigned char EngineThemePaletteIndex[3];            // +0x04
    unsigned char EngineBlend[2];                        // +0x07
    unsigned char Changed : 1;                           // +0x09
    unsigned char Passable : 1;
    unsigned char ShorePoint : 1;
    unsigned char CameraPassable : 1;
    unsigned char NavPreferred : 1;
};
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
template CHeightMapCell* __fastcall __uninitialized_copy<CHeightMapCell*, CHeightMapCell*>(CHeightMapCell*, CHeightMapCell*, CHeightMapCell*, const __false_type&);
}