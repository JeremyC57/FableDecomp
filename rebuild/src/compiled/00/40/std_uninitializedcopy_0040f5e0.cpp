// std::__uninitialized_copy(const CDisplayModeDesc* first, const CDisplayModeDesc* last, CDisplayModeDesc* result, const __false_type&) -- copy-construct [first, last) into raw slots at result, return the end
// Leaf: ??$__uninitialized_copy@PBVCDisplayModeDesc@@PAV1@@std@@YIPAVCDisplayModeDesc@@PBV1@0PAV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040F5E0, 56 bytes, `__fastcall` (first ecx, last edx, result and the tag on the stack), `ret 8`.
// Identity (proven): the catalog binding of 0x0040F5E0 is this symbol (recorded by its caller 0x004124C0 (vector<CDisplayModeDesc>::_M_insert_overflow, landed)).
// Callees: none (placement copy inlined behind its null test).
// Shape (family M010, representative 0x0040F5E0): STLport 4.6 `__uninitialized_copy(__false_type)` -- `cur = result`, the
//   first == last early test, `_Construct(&*cur, *first)` (null test, four dword moves through esi: the memberwise copy),
//   `++first, ++cur`; returns cur in eax.
inline void* operator new(unsigned int, void* p) { return p; }
class C2DExtentsI {                                       // Ego_r.pdb, 8 bytes
public:
    long X;
    long Y;
};
class CDisplayModeDesc {                                  // Ego_r.pdb, 16 bytes
public:
    C2DExtentsI Dimensions;                               // +0x0
    long        BitsPerPixel;                             // +0x8
    long        RefreshRate;                              // +0xC
    CDisplayModeDesc(const CDisplayModeDesc& other)       // PDB ??0CDisplayModeDesc@@QAE@ABV0@@Z: memberwise
        : Dimensions(other.Dimensions), BitsPerPixel(other.BitsPerPixel), RefreshRate(other.RefreshRate) { }
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
template CDisplayModeDesc* __fastcall __uninitialized_copy<const CDisplayModeDesc*, CDisplayModeDesc*>(const CDisplayModeDesc*, const CDisplayModeDesc*, CDisplayModeDesc*, const __false_type&);
}