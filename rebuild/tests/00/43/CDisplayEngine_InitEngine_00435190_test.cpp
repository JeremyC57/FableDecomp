// The engine doubles expose the same calls as the selected translation unit.
// The InitEngine body below is copied verbatim and verified by a source hash.
#include <stdio.h>
#include <string.h>
#include <assert.h>

enum EEvent {
    EVENT_SYSTEM, EVENT_ALLOC, EVENT_INITIALISE_PROGRESS, EVENT_INIT,
    EVENT_ADD_PROGRESS, EVENT_ADD, EVENT_ACTIVATE_PROGRESS, EVENT_ACTIVATE,
    EVENT_LOOKUP, EVENT_RELEASE
};
static EEvent Events[10];
static int EventCount;
static void Record(EEvent event)
{
    assert(EventCount < sizeof(Events) / sizeof(Events[0]));
    Events[EventCount++] = event;
}

class CDefinitionManager {};
class CRenderManager {};
class CIDrawWorld {};
class CWorld : public CIDrawWorld {};
class CFontBank {};
class CGraphicDataBank {};
class CMeshDataBank {};
class CSystemManager {};
class CEngineDef {
public:
    float GamePrimitiveDefaultFadeRangeRatio;
};

template<class T> class CDefPointer {
public:
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Record(EVENT_RELEASE); }
    T* operator->() const { assert(Object); return Object; }
    T* Object;
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    bool GetDef(const char* name, long index, CDefPointer<const CEngineDef>* out) const {
        assert(strcmp(name, "ENGINE") == 0 && index == 1);
        Record(EVENT_LOOKUP);
        out->Object = &Def;
        return true;
    }
    CEngineDef Def;
};
struct CEngineMemoryInit { long Values[10]; };
struct CEngineInit {
    CEngineInit() : DefinitionManager(0), RenderManager(0), Font(0),
        MainGraphicBank(0), World(0), MeshBank(0), GraphicsResourcePath(0),
        MeshResourcePath(0), ShaderResourcePath(0), EngineCacheResourcePath(0),
        WriteableEngineCacheResourcePath(0), StaticMapQuality(0), InitFlags(0) {}
    CDefinitionManager* DefinitionManager;
    CRenderManager* RenderManager;
    CFontBank* Font;
    CGraphicDataBank* MainGraphicBank;
    CIDrawWorld* World;
    CMeshDataBank* MeshBank;
    const char* GraphicsResourcePath;
    const char* MeshResourcePath;
    const char* ShaderResourcePath;
    const char* EngineCacheResourcePath;
    const char* WriteableEngineCacheResourcePath;
    CEngineMemoryInit MemoryInit;
    long StaticMapQuality;
    long InitFlags;
};
static CEngineInit CapturedInit;
static bool Allocate;
static bool InitResult;
class CIEngine {
public:
    bool Initialise(const CEngineInit& init) {
        Record(EVENT_INIT);
        CapturedInit = init;
        return InitResult;
    }
    void OnActivate() { Record(EVENT_ACTIVATE); }
};
static CIEngine Engine;
static CSystemManager System;
CSystemManager* GFGetSystemManager() { Record(EVENT_SYSTEM); return &System; }
CIEngine* GFAllocEngine() { Record(EVENT_ALLOC); return Allocate ? &Engine : 0; }
class CEngineManager {
public:
    static CEngineManager& Get() {
        static CEngineManager manager;
        return manager;
    }
    void AddEngine(CIEngine& engine) {
        assert(&engine == &Engine);
        Record(EVENT_ADD);
    }
};

class CMainGameComponent {
public:
    static CMainGameComponent* Get() {
        static CMainGameComponent game;
        return &game;
    }
    bool IsEditorActive() const { return EditorActive; }
    const CFontBank& PeekDebugFont() const { return Font; }
    bool EditorActive;
    CFontBank Font;
};

class CDisplayEngine {
public:
    bool InitEngine();
    CIEngine* Engine3D;
    CGameDefinitionManager* DefinitionManager;
    CMainGameComponent* Component;
    CGraphicDataBank* EngineGraphicBank;
    CWorld* World;
    CMeshDataBank* MeshBank;
    static float GamePrimitiveDefaultFadeRangeRatio;
};
float CDisplayEngine::GamePrimitiveDefaultFadeRangeRatio;
CRenderManager* GPRenderManager;
long GStaticMapQuality;
CEngineMemoryInit GEngineMemoryInit;
bool GEnableMinimumStaticMapRebuild;
bool GAllowDataGeneration;
bool g_bCaptureUIBackground;
bool GEnableEngineInitFlag8;
bool g_FeatureB;
bool g_FeatureD;
bool GBuildRetailStaticMaps;

namespace NResourceDirectoryNames {
const char* GetGraphicBankDir() { return "Graphics"; }
const char* GetMeshBankDir() { return "Meshes"; }
const char* GetShaderBankDir() { return "Shaders"; }
const char* GetEngineCacheDir() { return "Cache"; }
const char* GetWriteableEngineCacheDir() { return "Writable cache"; }
}

namespace NProgressDisplay {
void DisplayProgress(const char* message, float progress, bool first, bool second)
{
    assert(progress == -1.0f && !first && !second);
    if (strcmp(message, "Init Engine: Initialise") == 0)
        Record(EVENT_INITIALISE_PROGRESS);
    else if (strcmp(message, "Init Engine: AddEngine") == 0)
        Record(EVENT_ADD_PROGRESS);
    else {
        assert(strcmp(message, "Init Engine: OnActivate") == 0);
        Record(EVENT_ACTIVATE_PROGRESS);
    }
}
}

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

int main()
{
    CGameDefinitionManager defs;
    CGraphicDataBank graphics;
    CMeshDataBank meshes;
    CWorld world;
    CRenderManager renderer;
    CDisplayEngine display;
    display.DefinitionManager = &defs;
    display.Component = CMainGameComponent::Get();
    display.EngineGraphicBank = &graphics;
    display.World = &world;
    display.MeshBank = &meshes;
    defs.Def.GamePrimitiveDefaultFadeRangeRatio = 0.875f;
    GPRenderManager = &renderer;
    GStaticMapQuality = 3;
    for (int entry = 0; entry != 10; ++entry)
        GEngineMemoryInit.Values[entry] = entry + 27;

    Allocate = false;
    InitResult = false;
    EventCount = 0;
    display.Engine3D = &Engine;
    CDisplayEngine::GamePrimitiveDefaultFadeRangeRatio = 0.25f;
    assert(!display.InitEngine() && display.Engine3D == 0);
    assert(EventCount == 2 && Events[0] == EVENT_SYSTEM && Events[1] == EVENT_ALLOC);
    assert(CDisplayEngine::GamePrimitiveDefaultFadeRangeRatio == 0.25f);

    Allocate = true;
    EventCount = 0;
    assert(!display.InitEngine() && display.Engine3D == &Engine);
    assert(EventCount == 4 && Events[2] == EVENT_INITIALISE_PROGRESS && Events[3] == EVENT_INIT);
    assert(CDisplayEngine::GamePrimitiveDefaultFadeRangeRatio == 0.25f);

    InitResult = true;
    for (int choices = 0; choices != 256; ++choices) {
        EventCount = 0;
        display.World = choices & 1 ? &world : 0;
        GEnableMinimumStaticMapRebuild = (choices & 1) != 0;
        GAllowDataGeneration = (choices & 2) != 0;
        display.Component->EditorActive = (choices & 4) != 0;
        g_bCaptureUIBackground = (choices & 8) != 0;
        GEnableEngineInitFlag8 = (choices & 16) != 0;
        g_FeatureB = (choices & 32) != 0;
        g_FeatureD = (choices & 64) != 0;
        GBuildRetailStaticMaps = (choices & 128) != 0;
        const long expected = 1 | (GEnableMinimumStaticMapRebuild ? 2 : 0) |
            (GAllowDataGeneration || display.Component->EditorActive ? 4 : 0) |
            (g_bCaptureUIBackground ? 0x80 : 0) | (GEnableEngineInitFlag8 ? 8 : 0) |
            (g_FeatureB ? 0x40 : 0) | (g_FeatureD ? 0x100 : 0) |
            (GBuildRetailStaticMaps ? 0x10 : 0);

        assert(display.InitEngine() && display.Engine3D == &Engine);
        assert(EventCount == sizeof(Events) / sizeof(Events[0]));
        for (int event = 0; event != EventCount; ++event)
            assert(Events[event] == event);
        assert(CapturedInit.DefinitionManager == &defs);
        assert(CapturedInit.RenderManager == &renderer);
        assert(CapturedInit.Font == &display.Component->Font);
        assert(CapturedInit.MainGraphicBank == &graphics);
        assert(CapturedInit.World == display.World);
        assert(CapturedInit.MeshBank == &meshes);
        assert(strcmp(CapturedInit.GraphicsResourcePath, "Graphics") == 0);
        assert(strcmp(CapturedInit.MeshResourcePath, "Meshes") == 0);
        assert(strcmp(CapturedInit.ShaderResourcePath, "Shaders") == 0);
        assert(strcmp(CapturedInit.EngineCacheResourcePath, "Cache") == 0);
        assert(strcmp(CapturedInit.WriteableEngineCacheResourcePath, "Writable cache") == 0);
        for (int entry = 0; entry != 10; ++entry)
            assert(CapturedInit.MemoryInit.Values[entry] == entry + 27);
        assert(CapturedInit.StaticMapQuality == 3);
        assert(CapturedInit.InitFlags == expected);
        assert(CDisplayEngine::GamePrimitiveDefaultFadeRangeRatio == 0.875f);
    }
    puts("00435190_TEST PASS");
    return 0;
}