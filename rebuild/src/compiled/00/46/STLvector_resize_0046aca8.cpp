// std::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the sound bank entries
// Leaf: ?resize@?$vector@VCSoundBankEntry@@V?$allocator@VCSoundBankEntry@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046ACA8, 38 bytes, thiscall, `ret 4`; size-optimised TU (ebp frame, `and`/`or` stores).
// Identity: vector<CSoundBankEntry>: the only caller is GFSerialiseVectorBinaryIn<CSoundBankEntry> (0x0046AB9D inside
//   0x0046AB33, landed as the CSoundBankEntry transfer); the callee 0x0046ACCE is the two-argument resize. Members from
//   Ego_r.pdb: ID, Filename, SymbolFilename, LipSyncBankHandle (CDefString = one index, -1 when empty), DVD.
//   Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x0046ACCE.
// Pragma: `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (the lander's size sweep).
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`. CSoundBankEntry() initialises only ID (`and [ebp-14h],0`); the
//   three CDefString members are `or -1`; DVD stays uninitialised.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CDefString {
public:
    long Index;
    CDefString() : Index(-1) { }
};

class CSoundBankEntry {
public:
    unsigned long ID;
    CDefString Filename;
    CDefString SymbolFilename;
    CDefString LipSyncBankHandle;
    bool DVD;
    CSoundBankEntry() : ID(0) { }
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
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x0046ACCE)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CSoundBankEntry>::resize(size_type);
}