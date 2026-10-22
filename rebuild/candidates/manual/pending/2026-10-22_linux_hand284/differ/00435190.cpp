// CDisplayEngine::InitEngine() -- create the 3D engine, fill a CEngineInit (definition manager, display caps, world draw interface, debug font, graphic / mesh banks, the five resource directories, the global memory init, static-map quality and the option flags), initialise it, register it in the engine list, activate it and cache the ENGINE def's detail level
// Grade: DIFFER(783v774) -- everything but the ENGINE def block matches: retail puts the ENGINE name temporary at [esp+4] and the def pointer at [esp+8] (VC allocates the declared def first), and releases with `dec [ecx+4]; jne` on the already-loaded pointer where VC reloads the def pointer after the decrement. Tried: Release() member (pointee / CRefCounted base), local data pointer, !-- / split decrement / unsigned count, def block, const Data member
// Leaf: InitEngine
#include "engine/CDisplayEngine.h"
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString {
public:
    void* m_pStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);  // 0x0099B7D0
};
struct CDefinitionManager;
struct CEngineDef;
struct CRenderManager;
struct CIDrawWorld { virtual void v00(); };
struct CWorldFirstBase { virtual void v00(); };
struct CWorld : CWorldFirstBase, CIDrawWorld { };
struct CFontBank;
struct CEngineMemoryInit { long Data[10]; };
struct CEngineInit {                                    // retail 0x64 (no DebugString member)
    CDefinitionManager* DefinitionManager;              // +0x00
    CEngineDef*         EngineDef;                      // +0x04
    CRenderManager*     RenderManager;                  // +0x08
    CIDrawWorld*        World;                          // +0x0c
    CFontBank*          Font;                           // +0x10
    CGraphicDataBank*   MainGraphicBank;                // +0x14
    CGraphicDataBank*   TerrainGraphicBank;             // +0x18
    CMeshDataBank*      MeshBank;                       // +0x1c
    CWideString         MeshResourcePath;               // +0x20
    CWideString         GraphicsResourcePath;           // +0x24
    CWideString         ShaderResourcePath;             // +0x28
    CWideString         EngineCacheResourcePath;        // +0x2c
    CWideString         WriteableEngineCacheResourcePath;   // +0x30
    CEngineMemoryInit   MemoryInit;                     // +0x34
    long                StaticMapQuality;               // +0x5c
    long                InitFlags;                      // +0x60
    CEngineInit();                                      // 0x0042D830
};
struct CIEngine {
    virtual void v00(); virtual void v01();
    virtual bool Init(const CEngineInit& init);         // slot 2
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void OnActivate();                          // slot 49
};
struct CMainGameComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsRunningInWindow();                   // slot 9
    unsigned char _pad004[0x16144 - 4];
    CFontBank*    PDebugFont;                           // +0x16144 CCountedPointer<CFontBank>::Data
};
struct CEngineList { void push_back(CIEngine* engine); };   // 0x006404D0
struct CEngineDefData {
    virtual void v00();
    virtual void DeleteThis();                          // slot 1
    long          RefCount;                             // +0x04
    unsigned char _pad08[0x50 - 8];
    long          DetailLevel;                          // +0x50
};
template <class T> struct CDefPointer {
    T* Data;
    CDefPointer() : Data(0) {}
    __forceinline ~CDefPointer() { if (!--Data->RefCount) Data->DeleteThis(); }
    T* operator->() const { return Data; }
};
struct CGameDefinitionManager {
    template <class T> bool GetDef(const CCharString& name, long flags, CDefPointer<T>* out) const;   // 0x00430096
};
void* GetEngineSingleton();                             // 0x009A4EC0
CIEngine* CreateEngine();                               // 0x00B26340
CEngineList* GetEngineList();                           // 0x00436FB0
CWideString __fastcall GetGraphicsDir();                // 0x0041A100
CWideString __fastcall GetMeshDir();                    // 0x0041A1C0
CWideString __fastcall GetShaderDir();                  // 0x0041A1E0
CWideString __fastcall GetEngineCacheDir();             // 0x0041A200
CWideString __fastcall GetWriteableEngineCacheDir();    // 0x0041A220
void __fastcall DisplayProgress(const CCharString& text, bool a, float progress, bool b);   // 0x009E9F40
extern CRenderManager* g_pDisplayCaps;                  // 0x013B8384
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
extern long g_StaticMapQuality;                         // 0x01375490
extern CEngineMemoryInit g_EngineMemoryInit;            // 0x0137549C
extern bool g_bInitFlag2;                               // 0x013B8649
extern bool g_bInitFlag4;                               // 0x01375459
extern bool g_bInitFlag80;                              // 0x013750A7
extern bool g_bInitFlag8;                               // 0x013750A8
extern bool g_bInitFlag40;                              // 0x013750A5
extern bool g_bInitFlag100;                             // 0x013750A6
extern bool g_bInitFlag10;                              // 0x013B864A
extern long g_EngineDetailLevel;                        // 0x013B8774

struct CDisplayEngine_Init : CDisplayEngine {
    bool InitEngine();
};

bool CDisplayEngine_Init::InitEngine()
{
    CEngineInit init;
    GetEngineSingleton();
    Engine3D = 0;
    Engine3D = CreateEngine();
    if (Engine3D) {
        init.DefinitionManager = (CDefinitionManager*)DefinitionManager;
        init.RenderManager = g_pDisplayCaps;
        init.Font = Component->PDebugFont;
        init.MainGraphicBank = EngineGraphicBank;
        init.World = World;
        init.MeshBank = MeshBank;
        init.GraphicsResourcePath = GetGraphicsDir();
        init.MeshResourcePath = GetMeshDir();
        init.ShaderResourcePath = GetShaderDir();
        init.EngineCacheResourcePath = GetEngineCacheDir();
        init.WriteableEngineCacheResourcePath = GetWriteableEngineCacheDir();
        init.MemoryInit = g_EngineMemoryInit;
        init.StaticMapQuality = g_StaticMapQuality;
        init.InitFlags = (g_bInitFlag2 ? 2 : 0) | 1;
        if (g_pMainGameComponent->IsRunningInWindow())
            init.InitFlags |= 4;
        else
            init.InitFlags |= g_bInitFlag4 ? 4 : 0;
        if (g_bInitFlag80)
            init.InitFlags |= 0x80;
        if (g_bInitFlag8)
            init.InitFlags |= 8;
        if (g_bInitFlag40)
            init.InitFlags |= 0x40;
        if (g_bInitFlag100)
            init.InitFlags |= 0x100;
        init.InitFlags |= g_bInitFlag10 ? 0x10 : 0;
        DisplayProgress("Init Engine: Initialise", false, -1.0f, false);
        if (Engine3D->Init(init) == true) {
            DisplayProgress("Init Engine: AddEngine", false, -1.0f, false);
            GetEngineList()->push_back(Engine3D);
            DisplayProgress("Init Engine: OnActivate", false, -1.0f, false);
            Engine3D->OnActivate();
            {
                CDefPointer<CEngineDefData> def;
                DefinitionManager->GetDef("ENGINE", 1, &def);
                g_EngineDetailLevel = def->DetailLevel;
            }
            return true;
        }
    }
    return false;
}
