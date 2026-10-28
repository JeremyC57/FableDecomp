// CEngine::PeekGlobalAmbientColour() const -- the lighting manager's global (theme channel 0) Ambient colour, returned by value
// Leaf: ?PeekGlobalAmbientColour@CEngine@@UBE?BVCRGBFloatColour@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b25d50, 46 bytes, thiscall (`this` unused), `ret 4` (hidden result).
// Identity (proven): Ego_r.pdb ?PeekGlobalAmbientColour@CEngine@@UBE?BVCRGBFloatColour@@XZ (the CIEngine virtual) and its inline callee
//   ?PeekGlobalAmbientColour@CEngineLightingManager@@QBEABVCRGBFloatColour@@XZ; the global at 0x01436E9C is the lighting manager
//   (landed users e.g. CEnginePrimitiveRenderer::InitialiseGlobalLights 0x00B32300); +0x3C is ThemeChannels (retail
//   12-byte CArray<CEngineLightingManager::CThemeChannel>, 0x70-byte elements), AmbientColour at +0x10 of a channel.
// Shape: `return g_pEngineLightingManager->PeekGlobalAmbientColour();` with the manager's inline accessor returning
//   ThemeChannels[0].AmbientColour by reference; the 16-byte colour copied one dword at a time through esi.
// Header debt: engine/CEngineLightingManager.h flattens ThemeChannels to bytes and the CThemeChannel / CRGBFloatColour
//   types have no header, so the members used are modelled locally.
class CRGBFloatColour { public: float R, G, B, A; };

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    const _Tp* begin() const { return this->_M_start; }
    const _Tp& operator[](size_type __n) const { return *(begin() + __n); }
};
}
template <class T> class CArray : public std::vector<T> { };

class C3DVector { public: float X, Y, Z; };
class C4DVector { public: float X, Y, Z, W; };

class CEngineLightingManager {
public:
    struct CThemeChannel {                                // 0x70 bytes (Ego_r.pdb)
    public:
        CRGBFloatColour DiffuseColour;                   // +0x00
        CRGBFloatColour AmbientColour;                   // +0x10
        CRGBFloatColour BacklightColour;                 // +0x20
        CRGBFloatColour ReflectionColour;                // +0x30
        CRGBFloatColour FogColour;                       // +0x40
        float           FogStartZ;                       // +0x50
        float           FogEndZ;                         // +0x54
        float           ShadowFactor;                    // +0x58
        float           FadedShadowFactor;               // +0x5c
        C4DVector       FogTransform;                    // +0x60
    };
    virtual ~CEngineLightingManager();
    unsigned char          _pad_0x04[0x14];              // +0x04 base subobject words
    C3DVector              SunVector;                    // +0x18
    C3DVector              MoonVector;                   // +0x24
    C3DVector              DiffuseVector;                // +0x30
    CArray<CThemeChannel>  ThemeChannels;                // +0x3c
    const CRGBFloatColour& PeekGlobalDiffuseColour() const { return ThemeChannels[0].DiffuseColour; }
    const CRGBFloatColour& PeekGlobalAmbientColour() const { return ThemeChannels[0].AmbientColour; }
};

extern CEngineLightingManager* g_pEngineLightingManager;   // 0x01436E9C

class CIEngine {
public:
    virtual const CRGBFloatColour PeekGlobalAmbientColour() const = 0;
};
class CEngine : public CIEngine {
public:
    virtual const CRGBFloatColour PeekGlobalAmbientColour() const;
};

const CRGBFloatColour CEngine::PeekGlobalAmbientColour() const
{
    return g_pEngineLightingManager->PeekGlobalAmbientColour();
}