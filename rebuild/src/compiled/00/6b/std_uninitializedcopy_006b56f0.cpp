// std::__uninitialized_copy(const NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight* first, const NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight* last, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight* result, const __false_type&) -- copy-construct a range of 12-byte elements into raw storage
// Leaf: ??$__uninitialized_copy@PBUCThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B56F0, 56 bytes, `__fastcall` (/Gr: first in ecx, last in edx), `ret 8`.
// Identity (proven): reloc_bindings ??$__uninitialized_copy@PBUCThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@PAU123@@std@@...; callers the vector copy ctor 0x006B58F0, CEnvironmentThemeSetDay::BuildDay, vector::operator= 0x00888EE0.
// Shape (family K113, representative 0x004C2AD0): STLport 4.6 __uninitialized_copy(..., const __false_type&): the null-tested
//   placement copy (`test eax,eax ; je`), the trivially copied element as three dword moves.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {
public:
    struct CThemeWeight {                                // Ego_r.pdb 12 bytes
        long ThemeID;                                    // +0x00
        float Weight;                                    // +0x04
        bool LockWeight;                                 // +0x08
    };
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
template NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight* __fastcall __uninitialized_copy<const NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*>(const NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, const NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, const __false_type&);
}