// std::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the environment theme priority-stack entries
// Leaf: ?resize@?$vector@VCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@V?$allocator@VCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088A4E0, 53 bytes, thiscall, `ret 4`.
// Identity: vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry>: the callee 0x0088A390 is the two-argument
//   resize over the 48-byte entry (divide 0x2AAAAAAB, `sar edx,3`; landed same-TU relatives 0x00889300 / 0x008895F0 use
//   the same entry). The default entry is BaseWeight = 1.0f (+0), BaseWeightLocked = false (+4), an empty Themes vector
//   (+8..+0x10) and ThemeTransitionActive = false (+0x1C) -- Ego_r.pdb member names, retail offsets (12-byte STLport
//   vector). No reference to this body was found (xref: 0); catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is
//   propagated across byte-identical forwarders.
// Callees: vector::resize(size_type, const value_type&) @ 0x0088A390.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`. The entry constructor initialises BaseWeight and
//   ThemeTransitionActive in its list and BaseWeightLocked in the body: that gives retail's order (Themes zeroed, the +0x1C
//   byte, the +4 byte, then the 1.0f immediate after the pushes); a three-member list stores +4 before the vector.
//   Retail never destroys the default entry, here or in the same-TU 0x008883C0 (`resize(n, _Tp())` with no free of the
//   empty Themes storage), although the entry's real destructor frees Themes (the erase loop at 0x00889AB0). The
//   real STLport destructor chain makes VC7.1 emit the `if (_M_start) deallocate` tail (also with same-TU stand-in
//   bodies and under /GL), so the Themes vector is modelled with its constructor only; the empty default entry owns no
//   storage, so nothing is leaked.
namespace std {
template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x0088A390)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {     // retail 48 bytes (8-aligned for the double)
public:
    struct CThemeWeight {                        // 12 bytes
    public:
        long ThemeID;
        float Weight;
        bool LockWeight;
    };
    float BaseWeight;                           // +0x00
    bool BaseWeightLocked;                      // +0x04
    std::vector<CThemeWeight> Themes;          // +0x08  CArray<CThemeWeight>
    long ThemeTransitionTargetID;               // +0x14
    float ThemeTransitionInitialWeight;         // +0x18
    bool ThemeTransitionActive;                 // +0x1C
    double ThemeTransitionStartTime;            // +0x20
    float ThemeTransitionDuration;              // +0x28
    CEnvironmentThemePriorityStackEntry() : BaseWeight(1.0f), ThemeTransitionActive(false) { BaseWeightLocked = false; }
};
}

namespace std {
template void vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry>::resize(size_type);
}