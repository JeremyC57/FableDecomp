// std::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for a vector of 2D integer coordinates
// Leaf: ?resize@?$vector@VC2DCoordI@@V?$allocator@VC2DCoordI@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00657069, 21 bytes, thiscall, `ret 4`; size-optimised TU.
// Identity: vector<C2DCoordI> (Ego_r.pdb CDrawQuickAccessIcons::EffectPositions): the callee 0x00656FAB is the
//   two-argument resize over an 8-byte element. No direct reference (xref 0). C2DCoordI has an empty user constructor
//   (the landed constructor 0x00652F0A), so the default value is left uninitialised. Catalog
//   "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x00656FAB.
// Pragma: `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`: `push ecx ; push ecx` reserves the 8-byte temporary, no stores.
#pragma optimize("y", off)
#pragma optimize("s", on)
class C2DCoordI {
public:
    long X;
    long Y;
    C2DCoordI() { }
};

namespace std {
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
    typedef unsigned int size_type;
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x00656FAB)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<C2DCoordI>::resize(size_type);
}