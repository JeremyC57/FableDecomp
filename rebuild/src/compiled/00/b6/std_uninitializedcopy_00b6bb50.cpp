// std::__uninitialized_copy(CLandscapeTheme* first, CLandscapeTheme* last, CLandscapeTheme* result, const __false_type&) -- STLport placement copy-construct each 56-byte record (the implicit copy as rep movsd) and return the advanced result
// Leaf: ??$__uninitialized_copy@PAUCLandscapeTheme@@PAU1@@std@@YIPAUCLandscapeTheme@@PAU1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b6bb50, 47 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (proven): callers the fill insert 0x00B6C700 (twice), the two-argument resize 0x00B6CB20 that CEngineLandscapeRenderer::BuildThemes 0x00B687D0 applies to CEngineLandscapeRenderer::LandscapeThemes (CArray<CLandscapeTheme>) and that the landed std::vector<CLandscapeTheme>::resize(n) 0x00B6CC50 forwards to; 56-byte stride = sizeof(CLandscapeTheme).
// Callees: none.
// Shape (family L021): STLport 4.6 `__uninitialized_copy(first, last, result, const __false_type&)` verbatim (every user
//   class is non-POD to STLport); the record's implicit copy constructor is `rep movsd` of 14 dwords under the null test.
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
struct CLandscapeTheme {                                   // Ego_r.pdb (struct), 56 bytes (... WaterType, WaterHeight; implicit copy)
    long Data[14];
};
template CLandscapeTheme* __fastcall std::__uninitialized_copy<CLandscapeTheme*, CLandscapeTheme*>(CLandscapeTheme*, CLandscapeTheme*, CLandscapeTheme*, const std::__false_type&);