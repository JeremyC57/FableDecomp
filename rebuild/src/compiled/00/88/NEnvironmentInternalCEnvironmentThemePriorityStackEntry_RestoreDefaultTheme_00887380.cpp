// NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::RestoreDefaultTheme() -- drop every theme weight, stop any theme transition and give the base theme full weight
// Leaf: ?RestoreDefaultTheme@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@QAEXXZ
// Retail: Fable.exe @ 0x00887380, 49 bytes, thiscall `ret`.
// Identity (proven): the landed ProcessTransitions 0x00887A30 calls this as RestoreDefaultTheme (and 0x00887330 as
//   SetOnlyTheme); FableWin.pdb ?RestoreDefaultTheme@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@QAEXXZ.
// Layout (Ego_r.pdb, the CArray 12 bytes in retail): BaseWeight +0x00, BaseWeightLocked +0x04, Themes +0x08,
//   ThemeTransitionTargetID +0x14, ThemeTransitionInitialWeight +0x18, ThemeTransitionActive +0x1c.
// Callees: std::vector<CThemeWeight>::resize(size_type, const CThemeWeight&) 0x00889440 (STLport; the inline resize(n)
//   passes a value-initialised CThemeWeight). The landed 0x00889440 is bound as vector<CBrainBehaviour>::resize (the
//   hand953 J112 correction: it is vector<CThemeWeight>::resize).
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void resize(unsigned int __new_size, const _Tp& __x);
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};
}
template <class T> class CArray : public std::vector<T> { };
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {
public:
    struct CThemeWeight {
        long ThemeID;                                    // +0x00
        float Weight;                                    // +0x04
        bool LockWeight;                                 // +0x08
    };
    float BaseWeight;                                    // +0x00
    bool BaseWeightLocked;                               // +0x04
    CArray<CThemeWeight> Themes;                         // +0x08
    long ThemeTransitionTargetID;                        // +0x14
    float ThemeTransitionInitialWeight;                  // +0x18
    bool ThemeTransitionActive;                          // +0x1c
    void RestoreDefaultTheme();
};
}
void NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::RestoreDefaultTheme()
{
    ThemeTransitionActive = false;
    Themes.resize(0);
    BaseWeight = 1.0f;
}