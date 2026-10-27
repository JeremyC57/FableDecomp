// std::__uninitialized_copy(CEngineLightingManager::CThemeChannel* first, CEngineLightingManager::CThemeChannel* last, CEngineLightingManager::CThemeChannel* result, const __false_type&) -- STLport placement copy-construct each 112-byte record (the implicit copy as rep movsd) and return the advanced result
// Leaf: ??$__uninitialized_copy@PAUCThemeChannel@CEngineLightingManager@@PAU12@@std@@YIPAUCThemeChannel@CEngineLightingManager@@PAU12@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b49760, 47 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (probable): callers the fill insert 0x00B4A2C0 (twice), the two-argument resize 0x00B4A4C0 that CEngineLightingManager::FrameStart 0x00B484B0 applies to Ego_r.pdb CEngineLightingManager::ThemeChannels (CArray<CThemeChannel>); 112-byte stride = sizeof(CThemeChannel).
// Callees: none.
// Shape (family L021): STLport 4.6 `__uninitialized_copy(first, last, result, const __false_type&)` verbatim (every user
//   class is non-POD to STLport); the record's implicit copy constructor is `rep movsd` of 28 dwords under the null test.
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
class CEngineLightingManager {
public:
    struct CThemeChannel {                                 // Ego_r.pdb (struct), 112 bytes (... FogEndZ, ShadowFactor, FogTransform; implicit copy)
        long Data[28];
    };
};
template CEngineLightingManager::CThemeChannel* __fastcall std::__uninitialized_copy<CEngineLightingManager::CThemeChannel*, CEngineLightingManager::CThemeChannel*>(CEngineLightingManager::CThemeChannel*, CEngineLightingManager::CThemeChannel*, CEngineLightingManager::CThemeChannel*, const std::__false_type&);