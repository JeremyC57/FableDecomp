// _global::UninitFillN_ThemeStackEntry_00889230(CEnvironmentThemePriorityStackEntry* first, unsigned int n, const CEnvironmentThemePriorityStackEntry& x, const __false_type&) -- STLport __uninitialized_fill_n<CEnvironmentThemePriorityStackEntry*>: copy-construct n theme priority-stack entries into raw storage, return first + n
// Leaf: UninitFillN_ThemeStackEntry_00889230
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00889230, length 86 bytes, `ret 8`.
// Callers: 0x0088936A (0x00889300), 0x00889BEA (0x00889B20) and 0x00889E58 in the landed vector<Element>::_M_fill_insert
//   0x00889DA0 (whose same-TU `__uninitialized_fill_n` stand-in is this body, element CEnvironmentThemePriorityStackEntry).
// Callees: vector<CThemeWeight>::vector(const vector&) @ 0x006B58F0 (catalog "std::vector<unsigned_int>::vector").
// Identity: the catalog label `_Uninit_fill_n<CCamera*, unsigned int, CCamera, ...>` is propagated (Ego_r.pdb CCamera is
//   56 bytes). Layout from Ego_r.pdb CEnvironmentThemePriorityStackEntry at retail offsets (12-byte vector), as the landed
//   caller: BaseWeight +0x00, BaseWeightLocked +0x04, Themes +0x08, ThemeTransitionTargetID +0x14,
//   ThemeTransitionInitialWeight +0x18, ThemeTransitionActive +0x1C, ThemeTransitionStartTime (double) +0x20,
//   ThemeTransitionDuration +0x28; 48 bytes (8-aligned).
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`, `__fastcall` (first in ecx -> esi as
//   `cur`, n in edx -> ebx counted down with `dec ; jne`, &x and the tag reference on the stack). The implicit memberwise
//   copy constructor: the float and bool through integer registers, the vector's out-of-line copy on `this+8`, the
//   three scalars, the double through `fld/fstp qword`, the last float; placement-new null test; cur returned in eax.
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

CEnvironmentThemePriorityStackEntry* __fastcall UninitFillN_ThemeStackEntry_00889230(CEnvironmentThemePriorityStackEntry* first, unsigned int n, const CEnvironmentThemePriorityStackEntry& x, const __false_type&)
{
    CEnvironmentThemePriorityStackEntry* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CEnvironmentThemePriorityStackEntry(x);
    return cur;
}