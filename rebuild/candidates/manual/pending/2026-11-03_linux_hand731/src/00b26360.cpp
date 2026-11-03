// CEngine::Initialise(const CEngineInit& init) -- bring the engine up: uninitialise, take the definition manager and look up the
//   ENGINE def and the four ENGINE_VIDEO_OPTIONS defs, publish the system / display / render / shader managers, register the device
//   reset callback, size the memory pools, create the engine components (the world-map / water / landscape / sky / weather /
//   local-detail / screen-effect set and the ENVIRONMENT def only when InitFlags bit 0 is set), build the render layers and set
//   the engine's initial state; returns true.
// Leaf: ?Initialise@CEngine@@UAE_NABVCEngineInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) 0x00B26360, 5174 bytes, `ret 4`, no EH frame (no /EHsc). CEngine vtable 0x012A0F3C slot 2.
// Callees: CEngine::Uninitialise (vtable slot 85, +0x154), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CDefinitionManager::GetPDefFromClassIndex 0x009ADE10 (inside the inline GetDef<CEngineDef>(name, index, def)), GetDef<CEngineVideoOptionsDef>
//   (name, def) 0x00B28E40 (out of line), GFGetSystemManager 0x009A4EC0, CDisplayManager::AddDeviceResetCallback 0x009C0190,
//   CDefaultEngineMemoryInit() 0x00B8FBF0 / ApplyCustomSettings 0x00B8FC40, operator new 0x00BFEA1A, the 27 component constructors
//   and AddEngineComponent<T> COMDATs 0x00B29930..0x00B2A980, CBaseClassNonCopyable() 0x0099A2F0 (inline CEngineLayerRenderer ctor),
//   CEngine::AddRenderLayer 0x00B262C0, CEngineLayerRenderer::AddComponent 0x00B2AC80 (inline operator<<), GFGetTime 0x009E1BC0.
// Globals: the engine component pointers 0x01436E30..0x01436EAC, established spellings (NAME_FIXES_QUEUE c722/c723/c639 and the
//   landed CEngine::SetVideoOptions 0x00B24690 ones); 0x01436E10 (the input manager), 0x01436E20 (the ENGINE def), 0x01436E24 (the
//   ENVIRONMENT def), 0x01436E28 (the definition manager): descriptive (no PDB data symbol).
// Shape: the def names are plain string literals converted at the call (`GetDef("ENGINE", 1, &PEngineDef)`): retail builds the
//   CCharString temporary first and re-leas it, and pushes &PEngineVideoOptionsDefs[i] after it; an explicit CCharString(...)
//   argument passes the ctor's eax and pushes the def pointer before the ctor (DIFFER 5151v5174). The inline GetDef(name, index, def)
//   body is in this TU, as in the landed 0x004084B0 and chunk 727's 0x00B625E0. Components are created in retail order through the
//   private template AddEngineComponent<T>(T*, T*&, unsigned char) COMDATs; the render layers are `AddRenderLayer(new
//   CEngineLayerRenderer(layer, flags, scene_filter)) << component ...` (inline ctor and inline operator<< over AddComponent).
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CDEFAULTENGINEMEMORYINIT
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEENVIRONMENT
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINESTATEBLOCKS
#define FABLE_RETAIL_CENGINEBILLBOARDGENERATOR
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER
#define FABLE_RETAIL_CENGINESIMULATIONMANAGER
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINEWEATHERRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTCOLOURFILTER
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#include "engine/Direct3D9Types.h"
#include "engine/CEngine.h"
#include "engine/CEngineInit.h"
#include "engine/CDefaultEngineMemoryInit.h"
#include "engine/CDefinitionManager.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineLayerRenderer.h"
#include "engine/CEngineDef.h"
#include "engine/CEnvironmentDef.h"
#include "engine/CEngineVideoOptionsDef.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"
#include "engine/CEngineEnvironment.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineStateBlocks.h"
#include "engine/CEngineBillboardGenerator.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveRenderer2D.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEnginePrimitiveAttachmentManager.h"
#include "engine/CEngineSurfaceCompositionManager.h"
#include "engine/CEngineSimulationManager.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineSkyRenderer.h"
#include "engine/CEngineWeatherRenderer.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "engine/CEngineScreenEffectColourFilter.h"
#include "engine/CEngineScreenEffectGlowRenderer.h"
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"
class CInputManager;
namespace NEngineInternal {
extern CRenderManager*       GPRenderManager;          // 0x01436E18
extern CShaderRenderManager* GPShaderRenderManager;    // 0x01436E14
}
extern CInputManager*          g_pEngineInputManager_01436e10;   // 0x01436E10 (descriptive)
extern CDisplayManager*        g_pDisplayManager_01436e1c;       // 0x01436E1C
extern const CEngineDef*       g_pEngineDef_01436e20;            // 0x01436E20 (descriptive)
extern const CEnvironmentDef*  g_pEngineEnvironmentDef;          // 0x01436E24 (descriptive, chunk 727)
extern CDefinitionManager*     g_pEngineDefinitionManager;       // 0x01436E28 (descriptive, chunk 727)
extern CEngine*                g_pEngine_01436ea4;               // 0x01436EA4
extern CEngineScreenEffectRadialBlurRenderer*   g_pRadialBlurRenderer_01436e30;      // 0x01436E30
extern CEngineScreenEffectGlowRenderer*         g_pGlowRenderer_01436e34;            // 0x01436E34
extern CEngineScreenEffectDisplacementRenderer* g_pDisplacementRenderer_01436e38;    // 0x01436E38
extern CEngineScreenEffectOutlineGlow*          g_pEngineScreenEffectOutlineGlow;    // 0x01436E3C
extern CEngineScreenEffectColourFilter*         g_pColourFilter;                     // 0x01436E40
extern CEngineWeatherRenderer*                  g_pEngineWeatherRenderer;            // 0x01436E44
extern CEngineEnvironment*                      g_pEngineEnvironment_01436e48;       // 0x01436E48
extern CEngineBillboardGenerator*               g_pEngineBillboardGenerator;         // 0x01436E4C
extern CEngineSkyRenderer*                      g_pEngineSkyRenderer;                // 0x01436E50
extern CEngineWaterRenderer*                    g_pEngineWaterRenderer;              // 0x01436E54
extern CEnginePrimitiveOcclusionManager*        g_pPrimitiveOcclusionManager;        // 0x01436E58
extern CEngineOcclusionManager*                 g_pEngineOcclusionManager;           // 0x01436E5C
extern CEngineShadowRenderer*                   g_pEngineShadowRenderer;             // 0x01436E60
extern CEngineSimulationManager*                g_pEngineSimulationManager;          // 0x01436E68
extern CEnginePrimitiveAttachmentManager*       g_pEnginePrimitiveAttachmentManager; // 0x01436E70
extern CEngineSubPrimitiveRenderer*             g_pEngineSubPrimitiveRenderer;       // 0x01436E74
extern CEnginePrimitiveRenderer2D*              g_pEnginePrimitiveRenderer2D;        // 0x01436E7C
extern CEnginePrimitiveRenderer*                g_pEnginePrimitiveRenderer;          // 0x01436E80
extern CEnginePrimitiveTypeInfo*                g_pEnginePrimitiveTypeInfo;          // 0x01436E84
extern CEngineLocalDetailGenerator*             g_pLocalDetailGenerator;             // 0x01436E88
extern CEngineWorldMap*                         g_pEngineWorldMap;                   // 0x01436E8C
extern CEngineStateBlocks*                      g_pEngineStateBlocks;                // 0x01436E90
extern CEngineResourceManager*                  g_pResourceManager;                  // 0x01436E98
extern CEngineLightingManager*                  g_pEngineLightingManager;            // 0x01436E9C
extern CEngineCamera*                           g_pEngineCamera;                     // 0x01436EA0
extern CEngineLandscapeRenderer*                g_LandscapeRenderer_01436EA8;        // 0x01436EA8
extern CEngineSurfaceCompositionManager*        g_pEngineSurfaceCompositionManager;  // 0x01436EAC
extern "C" double __fastcall GFGetTime();                              // 0x009E1BC0

template <class T> bool CDefinitionManager::GetDef(const CCharString& name, long index, CDefPointer<const T>* def) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> base = GetPDefFromClassIndex(name, index);
    if (!base.IsNull()) {
        *def = reinterpret_cast<const CDefPointer<const T>&>(base);   // SetDynamicCast<const CDefClassBase> (release: the plain copy)
        return true;
    }
    return false;
}

bool CEngine::Initialise(const CEngineInit& init)
{
    Uninitialise();
    g_pEngineDefinitionManager = init.DefinitionManager;
    if (g_pEngineDefinitionManager) {
        g_pEngineDefinitionManager->GetDef("ENGINE", 1, &PEngineDef);
        g_pEngineDef_01436e20 = PEngineDef.Get();
        g_pEngineDefinitionManager->GetDef("ENGINE_VIDEO_OPTIONS_LOW", &PEngineVideoOptionsDefs[0]);
        g_pEngineDefinitionManager->GetDef("ENGINE_VIDEO_OPTIONS_NORMAL", &PEngineVideoOptionsDefs[1]);
        g_pEngineDefinitionManager->GetDef("ENGINE_VIDEO_OPTIONS_HIGH", &PEngineVideoOptionsDefs[2]);
        g_pEngineDefinitionManager->GetDef("ENGINE_VIDEO_OPTIONS_HIGHEST", &PEngineVideoOptionsDefs[3]);
    }
    g_pEngineInputManager_01436e10 = GFGetSystemManager()->GetInputManager();
    g_pDisplayManager_01436e1c = GFGetSystemManager()->GetDisplayManager();
    NEngineInternal::GPRenderManager = init.RenderManager;
    NEngineInternal::GPShaderRenderManager = &CShaderRenderManager::Get();
    g_pEngine_01436ea4 = this;
    g_pDisplayManager_01436e1c->AddDeviceResetCallback(this);
    CDefaultEngineMemoryInit memory_init;
    memory_init.ApplyCustomSettings(init.MemoryInit);
    AddEngineComponent(new CEnginePrimitiveTypeInfo(), g_pEnginePrimitiveTypeInfo, 0);
    AddEngineComponent(new CEngineResourceManager(init, memory_init), g_pResourceManager, 1);
    AddEngineComponent(new CEngineOcclusionManager(), g_pEngineOcclusionManager, 0);
    AddEngineComponent(new CEnginePrimitiveOcclusionManager(), g_pPrimitiveOcclusionManager, 0);
    AddEngineComponent(new CEngineEnvironment(), g_pEngineEnvironment_01436e48, 0);
    AddEngineComponent(new CEngineCamera(true), g_pEngineCamera, 0);
    AddEngineComponent(new CEngineStateBlocks(), g_pEngineStateBlocks, 0);
    AddEngineComponent(new CEngineBillboardGenerator(), g_pEngineBillboardGenerator, 0);
    AddEngineComponent(new CEngineScreenEffectOutlineGlow(init), g_pEngineScreenEffectOutlineGlow, 0);
    AddEngineComponent(new CEngineScreenEffectDisplacementRenderer(init), g_pDisplacementRenderer_01436e38, 0);
    AddEngineComponent(new CEngineSubPrimitiveRenderer(), g_pEngineSubPrimitiveRenderer, 0);
    AddEngineComponent(new CEnginePrimitiveRenderer(), g_pEnginePrimitiveRenderer, 0);
    AddEngineComponent(new CEnginePrimitiveRenderer2D(), g_pEnginePrimitiveRenderer2D, 0);
    AddEngineComponent(new CEngineLightingManager(), g_pEngineLightingManager, 0);
    AddEngineComponent(new CEnginePrimitiveAttachmentManager(), g_pEnginePrimitiveAttachmentManager, 0);
    AddEngineComponent(new CEngineSurfaceCompositionManager(), g_pEngineSurfaceCompositionManager, 0);
    AddEngineComponent(new CEngineSimulationManager(), g_pEngineSimulationManager, 0);
    AddEngineComponent(new CEngineShadowRenderer(), g_pEngineShadowRenderer, 0);
    if (init.InitFlags & 1) {
        g_pEngineDefinitionManager->GetDef("ENVIRONMENT", 1, &PEnvironmentDef);
        g_pEngineEnvironmentDef = PEnvironmentDef.Get();
        AddEngineComponent(new CEngineWorldMap(init), g_pEngineWorldMap, 0);
        AddEngineComponent(new CEngineWaterRenderer(init), g_pEngineWaterRenderer, 0);
        AddEngineComponent(new CEngineLandscapeRenderer(memory_init), g_LandscapeRenderer_01436EA8, 0);
        AddEngineComponent(new CEngineSkyRenderer(init), g_pEngineSkyRenderer, 0);
        AddEngineComponent(new CEngineWeatherRenderer(), g_pEngineWeatherRenderer, 0);
        AddEngineComponent(new CEngineLocalDetailGenerator((init.InitFlags & 4) != 0), g_pLocalDetailGenerator, 0);
        AddEngineComponent(new CEngineScreenEffectColourFilter(init), g_pColourFilter, 0);
        AddEngineComponent(new CEngineScreenEffectGlowRenderer(init), g_pGlowRenderer_01436e34, 0);
        AddEngineComponent(new CEngineScreenEffectRadialBlurRenderer(init), g_pRadialBlurRenderer_01436e30, 0);
    }
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x1), static_cast<ERenderRenderLayerFlags>(0), 1));
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x2), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEngineShadowRenderer << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x4), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_LandscapeRenderer_01436EA8;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x8), static_cast<ERenderRenderLayerFlags>(1), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x10), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x40), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_LandscapeRenderer_01436EA8;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x20), static_cast<ERenderRenderLayerFlags>(1), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x100), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x400), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x1000), static_cast<ERenderRenderLayerFlags>(1), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x2000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEngineSkyRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x4000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x8000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x20000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEngineWaterRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x100000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x8000000), static_cast<ERenderRenderLayerFlags>(0), 3)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x10000000), static_cast<ERenderRenderLayerFlags>(0), 3)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x10000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pDisplacementRenderer_01436e38;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x40000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x800), static_cast<ERenderRenderLayerFlags>(2), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x80000), static_cast<ERenderRenderLayerFlags>(2), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x200000), static_cast<ERenderRenderLayerFlags>(2), 1)) << g_pEnginePrimitiveRenderer << g_pEnginePrimitiveRenderer2D;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x400000), static_cast<ERenderRenderLayerFlags>(2), 1)) << g_pEnginePrimitiveRenderer << g_pEngineSkyRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x800000), static_cast<ERenderRenderLayerFlags>(2), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x2000000), static_cast<ERenderRenderLayerFlags>(2), 1)) << g_LandscapeRenderer_01436EA8 << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x80), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x200), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x4000000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEngineScreenEffectOutlineGlow;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x1000000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pColourFilter;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x8000000), static_cast<ERenderRenderLayerFlags>(0), 3)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x10000000), static_cast<ERenderRenderLayerFlags>(0), 3)) << g_pEnginePrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x20000000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEngineWeatherRenderer << g_pDisplacementRenderer_01436e38 << g_pGlowRenderer_01436e34 << g_pRadialBlurRenderer_01436e30;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x40000000), static_cast<ERenderRenderLayerFlags>(0), 3)) << g_pResourceManager;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x80000000), static_cast<ERenderRenderLayerFlags>(2), 2)) << g_pEnginePrimitiveRenderer2D;
    NonInterpolatedWorldTime = 0.0;
    NonInterpolatedPausedWorldTime = 0.0;
    EnableScreenShotMode = false;
    EnableWireFrame = false;
    EnableEngine = true;
    EnableTextures = true;
    EnablePostDeviceReset = true;
    IsInitialised = true;
    CurrentRenderFrame = 0;
    CurrentWorldFrame = 0;
    CurrentPausedWorldFrame = 0;
    WorldFramesPerSecond = 15;
    BlockOnLoad = false;
    AnisotropicFilteringLevel = 2;
    NewSceneFlag = true;
    NewSceneTimeOut = 20.0f;
    NewSceneLoadFlags = 0;
    CutscenePreloadFlag = false;
    CutscenePreloadTimeout = 2.5f;
    CutsceneFastPreloadTimeout = 1.0f / 15.0f;
    ActorAdditionalLoadTimeout = 0.1f;
    ActorRadius = 7.0f;
    ActorsLoading = false;
    CameraAngleChanged = false;
    SceneFilterFlags = 3;
    OffWorldTickProcessInfo.ProcessThisFrame = false;
    OffWorldTickProcessInfo.ProcessingEnabled = true;
    OffWorldTickProcessInfo.ProcessTicksBehindAccumilator = 0.0f;
    OffWorldTickProcessInfo.MaxProcessTicksBehind = 10.0f;
    OffWorldTickProcessInfo.MaxProcessTicksBehindBeforeCatchup = 3.0f;
    OffWorldTickProcessInfo.MinProcessFrequency = 10.0f;
    OffWorldTickProcessInfo.LastFrameTimeStamp = GFGetTime();
    return true;
}
