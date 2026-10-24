// std::_Construct(NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*, const NEnvironmentInternal::CEnvironmentThemePriorityStackEntry&) -- placement copy of an environment-theme stack entry: base weight + lock, theme-weight array, transition target/weight/active/start time/duration
// Retail: Fable.exe @ 0x006B6670, 65 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CArray<CThemeWeight> copy ctor @ 0x006B58F0.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N006b6670 {
struct CThemeWeightArray {                                 // retail 12-byte STLport-vector body (void)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CThemeWeightArray(const CThemeWeightArray& other);                  // 0x006B58F0
};
struct CEnvironmentThemePriorityStackEntry {     // NEnvironmentInternal::CEnvironmentThemePriorityStackEntry (retail sizeof 0x30)
    float             BaseWeight;                // +0x00
    bool              BaseWeightLocked;          // +0x04
    CThemeWeightArray Themes;                    // +0x08 CArray<CThemeWeight>
    long              ThemeTransitionTargetID;   // +0x14
    float             ThemeTransitionInitialWeight; // +0x18
    bool              ThemeTransitionActive;     // +0x1C
    double            ThemeTransitionStartTime;  // +0x20
    float             ThemeTransitionDuration;   // +0x28
    __forceinline CEnvironmentThemePriorityStackEntry(const CEnvironmentThemePriorityStackEntry& o)   // the implicit memberwise copy, spelled out so VC7.1 inlines it
        : BaseWeight(o.BaseWeight), BaseWeightLocked(o.BaseWeightLocked), Themes(o.Themes),
          ThemeTransitionTargetID(o.ThemeTransitionTargetID), ThemeTransitionInitialWeight(o.ThemeTransitionInitialWeight),
          ThemeTransitionActive(o.ThemeTransitionActive), ThemeTransitionStartTime(o.ThemeTransitionStartTime),
          ThemeTransitionDuration(o.ThemeTransitionDuration) { }
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CEnvironmentThemePriorityStackEntry* p, const CEnvironmentThemePriorityStackEntry& value)
{
    if (p != 0)
        new (p) CEnvironmentThemePriorityStackEntry(value);
}
}