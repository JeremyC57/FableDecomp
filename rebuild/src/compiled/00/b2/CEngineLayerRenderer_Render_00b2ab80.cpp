// CEngineLayerRenderer::Render(const CEngineComponentParams&) -- dispatch the pre-layer, render, and post-layer component passes for the active scene-filter mask.
// Leaf: ?Render@CEngineLayerRenderer@@UAEXABVCEngineComponentParams@@@Z
// Retail 00B2AB80, 251 bytes. RTTI vtable 012A0F04 COL->CHD proves CEngineLayerRenderer -> CBaseClassNonCopyable -> CBaseClass at offset 0.
// PDB CEngineLayerRenderer fields: Layer +4, Flags +8, SceneFilterFlags +0c, Components +10. No generated parameter header exists in rebuild/include/engine; the local parameter views retain the names used by the component API, with native offsets/size asserted below.
// The donor's Components is a 16-byte CArray; retail constructor/render use a 12-byte STLport
// first/last/end vector, so this is an explicit native projection. CEngineComponent.h supplies
// the real component flags at +8. Component dispatch slots are the actual CEngineComponent
// methods: Render +10, PreLayerRender +14, PostLayerRender +18, GetSceneFilterFlags +28.
#include "engine/CEngineComponent.h"
#include "fable_string.h"
#include <stddef.h>

class CBaseClass { public:
    virtual ~CBaseClass(); // RTTI vtable 0x01231710 slot 0; constructor 0x0099A2D0
    CBaseClass();
};

struct CBaseClassNonCopyable : CBaseClass {
    virtual ~CBaseClassNonCopyable(); // constructor 0x0099A2F0; its RTTI is present in sibling engine objects
    CBaseClassNonCopyable();
};

// The enum type names are from the verified constructor signature; values are a
// fixture-only projection because the donor enumeration declarations are absent.
enum EEngineRenderLayer { EEngineRenderLayer_FixtureValue = 0 };
enum ERenderRenderLayerFlags { ERenderRenderLayerFlags_FixtureValue = 0 };

class CEngineComponentParams { public:
    float FrameInterpolation;
    unsigned char InterpolationInfo[0x70];
    unsigned long FrameID;
    unsigned long SceneFilterFlags;
    bool OffWorldTickProcessFlag;
    unsigned char _pad_0x7d[3];
};

class CEngineComponentLoadNewSceneParams;
class CEngineComponentRenderParams { public:
    const CEngineComponentParams* Params;
    long RenderLayer;
    unsigned long RenderLayerFlags;
};

struct CEngineComponentDispatch {
    virtual ~CEngineComponentDispatch();
    virtual void FrameStart(const CEngineComponentParams&) { }
    virtual void FrameEnd(const CEngineComponentParams&) { }
    virtual bool LoadNewScene(const CEngineComponentLoadNewSceneParams&) { return false; }
    virtual void Render(const CEngineComponentRenderParams&) { }
    virtual void PreLayerRender(const CEngineComponentRenderParams&) { }
    virtual void PostLayerRender(const CEngineComponentRenderParams&) { }
    virtual void WorldFrameUpdate(bool) { }
    virtual void PausedWorldFrameUpdate() { }
    // This inherited slot is unused by Render; the standalone fixture supplies a harmless stub.
    virtual CCharString GetConsoleEnableFunctionName();
    virtual unsigned long GetSceneFilterFlags() = 0;
    virtual void OnReleaseDefaultPoolResources() { }
    virtual bool OnRestoreDefaultPoolResources() { return true; }

    void* GlobalPointerAddress;
    bool RenderEnabled;
    unsigned char KeepAlive;
    unsigned char _pad_0x0a[2];
};

struct CEngineComponentPointerArray {
    CEngineComponent** First;
    CEngineComponent** Last;
    CEngineComponent** End;
};

class CEngineLayerRenderer : public CBaseClassNonCopyable { public:
    virtual ~CEngineLayerRenderer();
    virtual void Render(const CEngineComponentParams& params);

    EEngineRenderLayer Layer;
    ERenderRenderLayerFlags Flags;
    unsigned long SceneFilterFlags;
    CEngineComponentPointerArray Components;
    CEngineLayerRenderer(EEngineRenderLayer layer, ERenderRenderLayerFlags flags,
                         unsigned long sceneFilterFlags); // exact PDB signature at 0x02C99F80
};

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CEngineComponentParams) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CEngineComponentParams, SceneFilterFlags) == 0x78);
FABLE_STATIC_ASSERT(sizeof(CEngineComponentRenderParams) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(CEngineComponentRenderParams, Params) == 0x00);
FABLE_STATIC_ASSERT(offsetof(CEngineComponentRenderParams, RenderLayer) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CEngineComponentRenderParams, RenderLayerFlags) == 0x08);
FABLE_STATIC_ASSERT(sizeof(CEngineComponentDispatch) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(CEngineComponentDispatch, GlobalPointerAddress) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CEngineComponentDispatch, RenderEnabled) == 0x08);
FABLE_STATIC_ASSERT(sizeof(CEngineComponentPointerArray) == 0x0c);
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CBaseClassNonCopyable) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CEngineLayerRenderer, Layer) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CEngineLayerRenderer, Flags) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CEngineLayerRenderer, SceneFilterFlags) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(CEngineLayerRenderer, Components) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CEngineLayerRenderer) == 0x1c);
#endif

void CEngineLayerRenderer::Render(const CEngineComponentParams& params)
{
    if ((params.SceneFilterFlags & SceneFilterFlags) == 0)
        return;

    CEngineComponentRenderParams renderParams;
    renderParams.Params = &params;
    renderParams.RenderLayer = Layer;
    renderParams.RenderLayerFlags = Flags;

    for (unsigned long i = 0; i < (unsigned long)(Components.Last - Components.First); ++i) {
        if (reinterpret_cast<CEngineComponentDispatch*>(Components.First[i])->GetSceneFilterFlags() &
            params.SceneFilterFlags)
            reinterpret_cast<CEngineComponentDispatch*>(Components.First[i])->PreLayerRender(renderParams);
    }

    for (unsigned long i = 0; i < (unsigned long)(Components.Last - Components.First); ++i) {
        if (reinterpret_cast<CEngineComponentDispatch*>(Components.First[i])->GetSceneFilterFlags() &
            params.SceneFilterFlags) {
            CEngineComponent* const component = Components.First[i];
            if (component->RenderEnabled)
                reinterpret_cast<CEngineComponentDispatch*>(component)->Render(renderParams);
        }
    }

    for (unsigned long i = 0; i < (unsigned long)(Components.Last - Components.First); ++i) {
        if (reinterpret_cast<CEngineComponentDispatch*>(Components.First[i])->GetSceneFilterFlags() &
            params.SceneFilterFlags)
            reinterpret_cast<CEngineComponentDispatch*>(Components.First[i])->PostLayerRender(renderParams);
    }
}