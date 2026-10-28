// CEngine::SmartRestoreComponentVideoMemory() -- after a device reset: release and restore every engine component's default-pool resources, then re-apply the shadow and effects video options; while any renderer refuses its option, step the shadow quality down (above 1 first), then the effects quality, then the shadow quality again, and retry; reports whether a quality had to drop, or failure
// Leaf: ?SmartRestoreComponentVideoMemory@CEngine@@AAE?AW4ESmartRestoreComponentVideoMemoryResult@1@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B24500, 399 bytes, thiscall `ret`, returns 0 (restored as set), 1 (restored
//   at a lower quality) or 2 (a component failed to restore, or nothing is left to lower).
// Identity (proven): Ego_r.pdb ?SmartRestoreComponentVideoMemory@CEngine@@AAE?AW4ESmartRestoreComponentVideoMemoryResult@1@XZ
//   (private, no arguments, a CEngine-nested result enum; donor 0x009AEC80, 0x1A0 bytes); callers CEngine::SetVideoOptions
//   and CEngine::OnPostDeviceReset (the next Ego_r names). Body: CEngine::EngineComponents (+0x168) slot 11/12
//   (CEngineComponent::OnReleaseDefaultPoolResources / OnRestoreDefaultPoolResources, Ego_r names, RTTI vtable 0x012A2544),
//   VideoOptions.ShadowQuality +0xE8 / EffectsQuality +0xEC, PeekVideoOptionsDef (PEngineVideoOptionsDefs[4] +0x120),
//   CEngineShadowRenderer::ChangeVideoOptionShadowQuality(long, float) 0x00B4FC10 (Ego_r ?ChangeVideoOptionShadowQuality@
//   CEngineShadowRenderer@@QAE_NJM@Z; ShadowBufferSize / ShadowDistanceScale), and SetEffectQuality (vtable slot 13, Ego_r
//   virtual bool (const CEngineVideoOptionsDef&)) on the displacement, glow and radial-blur renderers; the step-down is the
//   inlined CEngine::GetNextLowerQualitySetting (Ego_r, private).
// Globals (no PDB data names; each type proven by the constructor that publishes it, see the landed AddEngineComponent
//   instantiations): 0x01436E60 CEngineShadowRenderer* (bound ?g_pShadowRenderer_01436e60@@3PAVCEngineShadowRenderer@@A),
//   0x01436E38 CEngineScreenEffectDisplacementRenderer*, 0x01436E34 CEngineScreenEffectGlowRenderer*, 0x01436E30
//   CEngineScreenEffectRadialBlurRenderer* (descriptive names).
// Shape: `failed` starts false and ORs in each renderer's refusal (retail's first result goes through eax into bl;
//   `bool failed = <first>` builds bl directly, grade 1; one `|` expression promotes to int, grade 2); the result flag is
//   declared before the two qualities (its store comes first).
// Layout: local CEngine (engine/CEngine.h has no virtuals or methods); retail CEngineVideoOptionsDef is 4 bytes shorter
//   than Ego_r's (ShadowBufferSize +0x64, ShadowDistanceScale +0x68).
class CCharString;
class CEngineComponentParams;
class CEngineComponentLoadNewSceneParams;
class CEngineComponentRenderParams;
enum EEngineVideoOptionQuality { };

class CEngineVideoOptionsDef {
public:
    unsigned char _pad_0x00[0x64];                         // +0x00 def base .. ZSpriteDrawDistanceMultiplier
    long ShadowBufferSize;                                 // +0x64
    float ShadowDistanceScale;                             // +0x68
};
class CEngineComponent {
public:
    virtual ~CEngineComponent();
    virtual void FrameStart(const CEngineComponentParams& params);
    virtual void FrameEnd(const CEngineComponentParams& params);
    virtual bool LoadNewScene(const CEngineComponentLoadNewSceneParams& params);
    virtual void Render(const CEngineComponentRenderParams& params);
    virtual void PreLayerRender(const CEngineComponentRenderParams& params);
    virtual void PostLayerRender(const CEngineComponentRenderParams& params);
    virtual void WorldFrameUpdate(bool paused);
    virtual void PausedWorldFrameUpdate();
    virtual CCharString GetConsoleEnableFunctionName();
    virtual unsigned long PeekSceneFilterFlags() = 0;      // slot 10 (pure in the base; landed name, not in the PDB)
    virtual void OnReleaseDefaultPoolResources();          // slot 11
    virtual bool OnRestoreDefaultPoolResources();          // slot 12
};
class CEngineShadowRenderer : public CEngineComponent {
public:
    bool ChangeVideoOptionShadowQuality(long shadowBufferSize, float shadowDistanceScale);   // 0x00B4FC10
};
class CEngineScreenEffectDisplacementRenderer : public CEngineComponent {
public:
    virtual bool SetEffectQuality(const CEngineVideoOptionsDef& def);   // slot 13
};
class CEngineScreenEffectGlowRenderer : public CEngineComponent {
public:
    virtual bool SetEffectQuality(const CEngineVideoOptionsDef& def);   // slot 13
};
class CEngineScreenEffectRadialBlurRenderer : public CEngineComponent {
public:
    virtual bool SetEffectQuality(const CEngineVideoOptionsDef& def);   // slot 13
};
extern CEngineShadowRenderer* g_pShadowRenderer_01436e60;                       // 0x01436E60
extern CEngineScreenEffectDisplacementRenderer* g_pDisplacementRenderer_01436e38;  // 0x01436E38
extern CEngineScreenEffectGlowRenderer* g_pGlowRenderer_01436e34;               // 0x01436E34
extern CEngineScreenEffectRadialBlurRenderer* g_pRadialBlurRenderer_01436e30;   // 0x01436E30

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x0
    _Tp* _M_finish;                                        // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
}
template <class T> class CArray : public std::vector<T> { };
template <class T> class CDefPointer {
public:
    T* Object;
    const T& operator*() const { return *Object; }
};
class CEngineVideoOptions {
public:
    unsigned char _pad_0x00[0x28];                         // +0x00 ScreenResolution .. TextureDetail
    EEngineVideoOptionQuality ShadowQuality;               // +0x28
    EEngineVideoOptionQuality EffectsQuality;              // +0x2C
    long Antialiasing;                                     // +0x30
};

class CEngine {
public:
    enum ESmartRestoreComponentVideoMemoryResult { };
    virtual ~CEngine();
private:
    ESmartRestoreComponentVideoMemoryResult SmartRestoreComponentVideoMemory();
    EEngineVideoOptionQuality GetNextLowerQualitySetting(EEngineVideoOptionQuality quality)
    {
        switch (quality) {
        case 1: return (EEngineVideoOptionQuality)0;
        case 3: return (EEngineVideoOptionQuality)2;
        case 2: return (EEngineVideoOptionQuality)1;
        }
        return (EEngineVideoOptionQuality)0;
    }
public:
    const CEngineVideoOptionsDef& PeekVideoOptionsDef(EEngineVideoOptionQuality quality)
    {
        return *PEngineVideoOptionsDefs[quality];
    }
    unsigned char _pad_0x04[0xBC];                         // +0x04
    CEngineVideoOptions VideoOptions;                      // +0xC0
    unsigned char _pad_0xf4[0x2C];                         // +0xF4 OffWorldTickProcessInfo, PEnvironmentDef, PEngineDef
    CDefPointer<CEngineVideoOptionsDef> PEngineVideoOptionsDefs[4];   // +0x120
    unsigned char _pad_0x130[0x38];                        // +0x130 FrameMemoryPool .. RenderLayers
    CArray<CEngineComponent*> EngineComponents;            // +0x168
};

CEngine::ESmartRestoreComponentVideoMemoryResult CEngine::SmartRestoreComponentVideoMemory()
{
    bool qualityLowered = false;
    EEngineVideoOptionQuality shadowQuality = VideoOptions.ShadowQuality;
    EEngineVideoOptionQuality effectsQuality = VideoOptions.EffectsQuality;
    for (;;) {
        CArray<CEngineComponent*>::iterator it;
        for (it = EngineComponents.begin(); it != EngineComponents.end(); ++it)
            (*it)->OnReleaseDefaultPoolResources();
        for (it = EngineComponents.begin(); it != EngineComponents.end(); ++it)
            if (!(*it)->OnRestoreDefaultPoolResources())
                return (ESmartRestoreComponentVideoMemoryResult)2;
        const CEngineVideoOptionsDef& effectsDef = PeekVideoOptionsDef(effectsQuality);
        const CEngineVideoOptionsDef& shadowDef = PeekVideoOptionsDef(shadowQuality);
        bool failed = false;
        failed |= g_pShadowRenderer_01436e60 &&
                      !g_pShadowRenderer_01436e60->ChangeVideoOptionShadowQuality(shadowDef.ShadowBufferSize,
                                                                                  shadowDef.ShadowDistanceScale);
        failed |= g_pDisplacementRenderer_01436e38 && !g_pDisplacementRenderer_01436e38->SetEffectQuality(effectsDef);
        failed |= g_pGlowRenderer_01436e34 && !g_pGlowRenderer_01436e34->SetEffectQuality(effectsDef);
        failed |= g_pRadialBlurRenderer_01436e30 && !g_pRadialBlurRenderer_01436e30->SetEffectQuality(effectsDef);
        if (!failed)
            return (ESmartRestoreComponentVideoMemoryResult)(qualityLowered ? 1 : 0);
        qualityLowered = true;
        if (shadowQuality > 1)
            shadowQuality = GetNextLowerQualitySetting(shadowQuality);
        else if (effectsQuality > 0)
            effectsQuality = GetNextLowerQualitySetting(effectsQuality);
        else if (shadowQuality > 0)
            shadowQuality = GetNextLowerQualitySetting(shadowQuality);
        else
            return (ESmartRestoreComponentVideoMemoryResult)2;
    }
}