// Leaf: ?BuildThemes@CEngineLandscapeRenderer@@
// Grade: DIFFER(1486v1483) -- residue: stack-slot order (retail def +0x14 / i +0x18 / type +0x1c, ours i / type / def) and the
//   block-4 register choice (retail frees ebp for the graphic pointer and reloads the def from its slot; ours spills it).
//   Everything else -- the field copy order with per-store _M_start reloads, the bank lookups, the branch-free log2 mip
//   search, the zero-reset theme ctor, the 0x38 stride loop -- is instruction-identical (88 of ~480 instructions differ,
//   all in those two places). 16 variants tried (see NOTES.md); the closest are kept here.
// CEngineLandscapeRenderer::BuildThemes() -- size the landscape theme table to the number of instantiated ENGINE_THEME
//   defs, then copy each theme def's textures, bump maps, self-illumination and water settings into its CLandscapeTheme
//   and derive the four minimum mip levels (log2 of the graphic's frame width over the def's maximum size).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B687D0, 1483 bytes, __thiscall, plain `ret`; PDB ?BuildThemes@CEngineLandscapeRenderer@@AAEXXZ.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("ENGINE_THEME" 0x01235F48),
//   CDefinitionManager::GetNoInstantiatedDefsOfType 0x009AD3B0 (catalogued LTextSystem::GetIDFromSymbol) and
//   GetDef<CEngineThemeDef> 0x00B6BD40 (catalogued GetDef<CSoundThemeDef>: byte-identical instantiations) on the
//   definition manager 0x01436E28; vector<CLandscapeTheme>::resize(n, const T&) 0x00B6CB20 (catalogued for CBrainBehaviour).
// Layout: the theme vector is retail +0x4B8 (the Ego_r.pdb CEngineLandscapeRenderer is a different, larger build; the
//   generated engine header does not fit retail, so a local view is used); CLandscapeTheme (56 bytes) and CEngineThemeDef
//   (retail = PDB - 4: BaseTexture +0x3C .. WaterType +0x70) from Ego_r.pdb; the graphic bank is the engine's (0x01436E98)
//   +0x954, its 0x2C-byte CGraphicInfo vector at +0x1E0 with Frames +0x28, each frame's GraphicInfo +0x0C, FrameWidth +6.
// Why the landed source differed: it was a behavioural paraphrase (a `while` loop log2, invented helper calls) that
//   compiled to 327 bytes; this is a fresh reconstruction from the retail bytes.
// Shape: every theme field store indexes Themes[i] afresh (retail reloads _M_start per store); the mip helper takes the
//   frame width and the max size unsigned (retail `div`) and is the branch-free 16/8/4/2/1 highest-bit search on q + 1.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long RefCount;                                          // +0x04
    void ReleaseTemporary() const
    {
        CDefBase* self = const_cast<CDefBase*>(this);
        if (--self->RefCount == 0)
            self->Destroy();
    }
};

struct CEngineThemeDef : CDefBase {
    unsigned char _pad08[0x3c - 0x08];
    long  BaseTexture;                                      // +0x3c
    long  BackgroundTexture;                                // +0x40
    long  CliffBaseTexture;                                 // +0x44
    long  CliffBackgroundTexture;                           // +0x48
    long  BaseBumpMap;                                      // +0x4c
    long  CliffBumpMap;                                     // +0x50
    long  BaseTextureMaxSize;                               // +0x54
    long  CliffTextureMaxSize;                              // +0x58
    long  BaseBumpMapMaxSize;                               // +0x5c
    long  CliffBumpMapMaxSize;                              // +0x60
    float BaseTextureSelfIllumination;                      // +0x64
    float CliffTextureSelfIllumination;                     // +0x68
    float WaterHeight;                                      // +0x6c
    long  WaterType;                                        // +0x70 EWaterType
};

template <class T> struct CDefPointer {          // parked shape: no dtor; the def is released explicitly through the
    T* Data;                                      // register copy (retail releases with no null check: `dec [ebp+4]`)
    CDefPointer() : Data(0) { }
    T* operator->() const { return Data; }
};

struct CDefinitionManager {
    long GetNoInstantiatedDefsOfType(const CCharString& type) const;                       // 0x009AD3B0
    template <class T> bool GetDef(const CCharString& type, long index, CDefPointer<const T>* def) const;   // 0x00B6BD40
};
extern CDefinitionManager* g_pDefinitionManager;            // 0x01436E28

struct CGraphicInfo;
struct CGraphicInfoFrame {                                  // 16 bytes
    unsigned char _pad00[0x0c];
    CGraphicInfo* GraphicInfo;                              // +0x0c
};
struct CGraphicHeader {
    unsigned char  _pad00[6];
    unsigned short FrameWidth;                              // +0x06
    unsigned short FrameHeight;                             // +0x08
};
struct CGraphicInfo {                                       // 0x2c bytes
    CGraphicHeader     Header;
    unsigned char      _pad0a[0x28 - sizeof(CGraphicHeader)];
    CGraphicInfoFrame* Frames;                              // +0x28
};
template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(_M_start + n); }
    const T& operator[](unsigned int n) const { return *(_M_start + n); }
};
struct CGraphicDataBank {
    unsigned char              _pad000[0x1e0];
    CRetailVector<CGraphicInfo> Graphics;                   // +0x1e0
    const CGraphicInfo* GetGraphicInfo(unsigned long index) const { return index < Graphics.size() ? &Graphics[index] : 0; }
    __forceinline const CGraphicInfoFrame* GetFrames(unsigned long index) const
    {
        if (GetGraphicInfo(index))
            return GetGraphicInfo(index)->Frames;
        return 0;
    }
};
struct CEngine {
    unsigned char     _pad000[0x954];
    CGraphicDataBank* GraphicBank;                          // +0x954
};
extern CEngine* g_pEngine;                                  // 0x01436E98

struct CLandscapeTheme {                                    // 56 bytes (Ego_r.pdb)
    long  BaseTexture;                                      // +0x00
    long  BackgroundBaseTexture;                            // +0x04
    long  CliffTexture;                                     // +0x08
    long  BackgroundCliffTexture;                           // +0x0c
    long  BaseBumpMap;                                      // +0x10
    long  CliffBumpMap;                                     // +0x14
    long  BaseTextureMinMipMap;                             // +0x18
    long  CliffTextureMinMipMap;                            // +0x1c
    long  BaseBumpMapMinMipMap;                             // +0x20
    long  CliffBumpMapMinMipMap;                            // +0x24
    float BaseSelfIllumination;                             // +0x28
    float CliffSelfIllumination;                            // +0x2c
    long  WaterType;                                        // +0x30 EWaterType
    float WaterHeight;                                      // +0x34
    CLandscapeTheme()
        : BaseTexture(0), BackgroundBaseTexture(0), CliffTexture(0), BackgroundCliffTexture(0), BaseBumpMap(0),
          CliffBumpMap(0)
    {
        WaterHeight = 0.0f;
        WaterType = 0;
    }
};
template <class T> struct vector : CRetailVector<T> {
    void resize(unsigned int n, const T& x);                // 0x00B6CB20 for CLandscapeTheme
    void resize(unsigned int n) { resize(n, T()); }
};

static __forceinline long GetMinMipMap(unsigned long frameWidth, unsigned long maxSize)
{
    unsigned long ratio = frameWidth / maxSize;
    if (ratio == 0)
        return -1;
    if (ratio >= 0x80000000UL)
        return 31;
    ratio += 1;
    long shift = ((long)(0x10000 - ratio) >> 31) & 16;
    shift += ((long)((1 << (shift + 8)) - ratio) >> 31) & 8;
    shift += ((long)((1 << (shift + 4)) - ratio) >> 31) & 4;
    shift += ((long)((1 << (shift + 2)) - ratio) >> 31) & 2;
    shift += ((long)((1 << (shift + 1)) - ratio) >> 31) & 1;
    return shift;
}

static __forceinline unsigned long GetFrameWidth(long graphic)
{
    return g_pEngine->GraphicBank->GetFrames(graphic)->GraphicInfo->Header.FrameWidth;
}

class CEngineLandscapeRenderer {
public:
    unsigned char           _pad000[0x4b8];
    vector<CLandscapeTheme> Themes;                         // +0x4b8
    void BuildThemes();
};

void CEngineLandscapeRenderer::BuildThemes()
{
    long count = g_pDefinitionManager->GetNoInstantiatedDefsOfType("ENGINE_THEME");
    Themes.resize(count);
    for (unsigned long i = 1; i < Themes.size(); ++i) {
        CDefPointer<const CEngineThemeDef> def;
        g_pDefinitionManager->GetDef("ENGINE_THEME", i, &def);
        const CEngineThemeDef* d = def.Data;
        Themes[i].BaseTexture = d->BaseTexture;
        Themes[i].BackgroundBaseTexture = d->BackgroundTexture;
        Themes[i].CliffTexture = d->CliffBaseTexture;
        Themes[i].BackgroundCliffTexture = d->CliffBackgroundTexture;
        Themes[i].BaseBumpMap = d->BaseBumpMap;
        Themes[i].CliffBumpMap = d->CliffBumpMap;
        Themes[i].WaterHeight = d->WaterHeight;
        Themes[i].WaterType = d->WaterType;
        Themes[i].BaseTextureMinMipMap = 0;
        Themes[i].CliffTextureMinMipMap = 0;
        Themes[i].BaseBumpMapMinMipMap = 0;
        Themes[i].CliffBumpMapMinMipMap = 0;
        Themes[i].BaseSelfIllumination = d->BaseTextureSelfIllumination;
        Themes[i].CliffSelfIllumination = d->CliffTextureSelfIllumination;
        if (d->BaseTextureMaxSize > 0)
            Themes[i].BaseTextureMinMipMap = GetMinMipMap(GetFrameWidth(Themes[i].BaseTexture), d->BaseTextureMaxSize);
        if (d->CliffTextureMaxSize > 0)
            Themes[i].CliffTextureMinMipMap = GetMinMipMap(GetFrameWidth(Themes[i].CliffTexture), d->CliffTextureMaxSize);
        if (d->BaseBumpMapMaxSize > 0)
            Themes[i].BaseBumpMapMinMipMap = GetMinMipMap(GetFrameWidth(Themes[i].BaseBumpMap), d->BaseBumpMapMaxSize);
        if (d->CliffBumpMapMaxSize > 0)
            Themes[i].CliffBumpMapMinMipMap = GetMinMipMap(GetFrameWidth(Themes[i].CliffBumpMap), d->CliffBumpMapMaxSize);
        d->ReleaseTemporary();
    }
}
