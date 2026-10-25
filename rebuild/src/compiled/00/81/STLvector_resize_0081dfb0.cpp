// std::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the texture morph entries
// Leaf: ?resize@?$vector@VCTextureMorphEntry@@V?$allocator@VCTextureMorphEntry@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081DFB0, 44 bytes, thiscall, `ret 4`.
// Identity: vector<CTextureMorphEntry> (20 bytes: MorphType, TextureLayer, ReplacingTextureIndex, BankIndex, Blend --
//   Ego_r.pdb; FableWin.pdb has no constructor, so the record is POD and value-initialisation zero-fills it). The
//   callee 0x0081DAA0 is the two-argument resize. No direct reference (xref 0). Catalog
//   "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x0081DAA0.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`; `_Tp()` of a POD is five eax zero stores.
enum EHeroMorphType { MORPH_TYPE_NONE };
class CTextureMorphEntry {
public:
    EHeroMorphType MorphType;
    long TextureLayer;
    long ReplacingTextureIndex;
    long BankIndex;
    long Blend;
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
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x0081DAA0)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CTextureMorphEntry>::resize(size_type);
}