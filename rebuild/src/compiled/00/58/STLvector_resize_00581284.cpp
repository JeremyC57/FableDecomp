// _STL::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the hero marriage records
// Leaf: ?resize@?$vector@VCHeroMarriageInfo@@V?$allocator@VCHeroMarriageInfo@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00581284, 42 bytes, thiscall, `ret 4`; size-optimised TU.
// Identity: vector<CHeroMarriageInfo> (24 bytes: WifeUID, MaritalHomeUID, State, GayMarriage -- Ego_r.pdb): the callee
//   0x005811F1 is the two-argument resize; the caller 0x00581466 and the TransferableVector Resize 0x005813D7 (vtable
//   0x0124F350 slot 1) share it. Local model: engine/CHeroMarriageInfo.h is a generated POD `struct` (mangles U, no
//   constructor), but FableWin.pdb has ??0CHeroMarriageInfo@@QAE@XZ (a class with a user constructor).
//   Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x005811F1.
// Pragma: `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`; the constructor zeroes the two 64-bit UIDs and State through
//   eax and the bool through al.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CHeroMarriageInfo {
public:
    unsigned __int64 WifeUID;
    unsigned __int64 MaritalHomeUID;
    long State;
    bool GayMarriage;
    CHeroMarriageInfo() : WifeUID(0), MaritalHomeUID(0), State(0), GayMarriage(false) { }
};

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
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x005811F1)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CHeroMarriageInfo>::resize(size_type);
}