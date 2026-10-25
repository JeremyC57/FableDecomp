// _STL::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for a vector of longs
// Leaf: ?resize@?$vector@JV?$allocator@J@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004433F0, 27 bytes, thiscall, `ret 4`.
// Identity: vector<long>: the callee 0x004428F0 is the two-argument resize over a 4-byte element (shrink/grow with
//   `lea [eax*4]`); callers 0x00590EA3, 0x005F3E6D, 0x0064E0CF, 0x0066EA9D (the landed NHeroInformationScreens
//   CManager::InitialiseScrollBarIndices sizes its long index vector through it). Catalog
//   "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated across byte-identical forwarders.
// Callees: vector::resize(size_type, const value_type&) @ 0x004428F0.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`; the `long()` temporary lives in the `push ecx` frame word and
//   its zero is an immediate store scheduled after the argument pushes.
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
    typedef unsigned int size_type;
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x004428F0)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<long>::resize(size_type);
}