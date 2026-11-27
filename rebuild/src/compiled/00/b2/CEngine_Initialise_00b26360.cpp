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
//   ENVIRONMENT def), 0x01436E28 (the definition manager): NEngineInternal::GPDefinitionManager (PDB name).
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
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }   // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A
extern const CEngineDef*       g_pEngineDef_01436e20;            // 0x01436E20 (descriptive)
namespace NEngineInternal { extern const CEnvironmentDef* GPEnvironmentDef; }   // 0x01436E24 ?GPEnvironmentDef@NEngineInternal@@3PBVCEnvironmentDef@@B (PDB PUBLIC, Codex pass 22; respelled 2026-11-16, was the descriptive g_pEngineEnvironmentDef)
namespace NEngineInternal { extern const CDefinitionManager* GPDefinitionManager; }   // 0x01436E28 ?GPDefinitionManager@NEngineInternal@@3PBVCDefinitionManager@@B (Ego_r / FableWin / Ego_d)
namespace NEngineInternal { extern CEngine* GPEngine; }               // 0x01436EA4
extern CEngineScreenEffectRadialBlurRenderer*   g_pRadialBlurRenderer_01436e30;      // 0x01436E30
extern CEngineScreenEffectGlowRenderer*         g_pGlowRenderer_01436e34;            // 0x01436E34
namespace NEngineInternal { extern CEngineScreenEffectDisplacementRenderer* GPScreenEffectDisplacementRenderer; }    // 0x01436E38
namespace NEngineInternal { extern CEngineScreenEffectOutlineGlow* GPScreenEffectOutlineGlow; }   // 0x01436E3C ?GPScreenEffectOutlineGlow@NEngineInternal@@3PAVCEngineScreenEffectOutlineGlow@@A (Anniversary publics; type: ctor 0x00B891B0 / RTTI)
extern CEngineScreenEffectColourFilter*         g_pColourFilter;                     // 0x01436E40
extern CEngineWeatherRenderer*                  g_pEngineWeatherRenderer;            // 0x01436E44
// NF479 (Codex pass 29 name fixes): ?GPEngineEnvironment@NEngineInternal@@3PAVCEngineEnvironment@@A (raw Ego_r / FableWin / Ego_d PDB
//   publics; was g_pEngineEnvironment_01436e48).
namespace NEngineInternal { extern CEngineEnvironment* GPEngineEnvironment; }        // 0x01436E48
extern CEngineBillboardGenerator*               g_pEngineBillboardGenerator;         // 0x01436E4C
namespace NEngineInternal { extern CEngineSkyRenderer* GPSkyRenderer; }                // 0x01436E50
extern CEngineWaterRenderer*                    g_pEngineWaterRenderer;              // 0x01436E54
extern CEnginePrimitiveOcclusionManager*        g_pPrimitiveOcclusionManager;        // 0x01436E58
namespace NEngineInternal { extern CEngineOcclusionManager* GPOcclusionManager; }           // 0x01436E5C
namespace NEngineInternal { extern CEngineShadowRenderer* GPShadowRenderer; }             // 0x01436E60
extern CEngineSimulationManager*                g_pEngineSimulationManager;          // 0x01436E68
extern CEnginePrimitiveAttachmentManager*       g_pEnginePrimitiveAttachmentManager; // 0x01436E70
namespace NEngineInternal { extern CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer; }   // 0x01436E74 (Ego_d.pdb PUB32; NF83 respelt from g_pEngineSubPrimitiveRenderer)
namespace NEngineInternal { extern CEnginePrimitiveRenderer2D* GPPrimitiveRenderer2D; }        // 0x01436E7C
namespace NEngineInternal { extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; }          // 0x01436E80
namespace NEngineInternal { extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo; }          // 0x01436E84
// NF479 (Codex pass 29 name fixes): ?GPLocalDetailGenerator@NEngineInternal@@3PAVCEngineLocalDetailGenerator@@A (raw Ego_r / FableWin /
//   Ego_d PDB publics; was the free g_pLocalDetailGenerator).
namespace NEngineInternal { extern CEngineLocalDetailGenerator*             GPLocalDetailGenerator; }             // 0x01436E88
namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }                   // 0x01436E8C
extern CEngineStateBlocks*                      g_pEngineStateBlocks;                // 0x01436E90
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }                  // 0x01436E98
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }   // 0x01436E9C ?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                     // 0x01436EA0
namespace NEngineInternal { extern CEngineLandscapeRenderer* GPLandscapeRenderer; }        // 0x01436EA8 (Ego_r / FableWin / Ego_d PDB spelling)
extern CEngineSurfaceCompositionManager*        g_pEngineSurfaceCompositionManager;  // 0x01436EAC
#include "engine/GFGetTime.h"                          // 0x009E1BC0 ?GFGetTime@@YINXZ (Ego_r.pdb / FableWin.pdb; NF133 q4294)

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
    NEngineInternal::GPDefinitionManager = init.DefinitionManager;
    if (NEngineInternal::GPDefinitionManager) {
        NEngineInternal::GPDefinitionManager->GetDef("ENGINE", 1, &PEngineDef);
        g_pEngineDef_01436e20 = PEngineDef.Get();
        NEngineInternal::GPDefinitionManager->GetDef("ENGINE_VIDEO_OPTIONS_LOW", &PEngineVideoOptionsDefs[0]);
        NEngineInternal::GPDefinitionManager->GetDef("ENGINE_VIDEO_OPTIONS_NORMAL", &PEngineVideoOptionsDefs[1]);
        NEngineInternal::GPDefinitionManager->GetDef("ENGINE_VIDEO_OPTIONS_HIGH", &PEngineVideoOptionsDefs[2]);
        NEngineInternal::GPDefinitionManager->GetDef("ENGINE_VIDEO_OPTIONS_HIGHEST", &PEngineVideoOptionsDefs[3]);
    }
    g_pEngineInputManager_01436e10 = GFGetSystemManager()->GetInputManager();
    NEngineInternal::GPDisplayManager = GFGetSystemManager()->GetDisplayManager();
    NEngineInternal::GPRenderManager = init.RenderManager;
    NEngineInternal::GPShaderRenderManager = &CShaderRenderManager::Get();
    NEngineInternal::GPEngine = this;
    NEngineInternal::GPDisplayManager->AddDeviceResetCallback(this);
    CDefaultEngineMemoryInit memory_init;
    memory_init.ApplyCustomSettings(init.MemoryInit);
    AddEngineComponent(new CEnginePrimitiveTypeInfo(), NEngineInternal::GPPrimitiveTypeInfo, 0);
    AddEngineComponent(new CEngineResourceManager(init, memory_init), NEngineInternal::GPResourceManager, 1);
    AddEngineComponent(new CEngineOcclusionManager(), NEngineInternal::GPOcclusionManager, 0);
    AddEngineComponent(new CEnginePrimitiveOcclusionManager(), g_pPrimitiveOcclusionManager, 0);
    AddEngineComponent(new CEngineEnvironment(), NEngineInternal::GPEngineEnvironment, 0);
    AddEngineComponent(new CEngineCamera(true), NEngineInternal::GPCamera, 0);
    AddEngineComponent(new CEngineStateBlocks(), g_pEngineStateBlocks, 0);
    AddEngineComponent(new CEngineBillboardGenerator(), g_pEngineBillboardGenerator, 0);
    AddEngineComponent(new CEngineScreenEffectOutlineGlow(init), NEngineInternal::GPScreenEffectOutlineGlow, 0);
    AddEngineComponent(new CEngineScreenEffectDisplacementRenderer(init), NEngineInternal::GPScreenEffectDisplacementRenderer, 0);
    AddEngineComponent(new CEngineSubPrimitiveRenderer(), NEngineInternal::GPSubPrimitiveRenderer, 0);
    AddEngineComponent(new CEnginePrimitiveRenderer(), NEngineInternal::GPPrimitiveRenderer, 0);
    AddEngineComponent(new CEnginePrimitiveRenderer2D(), NEngineInternal::GPPrimitiveRenderer2D, 0);
    AddEngineComponent(new CEngineLightingManager(), NEngineInternal::GPLightingManager, 0);
    AddEngineComponent(new CEnginePrimitiveAttachmentManager(), g_pEnginePrimitiveAttachmentManager, 0);
    AddEngineComponent(new CEngineSurfaceCompositionManager(), g_pEngineSurfaceCompositionManager, 0);
    AddEngineComponent(new CEngineSimulationManager(), g_pEngineSimulationManager, 0);
    AddEngineComponent(new CEngineShadowRenderer(), NEngineInternal::GPShadowRenderer, 0);
    if (init.InitFlags & 1) {
        NEngineInternal::GPDefinitionManager->GetDef("ENVIRONMENT", 1, &PEnvironmentDef);
        NEngineInternal::GPEnvironmentDef = PEnvironmentDef.Get();
        AddEngineComponent(new CEngineWorldMap(init), NEngineInternal::GPEngineWorldMap, 0);
        AddEngineComponent(new CEngineWaterRenderer(init), g_pEngineWaterRenderer, 0);
        AddEngineComponent(new CEngineLandscapeRenderer(memory_init), NEngineInternal::GPLandscapeRenderer, 0);
        AddEngineComponent(new CEngineSkyRenderer(init), NEngineInternal::GPSkyRenderer, 0);
        AddEngineComponent(new CEngineWeatherRenderer(), g_pEngineWeatherRenderer, 0);
        AddEngineComponent(new CEngineLocalDetailGenerator((init.InitFlags & 4) != 0), NEngineInternal::GPLocalDetailGenerator, 0);
        AddEngineComponent(new CEngineScreenEffectColourFilter(init), g_pColourFilter, 0);
        AddEngineComponent(new CEngineScreenEffectGlowRenderer(init), g_pGlowRenderer_01436e34, 0);
        AddEngineComponent(new CEngineScreenEffectRadialBlurRenderer(init), g_pRadialBlurRenderer_01436e30, 0);
    }
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x1), static_cast<ERenderRenderLayerFlags>(0), 1));
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x2), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPShadowRenderer << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x4), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPLandscapeRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x8), static_cast<ERenderRenderLayerFlags>(1), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x10), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x40), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPLandscapeRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x20), static_cast<ERenderRenderLayerFlags>(1), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x100), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x400), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x1000), static_cast<ERenderRenderLayerFlags>(1), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x2000), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPSkyRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x4000), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x8000), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x20000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEngineWaterRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x100000), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x8000000), static_cast<ERenderRenderLayerFlags>(0), 3)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x10000000), static_cast<ERenderRenderLayerFlags>(0), 3)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x10000), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPScreenEffectDisplacementRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x40000), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x800), static_cast<ERenderRenderLayerFlags>(2), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x80000), static_cast<ERenderRenderLayerFlags>(2), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x200000), static_cast<ERenderRenderLayerFlags>(2), 1)) << NEngineInternal::GPPrimitiveRenderer << NEngineInternal::GPPrimitiveRenderer2D;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x400000), static_cast<ERenderRenderLayerFlags>(2), 1)) << NEngineInternal::GPPrimitiveRenderer << NEngineInternal::GPSkyRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x800000), static_cast<ERenderRenderLayerFlags>(2), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x2000000), static_cast<ERenderRenderLayerFlags>(2), 1)) << NEngineInternal::GPLandscapeRenderer << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x80), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x200), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x4000000), static_cast<ERenderRenderLayerFlags>(0), 1)) << NEngineInternal::GPScreenEffectOutlineGlow;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x1000000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pColourFilter;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x8000000), static_cast<ERenderRenderLayerFlags>(0), 3)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x10000000), static_cast<ERenderRenderLayerFlags>(0), 3)) << NEngineInternal::GPPrimitiveRenderer;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x20000000), static_cast<ERenderRenderLayerFlags>(0), 1)) << g_pEngineWeatherRenderer << NEngineInternal::GPScreenEffectDisplacementRenderer << g_pGlowRenderer_01436e34 << g_pRadialBlurRenderer_01436e30;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x40000000), static_cast<ERenderRenderLayerFlags>(0), 3)) << NEngineInternal::GPResourceManager;
    AddRenderLayer(new CEngineLayerRenderer(static_cast<EEngineRenderLayer>(0x80000000), static_cast<ERenderRenderLayerFlags>(2), 2)) << NEngineInternal::GPPrimitiveRenderer2D;
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