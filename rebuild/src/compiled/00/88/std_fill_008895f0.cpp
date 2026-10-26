// std::fill(CEnvironmentThemePriorityStackEntry* first, CEnvironmentThemePriorityStackEntry* last, const CEnvironmentThemePriorityStackEntry& val) -- assign val to every theme-stack entry of [first, last)
// Leaf: fill
// Retail: Fable.exe @ 0x008895F0, 96 bytes, `ret 4`: __fastcall (first ecx -> esi, last edx -> ebx, the value
// reference on the stack -> edi). Only call site 0x00889E95 inside the landed
// vector<CEnvironmentThemePriorityStackEntry>::_M_fill_insert 0x00889DA0, which names this body as its `fill` callee.
// Identity: the catalog's pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> is propagated;
// the element is NEnvironmentInternal::CEnvironmentThemePriorityStackEntry (Ego_r.pdb, 56 bytes there with its
// 16-byte CArray; retail 48 bytes: the 12-byte Themes array moves every later member down by 4).
// Layout (Ego_r.pdb names, retail offsets): BaseWeight +0, BaseWeightLocked +4, Themes CArray<CThemeWeight> +8
//   (operator= @ 0x00888EE0), ThemeTransitionTargetID +0x14, ThemeTransitionInitialWeight +0x18,
//   ThemeTransitionActive +0x1C, ThemeTransitionStartTime (double) +0x20, ThemeTransitionDuration +0x28.
// Shape: implicit memberwise copy assignment: dword, byte, the array assignment call (source address hoisted into ebp),
// dword, dword, byte, the double through fld/fstp qword, dword; stride 0x30.
namespace NEnvironmentInternal {
template <class T>
struct CArray {                                 // the engine's 12-byte vector
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    CArray<T>& operator=(const CArray<T>& x);   // out of line
};
class CEnvironmentThemePriorityStackEntry { public:    // 48 bytes
    struct CThemeWeight {                       // 12 bytes
        long  ThemeID;                          // +0x00
        float Weight;                           // +0x04
        bool  LockWeight;                       // +0x08
    };
    float                BaseWeight;                     // +0x00
    bool                 BaseWeightLocked;               // +0x04
    CArray<CThemeWeight> Themes;                         // +0x08
    long                 ThemeTransitionTargetID;        // +0x14
    float                ThemeTransitionInitialWeight;   // +0x18
    bool                 ThemeTransitionActive;          // +0x1C
    double               ThemeTransitionStartTime;       // +0x20
    float                ThemeTransitionDuration;        // +0x28
};
}

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*,
                              NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*,
                              const NEnvironmentInternal::CEnvironmentThemePriorityStackEntry&);
}