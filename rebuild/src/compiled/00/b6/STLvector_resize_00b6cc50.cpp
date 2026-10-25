// std::vector::resize(unsigned int new_size) -- STLport vector<CLandscapeTheme>::resize(n): resize the landscape theme list to n entries, filling with a default theme (no textures or bump maps, no water)
// Leaf: ?resize@?$vector@UCLandscapeTheme@@V?$allocator@UCLandscapeTheme@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6CC50, 56 bytes (+8 int3), thiscall on the vector, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?resize@?$vector@UCLandscapeTheme@@...@@QAEXI@Z; no direct call site in the image
//   (xref: 0 calls / 0 references); its callee is the two-argument resize CEngineLandscapeRenderer::BuildThemes
//   0x00B687D0 calls on CEngineLandscapeRenderer::LandscapeThemes (CArray<CLandscapeTheme>).
// Callees: vector<CLandscapeTheme>::resize(n, const CLandscapeTheme&) 0x00B6CB20 (out of line).
// Layout: Ego_r.pdb struct CLandscapeTheme (56 bytes): BaseTexture +0, BackgroundBaseTexture +4, CliffTexture +8,
//   BackgroundCliffTexture +0xC, BaseBumpMap +0x10, CliffBumpMap +0x14, the four min-mip levels +0x18..+0x24, the two
//   self-illuminations +0x28 / +0x2C, WaterType +0x30, WaterHeight +0x34; FableWin.pdb lists its default constructor.
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument resize out of line: the
//   constructor's zeroes all come from one `xor eax,eax` before the pushes (the six texture handles and WaterHeight from
//   the initialiser list, then WaterType assigned in the body: retail stores +0x34 before +0x30); the mip levels and illuminations stay unset.
// Label correction: catalogued as `vector<EWeaponClass>::_Umove<EWeaponClass*>` (propagated); the body is
//   vector<CLandscapeTheme>::resize(n).
enum EWaterType { WATER_TYPE_NONE = 0 };

struct CLandscapeTheme {                            // 56 bytes
    long       BaseTexture;                         // +0x00
    long       BackgroundBaseTexture;               // +0x04
    long       CliffTexture;                        // +0x08
    long       BackgroundCliffTexture;              // +0x0C
    long       BaseBumpMap;                         // +0x10
    long       CliffBumpMap;                        // +0x14
    long       BaseTextureMinMipMap;                // +0x18
    long       CliffTextureMinMipMap;               // +0x1C
    long       BaseBumpMapMinMipMap;                // +0x20
    long       CliffBumpMapMinMipMap;               // +0x24
    float      BaseSelfIllumination;                // +0x28
    float      CliffSelfIllumination;               // +0x2C
    EWaterType WaterType;                           // +0x30
    float      WaterHeight;                         // +0x34
    CLandscapeTheme()
        : BaseTexture(0), BackgroundBaseTexture(0), CliffTexture(0), BackgroundCliffTexture(0), BaseBumpMap(0),
          CliffBumpMap(0), WaterHeight(0.0f)
    {
        WaterType = WATER_TYPE_NONE;
    }
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08

    void resize(size_type __new_size, const _Tp& __x);              // 0x00B6CB20
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

template void std::vector<CLandscapeTheme>::resize(unsigned int);