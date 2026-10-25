// _STL::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the degradable-state records
// Leaf: ?resize@?$vector@VCDegradableInfo@@V?$allocator@VCDegradableInfo@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067F3A0, 45 bytes, thiscall, `ret 4`.
// Identity: vector<CDegradableInfo> (28 bytes: HealthPercentage, CEngineGraphic EngineGraphic +4 (14 bytes, 2-packed),
//   SmashParticleEmitter +0x14, BlocksNavigation +0x18 -- Ego_r.pdb): the callee 0x0067F220 is the two-argument resize.
//   No direct reference (xref 0). FableWin.pdb has ??0CDegradableInfo@@QAE@XZ and ??0CEngineGraphic@@QAE@XZ.
//   Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x0067F220.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`; CDegradableInfo() has an empty body, so only the embedded
//   CEngineGraphic() runs: three dwords (the two floats as integer zeros in a speed TU) and two bytes at +4..+0x11.
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
class CDegradableInfo {
public:
    float HealthPercentage;
    CEngineGraphic EngineGraphic;
    long SmashParticleEmitter;
    bool BlocksNavigation;
    CDegradableInfo() { }
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
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x0067F220)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CDegradableInfo>::resize(size_type);
}