// NEnvironmentInternal::CEnvironmentThemePriorityStack::CEnvironmentThemePriorityStack(long count) -- builds an empty entry array and resizes it to count default entries (base weight 1, unlocked, no transition)
// Leaf: ??0CEnvironmentThemePriorityStack@NEnvironmentInternal@@QAE@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008883C0, 70 bytes, thiscall, `ret 4`.
// Identity: FableWin.pdb / Ego_r.pdb ??0CEnvironmentThemePriorityStack@NEnvironmentInternal@@QAE@J@Z; the one caller is
//   CEnvironment::CEnvironment 0x006B4B66 (`PriorityStackChannels.push_back(CEnvironmentThemePriorityStack(5))`, the
//   landed 0x006B4900 already declares 0x008883C0 as this constructor).
// Callees: vector<CEnvironmentThemePriorityStackEntry>::resize(size_type, const value_type&) 0x0088A390 (out of line;
//   0x30-byte elements: the `/ 48` size and `n * 48` erase point).
// Layout: Ents (CArray<CEnvironmentThemePriorityStackEntry>, a 12-byte STLport vector) +0. Entry (0x30 bytes, Ego_r
//   member order with 12-byte CArrays): BaseWeight +0, BaseWeightLocked +4, Themes (CArray<CThemeWeight>) +8,
//   ThemeTransitionTargetID +0x14, ThemeTransitionInitialWeight +0x18, ThemeTransitionActive +0x1C,
//   ThemeTransitionStartTime (double) +0x20, ThemeTransitionDuration +0x28.
// Shape: `Ents.resize(count)` with STLport's inline one-argument `resize(n) { resize(n, _Tp()); }`: the vector's three
//   zero words, then the default entry temporary in the frame: Themes zeroed, ThemeTransitionActive from the init list,
//   then BaseWeightLocked (assigned in the constructor body -- in the init list its byte store goes first), BaseWeight
//   = 1.0f sinks below the pushes; the temporary leaves no destructor code after the call.
// Label correction: catalogued as `vector<long>::vector<int>` (propagated); the body is the
//   CEnvironmentThemePriorityStack(long) constructor.
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    typedef unsigned int size_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    void resize(size_type __new_size, const _Tp& __x);          // 0x0088A390 for the priority-stack entries
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

template <class T> class CArray : public std::vector<T> {
public:
    CArray() { }
};

namespace NEnvironmentInternal {

class CEnvironmentThemePriorityStackEntry {
public:
    class CThemeWeight;

    float                BaseWeight;                            // +0x00
    bool                 BaseWeightLocked;                      // +0x04
    CArray<CThemeWeight> Themes;                                // +0x08
    long                 ThemeTransitionTargetID;               // +0x14
    float                ThemeTransitionInitialWeight;          // +0x18
    bool                 ThemeTransitionActive;                 // +0x1C
    double               ThemeTransitionStartTime;              // +0x20
    float                ThemeTransitionDuration;               // +0x28

    CEnvironmentThemePriorityStackEntry() : BaseWeight(1.0f), ThemeTransitionActive(false)
    {
        BaseWeightLocked = false;
    }
};

class CEnvironmentThemePriorityStack {
public:
    CArray<CEnvironmentThemePriorityStackEntry> Ents;           // +0x00

    CEnvironmentThemePriorityStack(long count);
};

CEnvironmentThemePriorityStack::CEnvironmentThemePriorityStack(long count)
{
    Ents.resize(count);
}

}