// _STL::vector::resize(size_type new_size, const CEnvironmentThemePriorityStackEntry& x) -- shrink with erase or grow with a fill insert (48-byte theme priority entries)
// Leaf: ?resize@?$vector@VCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088A390, 100 bytes, thiscall, `ret 8`.
// Identity: vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry>. Callers: the one-argument resize
//   0x0088A4E0 (its default entry is BaseWeight = 1.0f, BaseWeightLocked = false, an empty Themes vector, and
//   ThemeTransitionActive = false -- exactly Ego_r.pdb's CEnvironmentThemePriorityStackEntry members at +0, +4,
//   +8..+0x10, +0x1C) and 0x008883F8. Ego_r's 56-byte entry holds one CArray (16 -> 12 in retail) and a double, so the
//   retail record is 0x2C bytes padded to 48: the divide is by 48 (0x2AAAAAAB, `sar edx,3`).
//   Catalog "vector<CVectorMap<unsigned long,...>>::resize" is propagated.
// Callees: erase(iterator, iterator) @ 0x00889AB0, _M_fill_insert @ 0x00889DA0.
// Shape: STLport 4.6 `resize(n, x)` (see 0x0052BEA0).
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {   // retail 48 bytes (8-aligned for the double)
public:
    class CThemeWeight;
    float BaseWeight;                         // +0x00
    bool BaseWeightLocked;                    // +0x04
    CThemeWeight* Themes[3];                  // +0x08  CArray<CThemeWeight> (STLport vector)
    long ThemeTransitionTargetID;             // +0x14
    float ThemeTransitionInitialWeight;       // +0x18
    bool ThemeTransitionActive;               // +0x1C
    double ThemeTransitionStartTime;          // +0x20
    float ThemeTransitionDuration;            // +0x28
};
}

namespace _STL {
template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                     // out of line in retail
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);    // out of line in retail
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x) {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry>::resize(size_type, const NEnvironmentInternal::CEnvironmentThemePriorityStackEntry&);
}