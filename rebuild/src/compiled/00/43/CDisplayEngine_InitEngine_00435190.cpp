// Initialize and activate the display engine, then publish the primitive fade range.
// Leaf: ?InitEngine@CDisplayEngine@@MAE_NXZ
// Flags:
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_ENGINE_GETDEF_PASS27
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFPOINTER_ORIGINAL_DIRECT_DEREF_PASS26
#define FABLE_RETAIL_CDISPLAYENGINE_INITENGINE_PASS27
#define FABLE_RETAIL_CENGINEDEF_DRAW_DISTANCE_PASS27
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEINIT_INLINE_DESTRUCTOR_PASS27
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CTHINGPHYSICAL_DEBUGDRAW_PASS24
#define FABLE_RETAIL_CTBASESINGLETON_GET_DECL
#include "engine/CDisplayEngine.h"
#include "engine/CWorld.h"
#include "engine/CIEngine.h"
#include "engine/CEngineInit.h"
#include "engine/CEngineDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CEngineManager.h"
#include "engine/NProgressDisplay.h"
class CSystemManager;

CIEngine* FABLE_FASTCALL GFAllocEngine();
CSystemManager* FABLE_FASTCALL GFGetSystemManager();

namespace NResourceDirectoryNames {
    CWideString FABLE_FASTCALL GetGraphicBankDir();
    CWideString FABLE_FASTCALL GetMeshBankDir();
    CWideString FABLE_FASTCALL GetShaderBankDir();
    CWideString FABLE_FASTCALL GetEngineCacheDir();
    CWideString FABLE_FASTCALL GetWriteableEngineCacheDir();
}

extern CRenderManager* GPRenderManager;
extern long GStaticMapQuality;
extern CEngineMemoryInit GEngineMemoryInit;
extern bool GEnableMinimumStaticMapRebuild;
extern bool GAllowDataGeneration;
extern bool g_bCaptureUIBackground;
extern bool GEnableEngineInitFlag8;  // Descriptive: the renderer flag at 0x013750A8.
extern bool g_FeatureB;
extern bool g_FeatureD;
extern bool GBuildRetailStaticMaps;

bool CDisplayEngine::InitEngine()
{
    CEngineInit engine_init;
    CSystemManager* t = GFGetSystemManager();
    unsigned long error;
    long old_id = 0;
    Engine3D = 0;
    Engine3D = GFAllocEngine();
    if (Engine3D) {
        engine_init.DefinitionManager = DefinitionManager;
        engine_init.RenderManager = GPRenderManager;
        engine_init.Font = const_cast<CFontBank*>(&Component->PeekDebugFont());
        engine_init.MainGraphicBank = EngineGraphicBank;
        engine_init.World = World;
        engine_init.MeshBank = MeshBank;
        engine_init.GraphicsResourcePath = NResourceDirectoryNames::GetGraphicBankDir();
        engine_init.MeshResourcePath = NResourceDirectoryNames::GetMeshBankDir();
        engine_init.ShaderResourcePath = NResourceDirectoryNames::GetShaderBankDir();
        engine_init.EngineCacheResourcePath = NResourceDirectoryNames::GetEngineCacheDir();
        engine_init.WriteableEngineCacheResourcePath = NResourceDirectoryNames::GetWriteableEngineCacheDir();
        engine_init.MemoryInit = GEngineMemoryInit;
        engine_init.StaticMapQuality = GStaticMapQuality;
        engine_init.InitFlags = (GEnableMinimumStaticMapRebuild ? 2 : 0) | 1;
        if (CMainGameComponent::Get()->IsEditorActive())
            engine_init.InitFlags |= 4;
        else
            engine_init.InitFlags |= GAllowDataGeneration ? 4 : 0;
        if (g_bCaptureUIBackground)
            engine_init.InitFlags |= 0x80;
        if (GEnableEngineInitFlag8)
            engine_init.InitFlags |= 8;
        if (g_FeatureB)
            engine_init.InitFlags |= 0x40;
        if (g_FeatureD)
            engine_init.InitFlags |= 0x100;
        engine_init.InitFlags |= GBuildRetailStaticMaps ? 0x10 : 0;
        NProgressDisplay::DisplayProgress("Init Engine: Initialise", -1.0f, false, false);
        if (Engine3D->Initialise(engine_init) == true) {
            NProgressDisplay::DisplayProgress("Init Engine: AddEngine", -1.0f, false, false);
            CEngineManager::Get().AddEngine(*Engine3D);
            NProgressDisplay::DisplayProgress("Init Engine: OnActivate", -1.0f, false, false);
            Engine3D->OnActivate();
            {
                CDefPointer<const CEngineDef> pengine_def;
                DefinitionManager->GetDef("ENGINE", 1, &pengine_def);
                GamePrimitiveDefaultFadeRangeRatio = pengine_def->GamePrimitiveDefaultFadeRangeRatio;
            }
            return true;
        }
    }
    return false;
}