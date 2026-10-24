// Grade: DIFFER(458v458) -- one scheduling pair: retail pushes the GetDef arguments as `lea eax,[ebp+70h] ; push eax ; push 1 ; lea eax,[ebp+74h] ; push eax`, this source hoists the name's `lea` above `push 1`. Tried (6 grades): base-class count (`add eax,4 ; dec [eax]`), direct count member, GetDef stand-in (fixed the release), explicit CCharString("ENGINE", -1) temporary (built after the pushes, 455v458), named block-local string (same swap). Next: GetDef's second parameter as `bool`/enum, or the lookup through an inline wrapper taking the name by const reference.
// CNewFrontendGameComponent::InitialiseEngine() -- create the engine and initialise it for the front end
// Retail 0x0042E204, 458 bytes (Ego_r.pdb ?InitialiseEngine@CNewFrontendGameComponent@@AAEXXZ). Body:
// m_pEngine.Reset(CreateEngine()) (operator new 0x178 + CEngine ctor at 0x00B26340; CCountedPointer<CIEngine>::Reset
// 0x0042FD04); the "ENGINE" CEnvironmentDef is looked up through GetDefinitionManager() (0x0043368D, the global
// 0x013B8760) ->GetDef<CEnvironmentDef>(name, 1, &def) (0x00430096); a CEngineInit (ctor 0x0042D830, dtor 0x0042F7C1)
// is filled -- render manager from the system manager's display manager (GFGetSystemManager 0x009A4EC0 -> +0x60 ->
// PRenderManager +8), no mesh bank, the menu font and front-end graphic bank, the five resource paths
// (NResourceDirectoryNames::GetGraphicBankDir 0x0041A100, GetMeshBankDir 0x0041A1C0, GetShaderBankDir 0x0041A1E0,
// GetEngineCacheDir 0x0041A200, GetWriteableEngineCacheDir 0x0041A220; CWideString operator= 0x0099B7D0, dtor
// 0x0099B510), the definition manager, the environment def, the default memory description (the 0x28-byte global
// 0x0137549C, `rep movsd`) and two init flags (4 when the WAD build global 0x01375459 is set, 0x20 for the legacy
// front end 0x013B8642) -- then m_pEngine->Initialise(init) (CIEngine slot 2), Enable3DSceneRendering(false)
// (slot 76) and Set2DPrimitiveLightingParameters(C3DVector(1,1,1), three CRGBFloatColour(1,1,1,1)) (slot 67).
// Shape: /Os TU with the biased large frame (`lea ebp,[esp-78h]`, optimize("s") alone); "ENGINE" is an implicit
// CCharString conversion (built before the pushes, re-addressed with `lea`); the env def pointer is zeroed before
// it; the lighting arguments are constructed right to left as temporaries (`fld1 ; fstp` each). The env def is
// released through an inline Release() on the def (count a direct member at +4: `dec [ecx+4] ; jne`), and GetDef
// gets a same-TU out-only stand-in so the def slot does not escape (else the pointer is re-read after the `dec`).
// Layout: retail CEngineInit has eight pointers (no DebugString: the Ego_r.pdb one is a debug-build member), so
// Font +0x10, MainGraphicBank +0x14, MeshBank +0x1C, paths +0x20..+0x30, MemoryInit +0x34, InitFlags +0x60;
// CNewFrontendGameComponent m_pMenuFont +0x10, m_pFrontEndGraphicBank +0x40, m_pEngine +0x58 (Ego_r.pdb).
// Leaf: ?InitialiseEngine@CNewFrontendGameComponent
#pragma optimize("s", on)

class CWideString
{
public:
    void* PData;
    CWideString();                                       // 0x0099AED0
    ~CWideString();                                      // 0x0099B510
    CWideString& operator=(const CWideString& other);    // 0x0099B7D0
};

class CCharString
{
public:
    void* PData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};

namespace NResourceDirectoryNames {
    CWideString __fastcall GetGraphicBankDir();          // 0x0041A100
    CWideString __fastcall GetMeshBankDir();             // 0x0041A1C0
    CWideString __fastcall GetShaderBankDir();           // 0x0041A1E0
    CWideString __fastcall GetEngineCacheDir();          // 0x0041A200
    CWideString __fastcall GetWriteableEngineCacheDir(); // 0x0041A220
}

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
};

struct CEngineMemoryInit {                               // 0x28 bytes
    long  LandscapePoolSize;
    float LandscapePhysicalMemoryRatio;
    long  LandscapeVirtualMemoryMinSize;
    long  MeshPoolSize;
    long  MeshStatsPoolSize;
    float MeshPhysicalMemoryRatio;
    long  HiresTextureMemory;
    long  PhysicsMeshPoolSize;
    long  AnimationPoolSize;
    long  ClothPoolSize;
};
extern CEngineMemoryInit g_DefaultEngineMemoryInit;      // 0x0137549C
extern bool g_WADBuildEnabled;                           // 0x01375459
extern bool g_bUseLegacyFrontend;                        // 0x013B8642

class CDefinitionManager;
class CEngineDef;
class CRenderManager;
class CIDrawWorld;
class CFontBank;
class CGraphicDataBank;
class CMeshDataBank;

struct CEngineInit {                                     // retail layout (0x64 bytes)
    CDefinitionManager* DefinitionManager;               // +0x00
    CEngineDef*         EngineDef;                       // +0x04
    CRenderManager*     RenderManager;                   // +0x08
    CIDrawWorld*        World;                           // +0x0c
    CFontBank*          Font;                            // +0x10
    CGraphicDataBank*   MainGraphicBank;                 // +0x14
    CGraphicDataBank*   TerrainGraphicBank;              // +0x18
    CMeshDataBank*      MeshBank;                        // +0x1c
    CWideString         MeshResourcePath;                // +0x20
    CWideString         GraphicsResourcePath;            // +0x24
    CWideString         ShaderResourcePath;              // +0x28
    CWideString         EngineCacheResourcePath;         // +0x2c
    CWideString         WriteableEngineCacheResourcePath;// +0x30
    CEngineMemoryInit   MemoryInit;                      // +0x34
    long                StaticMapQuality;                // +0x5c
    long                InitFlags;                       // +0x60
    CEngineInit();                                       // 0x0042D830
    ~CEngineInit();                                      // 0x0042F7C1
};

class CEnvironmentDef;
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                              // slot 1
    long m_count;                                        // +0x04
    __forceinline void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    CDefBase* Data;
    CDefPointer() : Data(0) {}
    __forceinline ~CDefPointer()
    {
        if (Data)
            Data->Release();
    }
};

class CDefinitionManager {
public:
    template <class T> bool GetDef(const CCharString& name, long kind, CDefPointer<const T>* out) const;   // 0x00430096 for CEnvironmentDef
};
CDefinitionManager* GetDefinitionManager();              // 0x0043368D
CDefBase* __fastcall LookupDefStandIn(const CCharString& name, long kind);   // stand-in helper (no retail counterpart)

#pragma auto_inline(off)
// Same-TU stand-in for CDefinitionManager::GetDef<CEnvironmentDef> (0x00430096): it reads the old def and writes only
// through `out`, so the caller's def pointer slot does not escape (its release then reads the slot once into ecx).
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CEnvironmentDef>(const CCharString& name, long kind,
                                                                                 CDefPointer<const CEnvironmentDef>* out) const
{
    CDefBase* def = LookupDefStandIn(name, kind);
    if (!def)
        return false;
    if (out->Data)
        out->Data->Release();
    out->Data = def;
    ++def->m_count;
    return true;
}
#pragma auto_inline(on)

struct CDisplayManager {
    void* _vftable;
    void* SystemManager;
    CRenderManager* PRenderManager;                      // +0x08 (scoped_ptr<CRenderManager>)
};
struct CSystemManager {
    unsigned char _pad[0x60];
    CDisplayManager* DisplayManager;                     // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();         // 0x009A4EC0

class CIEngine {
public:
    virtual void v00(); virtual void v01();
    virtual bool Initialise(const CEngineInit& init);    // slot 2 (+0x08)
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
    virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void Set2DPrimitiveLightingParameters(const C3DVector& direction, const CRGBFloatColour& directional,
                                                  const CRGBFloatColour& ambient, const CRGBFloatColour& extra);   // slot 67 (+0x10c)
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73();
    virtual void v74(); virtual void v75();
    virtual void Enable3DSceneRendering(bool enable);    // slot 76 (+0x130)
};
CIEngine* CreateEngine();                                // 0x00B26340 (new CEngine)

template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
    void Reset(T* object);                               // 0x0042FD04 for CIEngine
    T* operator->() const { return Data; }
};

class CNewFrontendGameComponent {
public:
    unsigned char                     _pad00[0x10];
    CCountedPointer<CFontBank>        m_pMenuFont;              // +0x10
    unsigned char                     _pad18[0x40 - 0x18];
    CCountedPointer<CGraphicDataBank> m_pFrontEndGraphicBank;   // +0x40
    unsigned char                     _pad48[0x58 - 0x48];
    CCountedPointer<CIEngine>         m_pEngine;                // +0x58
    void InitialiseEngine();
};

void CNewFrontendGameComponent::InitialiseEngine()
{
    m_pEngine.Reset(CreateEngine());
    CDefPointer<const CEnvironmentDef> environment;
    GetDefinitionManager()->GetDef<CEnvironmentDef>("ENGINE", 1, &environment);

    CEngineInit init;
    init.RenderManager = GFGetSystemManager()->DisplayManager->PRenderManager;
    init.MeshBank = 0;
    init.Font = m_pMenuFont.Data;
    init.MainGraphicBank = m_pFrontEndGraphicBank.Data;
    init.GraphicsResourcePath = NResourceDirectoryNames::GetGraphicBankDir();
    init.MeshResourcePath = NResourceDirectoryNames::GetMeshBankDir();
    init.ShaderResourcePath = NResourceDirectoryNames::GetShaderBankDir();
    init.EngineCacheResourcePath = NResourceDirectoryNames::GetEngineCacheDir();
    init.DefinitionManager = GetDefinitionManager();
    init.WriteableEngineCacheResourcePath = NResourceDirectoryNames::GetWriteableEngineCacheDir();
    init.EngineDef = (CEngineDef*)environment.Data;
    init.MemoryInit = g_DefaultEngineMemoryInit;
    init.InitFlags |= g_WADBuildEnabled ? 4 : 0;
    if (g_bUseLegacyFrontend)
        init.InitFlags |= 0x20;

    m_pEngine->Initialise(init);
    m_pEngine->Enable3DSceneRendering(false);
    m_pEngine->Set2DPrimitiveLightingParameters(C3DVector(1.0f, 1.0f, 1.0f), CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f),
                                                CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f), CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f));
}
