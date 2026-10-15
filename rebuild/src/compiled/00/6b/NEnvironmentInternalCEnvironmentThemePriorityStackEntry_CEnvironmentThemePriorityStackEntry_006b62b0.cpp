// NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CEnvironmentThemePriorityStackEntry(const CEnvironmentThemePriorityStackEntry&) -- memberwise copy of an environment theme priority-stack entry (the theme-weight array through its copy constructor)
// Leaf: ??0CEnvironmentThemePriorityStackEntry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B62B0, length 67 bytes.
// Callees: CArray<CThemeWeight>::CArray(const CArray&) @ 0x006B58F0.
// Identity: the manifest label CProfile::CProfile is propagated. The caller is the vector insert 0x00889DA0
// (48-byte elements: the 0x2AAAAAAB reciprocal with `sar 3`); the only Ego_r.pdb class with this shape is
// NEnvironmentInternal::CEnvironmentThemePriorityStackEntry (PDB 56 bytes with a 16-byte CArray -> retail
// 0x30): BaseWeight +0, BaseWeightLocked +4, Themes +8, ThemeTransitionTargetID +0x14,
// ThemeTransitionInitialWeight +0x18, ThemeTransitionActive +0x1c, ThemeTransitionStartTime +0x20 (double),
// ThemeTransitionDuration +0x28.
// Shape: memberwise in declaration order; floats move through integer registers, the double through the FPU.
namespace NEnvironmentInternal {

struct CEnvironmentThemePriorityStackEntry {                // 0x30 bytes in retail
    struct CThemeWeight;
    struct CThemeWeightArray {                              // retail CArray<CThemeWeight>: three pointers
        CThemeWeight* m_pFirst;
        CThemeWeight* m_pLast;
        CThemeWeight* m_pEnd;
        CThemeWeightArray(const CThemeWeightArray& other);  // 0x006B58F0
    };
    float             BaseWeight;                           // +0x00
    bool              BaseWeightLocked;                     // +0x04
    CThemeWeightArray Themes;                               // +0x08
    long              ThemeTransitionTargetID;              // +0x14
    float             ThemeTransitionInitialWeight;         // +0x18
    bool              ThemeTransitionActive;                // +0x1c
    double            ThemeTransitionStartTime;             // +0x20
    float             ThemeTransitionDuration;              // +0x28
    CEnvironmentThemePriorityStackEntry(const CEnvironmentThemePriorityStackEntry& other);
};

CEnvironmentThemePriorityStackEntry::CEnvironmentThemePriorityStackEntry(const CEnvironmentThemePriorityStackEntry& other)
    : BaseWeight(other.BaseWeight), BaseWeightLocked(other.BaseWeightLocked), Themes(other.Themes),
      ThemeTransitionTargetID(other.ThemeTransitionTargetID),
      ThemeTransitionInitialWeight(other.ThemeTransitionInitialWeight),
      ThemeTransitionActive(other.ThemeTransitionActive),
      ThemeTransitionStartTime(other.ThemeTransitionStartTime),
      ThemeTransitionDuration(other.ThemeTransitionDuration)
{
}

}