// _STL::__uninitialized_copy(CEnvironmentThemePriorityStackEntry* first, CEnvironmentThemePriorityStackEntry* last, CEnvironmentThemePriorityStackEntry* result, const __false_type&) -- copy-construct the theme priority-stack entries of [first, last) into raw storage at result, return the end
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00889300, length 90 bytes, `ret 8`.
// Identity: the catalog label `_Move<CMusicEntry*,CMusicEntry*>` is propagated from a byte twin. The 48-byte element is
//   NEnvironmentInternal::CEnvironmentThemePriorityStackEntry, as in the landed same-TU __uninitialized_fill_n 0x00889230
//   and fill 0x008895F0 (Ego_r.pdb names, retail offsets). Callers: the vector regrowth/insert paths at 0x00889B89,
//   0x00889C06 and the landed _M_fill_insert 0x00889DA0 (0x00889E1F, 0x00889E79).
// Callees: vector<CThemeWeight>::vector(const vector&) @ 0x006B58F0 (the Themes member's copy constructor).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` in a /Gr TU (`__fastcall`: first
//   in ecx -> esi, last in edx -> ebx, result and the tag reference on the stack, result -> edi as `cur`). The implicit
//   memberwise copy constructor (float/bool through integer registers, the vector copy on +8, two scalars and a bool,
//   the double through `fld/fstp qword`, the last float) behind the placement-new null test; stride 0x30.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct CThemeWeight {                           // 12 bytes
    int   ThemeID;
    float Weight;
    bool  LockWeight;
};

struct CThemeWeightVector {                     // retail 12-byte STLport vector<CThemeWeight>
    CThemeWeight* _M_start;
    CThemeWeight* _M_finish;
    CThemeWeight* _M_end_of_storage;
    CThemeWeightVector(const CThemeWeightVector& other);   // 0x006B58F0
};

struct CEnvironmentThemePriorityStackEntry {    // 48 bytes (retail)
    float              BaseWeight;                     // +0x00
    bool               BaseWeightLocked;               // +0x04
    CThemeWeightVector Themes;                         // +0x08
    int                ThemeTransitionTargetID;        // +0x14
    float              ThemeTransitionInitialWeight;   // +0x18
    bool               ThemeTransitionActive;          // +0x1C
    double             ThemeTransitionStartTime;       // +0x20
    float              ThemeTransitionDuration;        // +0x28
};

namespace _STL {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template CEnvironmentThemePriorityStackEntry* __fastcall __uninitialized_copy<CEnvironmentThemePriorityStackEntry*, CEnvironmentThemePriorityStackEntry*>(
    CEnvironmentThemePriorityStackEntry*, CEnvironmentThemePriorityStackEntry*, CEnvironmentThemePriorityStackEntry*, const __false_type&);
}