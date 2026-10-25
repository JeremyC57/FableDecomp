// std::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the engine graphic records
// Leaf: ?resize@?$vector@VCEngineGraphic@@V?$allocator@VCEngineGraphic@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EA398, 43 bytes, thiscall, `ret 4`; size-optimised TU.
// Identity: vector<CEngineGraphic>: the only caller is the CEngineGraphic vector transfer at 0x004EA326 (inside the
//   CReplaceableMeshes binary transfer-in 0x004EA2C9); the callee 0x004EA3C3 is the two-argument resize. Ego_r.pdb:
//   sizeof(CEngineGraphic) = 14 (BankIndex, AnimStep, RenderSizeX, AdditiveAlpha +12, Type +13 as a byte), so the record
//   is 2-packed (precedent: GOTCHAS `pack(2)` on CEngineGraphic); FableWin.pdb has ??0CEngineGraphic@@QAE@XZ.
//   Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x004EA3C3.
// Pragma: `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`; the 14-byte temporary sits at [ebp-0Eh] (unaligned dword stores
//   at -0Eh/-0Ah/-6) and the size-optimised constructor reloads fldz for each float.
#pragma optimize("y", off)
#pragma optimize("s", on)
#pragma pack(push, 2)
class CEngineGraphic {
public:
    unsigned long BankIndex;
    float AnimStep;
    float RenderSizeX;
    bool AdditiveAlpha;
    unsigned char Type;
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) { }
};
#pragma pack(pop)

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
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x004EA3C3)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CEngineGraphic>::resize(size_type);
}