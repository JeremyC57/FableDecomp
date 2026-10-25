// std::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the (EHeroMorphType, CTextureMorphEntry) pair vector
// Leaf: ?resize@?$vector@U?$pair@W4EHeroMorphType@@VCTextureMorphEntry@@@std@@V?$allocator@U?$pair@W4EHeroMorphType@@VCTextureMorphEntry@@@std@@@2@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081DE90, 64 bytes, thiscall, `ret 4`.
// Identity: the storage vector of CVectorMap<EHeroMorphType, CTextureMorphEntry> (CTextureMorphs). No direct reference to
//   this body (xref 0); the callee 0x0081D920 is the two-argument resize over a 24-byte element (pair = 4-byte enum +
//   20-byte CTextureMorphEntry), also called by the map's binary transfer-in 0x0081B300 (at 0x0081B38B; its catalog
//   name "CExpressionSet::TransferBinaryIn" is propagated).
//   FableWin.pdb names the record class CTextureMorphEntry (V) with no user constructor, and the pair `U?$pair@`.
//   Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated across byte-identical forwarders.
// Callees: vector::resize(size_type, const value_type&) @ 0x0081D920.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` with `pair() : first(_T1()), second(_T2())`. The POD
//   CTextureMorphEntry() value-init zero-fills a temporary and is then COPIED into `second`: VC7.1 keeps that zero
//   temporary in eax/edx/esi/edi (`mov edx,eax ; mov esi,eax ; mov edi,eax`) and stores it late, while first's
//   `EHeroMorphType()` is an immediate `mov dword [esp+10h],0`.
enum EHeroMorphType { MORPH_TYPE_NONE };

class CTextureMorphEntry {                     // 20 bytes, POD (no user constructor in FableWin.pdb)
public:
    EHeroMorphType MorphType;                  // +0x00
    long TextureLayer;                         // +0x04
    long ReplacingTextureIndex;                // +0x08
    long BankIndex;                            // +0x0C
    long Blend;                                // +0x10
};

namespace std {
template <class _T1, class _T2>
struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

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
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x0081D920)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<std::pair<EHeroMorphType, CTextureMorphEntry> >::resize(size_type);
}