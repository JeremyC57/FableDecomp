// std::__uninitialized_copy(CHeightMapCell* first, CHeightMapCell* last, CHeightMapCell* result, const __false_type&) -- copy-construct [first, last) into raw storage at result, return the end
// Leaf: ??$__uninitialized_copy@PAVCHeightMapCell@@PAV1@@std@@YIPAVCHeightMapCell@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008f67c0, 56 bytes, /Gr `__fastcall` (first ecx, last edx, result and the tag on the stack), `ret 8`.
// Identity (probable): its callers are the vector helpers 0x008F6950 / 0x008F6A90 in the CHeightMap TU; 0x008F6A90's only caller is CHeightMap::Resize 0x008F61A0 (Ego_r.pdb ?Resize@CHeightMap@@AAEXJJJJ@Z), whose only vector member is Cells (vector<CHeightMapCell>, 12-byte cells); the catalog's CTCSmokeGenerator::CGenerationPos label for 0x008F6A90 is propagated (CGenerationPos is 20 bytes).
// Body: STLport 4.6 `for (; first != last; ++first, ++cur) _Construct(&*cur, *first); return cur;` -- the element's implicit copy
//   constructor is a three-dword block copy (ebx through esi/edi), the placement new null-tests the destination.
// Shape (family L079).
class CHeightMapCell {                                     // Ego_r.pdb, 12 bytes
public:
    float         Height;                                 // +0x00
    unsigned char EngineThemePaletteIndex[3];             // +0x04
    unsigned char EngineBlend[2];                         // +0x07
    unsigned char Changed : 1;                            // +0x09
    unsigned char Passable : 1;
    unsigned char ShorePoint : 1;
    unsigned char CameraPassable : 1;
    unsigned char NavPreferred : 1;
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
template CHeightMapCell* __fastcall __uninitialized_copy(CHeightMapCell*, CHeightMapCell*, CHeightMapCell*, const __false_type&);
}   // namespace std