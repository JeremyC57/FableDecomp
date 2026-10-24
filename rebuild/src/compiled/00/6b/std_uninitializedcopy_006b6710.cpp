// std::__uninitialized_copy(CEnvironmentThemePriorityStackEntry* first, CEnvironmentThemePriorityStackEntry* last, CEnvironmentThemePriorityStackEntry* result, const __false_type&) -- placement-copy a range of environment theme priority-stack entries, return the advanced result
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6710, 90 bytes, `ret 8`.
// Callers: one site, 0x006B689A (the vector<CEnvironmentThemePriorityStackEntry> growth helper).
// Callees: CArray<CThemeWeight>::CArray(const CArray&) @ 0x006B58F0 (the STLport vector copy constructor).
// Types (Ego_r.pdb NEnvironmentInternal::CEnvironmentThemePriorityStackEntry, 56 bytes with a 16-byte CArray -> retail
//   0x30): BaseWeight +0, BaseWeightLocked +4, Themes +8, ThemeTransitionTargetID +0x14, ThemeTransitionInitialWeight
//   +0x18, ThemeTransitionActive +0x1C, ThemeTransitionStartTime (double) +0x20, ThemeTransitionDuration +0x28 -- the
//   same model as the landed copy constructor 0x006B62B0 and _Construct 0x006B6670.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
//   in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `cur = result` loaded before the
//   empty test, a placement-new null test per element and the entry's IMPLICIT memberwise copy constructor inlined
//   (words through eax/ecx, the bools as bytes, the vector through its out-of-line copy constructor, the double through
//   the FPU).
// Label correction: the catalog `_Move<CMusicEntry*,CMusicEntry*>` is propagated; `ret 8` + the placement null test name
//   __uninitialized_copy and the 0x30 stride + the CArray copy call name the element.
inline void* operator new(unsigned int, void* p) { return p; }

struct __false_type { __false_type() { } };

namespace NEnvironmentInternal {
struct CThemeWeight;
struct CThemeWeightArray {                                  // retail CArray<CThemeWeight>: STLport vector, 12 bytes
    CThemeWeight* _M_start;
    CThemeWeight* _M_finish;
    CThemeWeight* _M_end_of_storage;
    CThemeWeightArray(const CThemeWeightArray& other);      // 0x006B58F0
};

struct CEnvironmentThemePriorityStackEntry {                // 0x30 bytes in retail
    float             BaseWeight;                           // +0x00
    bool              BaseWeightLocked;                     // +0x04
    CThemeWeightArray Themes;                               // +0x08
    long              ThemeTransitionTargetID;              // +0x14
    float             ThemeTransitionInitialWeight;         // +0x18
    bool              ThemeTransitionActive;                // +0x1C
    double            ThemeTransitionStartTime;             // +0x20
    float             ThemeTransitionDuration;              // +0x28
};
}

namespace std {
template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template NEnvironmentInternal::CEnvironmentThemePriorityStackEntry* __fastcall
__uninitialized_copy<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*>(
    NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*,
    NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*, const __false_type&);
}