// CNewFrontendGameComponent::InitialiseEngine() -- create the engine and initialise it for the front end
// Leaf: ?InitialiseEngine@CNewFrontendGameComponent@@AAEXXZ
// Retail: Fable.exe 0x0042E204, 458 bytes, size-optimised TU with the biased large frame (`lea ebp,[esp-78h]`).
// Names (Ego_r.pdb / FableWin.pdb): ?InitialiseEngine@CNewFrontendGameComponent@@AAEXXZ (private); ?GFAllocEngine@@YIPAVCIEngine@@XZ
//   0x00B26340 (`new CEngine`, 0x178 bytes); ?Reset@?$CCountedPointer@VCIEngine@@@@QAEXPAVCIEngine@@@Z 0x0042FD04;
//   ?Get@CStartupDefinitionManager@@SIAAV1@XZ 0x0043368D; ??$GetDef@VCEngineDef@@@CDefinitionManager@@QBE_NABVCCharString@@
//   JPAV?$CDefPointer@$$CBVCEngineDef@@@@@Z 0x00430096; CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0;
//   ??0CEngineInit@@QAE@XZ 0x0042D830 / ??1CEngineInit@@QAE@XZ 0x0042F7C1; ?GFGetSystemManager@@YIPAVCSystemManager@@XZ 0x009A4EC0;
//   NResourceDirectoryNames::GetGraphicBankDir 0x0041A100, GetMeshBankDir 0x0041A1C0, GetShaderBankDir 0x0041A1E0,
//   GetEngineCacheDir 0x0041A200, GetWriteableEngineCacheDir 0x0041A220 (all `?...@NResourceDirectoryNames@@YI?AVCWideString@@XZ`);
//   CWideString::operator= 0x0099B7D0, ~CWideString 0x0099B510. Globals: g_WADBuildEnabled 0x01375459, g_bUseLegacyFrontend
//   0x013B8642 (bound names), the default CEngineMemoryInit 0x0137549C (descriptive name). CIEngine virtuals by slot from the
//   landed CEngine ctor 0x00B260B0 table: Initialise 2 (+0x08), Set2DPrimitiveLightingParameters 67 (+0x10C),
//   Enable3DSceneRendering 76 (+0x130).
// Body: m_pEngine.Reset(GFAllocEngine()); the "ENGINE" CEngineDef through CStartupDefinitionManager::Get().GetDef(); a
//   CEngineInit filled (render manager from the system manager's display manager, no mesh bank, the menu font and front-end
//   graphic bank, the five resource paths, the definition manager, the engine def, the default memory description
//   (`rep movsd` of 10 dwords) and two flags: 4 for the WAD build, 0x20 for the legacy front end); then Initialise(init),
//   Enable3DSceneRendering(false) and Set2DPrimitiveLightingParameters(C3DVector(1,1,1), three CRGBFloatColour(1,1,1,1)).
// Shape: the manager is CStartupDefinitionManager::Get(), a static __fastcall returning a REFERENCE (both PDBs). With that real
//   signature VC7.1 pushes the GetDef arguments as retail (`lea eax,[ebp+70h] ; push eax ; push 1 ; lea eax,[ebp+74h] ;
//   push eax`); the old pointer-returning GetDefinitionManager() model hoisted the name's lea above `push 1` through 6 grades.
//   GetDef<CEngineDef> is a template instantiated in this TU (0x00430096 sits among the CNewFrontendGameComponent rows), so its
//   body is here under auto_inline(off): with it VC7.1 knows the def slot does not escape and releases it as retail
//   (`mov ecx,[ebp+70h] ; dec [ecx+4] ; jne`); declared-only it re-reads through `add eax,4 ; dec [eax] ; cmp [eax],0`.
//   The def is the CEngineDef named "ENGINE" (FableWin GetDef<CEngineDef> instantiation; CEngineInit::EngineDef is CEngineDef*).
//   "ENGINE" is an implicit CCharString conversion (built before the pushes); the engine def pointer is a null local
//   (`and [ebp+70h],0`) released by its inline destructor at the end (`dec [ecx+4] ; jne ; call [eax+4]`).
// Layout: retail CEngineInit has no DebugString (an Ego_r debug member): Font +0x10, MainGraphicBank +0x14, MeshBank +0x1C, paths
//   +0x20..+0x30, MemoryInit +0x34, InitFlags +0x60. CNewFrontendGameComponent (Ego_r.pdb, no engine header): m_pMenuFont +0x10,
//   m_pFrontEndGraphicBank +0x40, m_pEngine +0x58.
// Header debt: engine/CDefPointer.h (retail model) declares no null constructor (FableWin ??0..QAE@W4ENullDef@@@Z; enumerator
//   name unproven), so the CDefPointer below is that header's model plus the ENullDef constructor.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointeeBase.h"
#define CDefinitionManager CDefinitionManagerPdb
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager

class CWideString
{
public:
    CWideString();                                       // 0x0099AED0
    ~CWideString();                                      // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
private:
    void* PStringData;
};

class CCharString
{
public:
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
private:
    void* PStringData;
};

namespace NResourceDirectoryNames
{
    CWideString __fastcall GetGraphicBankDir();          // 0x0041A100
    CWideString __fastcall GetMeshBankDir();             // 0x0041A1C0
    CWideString __fastcall GetShaderBankDir();           // 0x0041A1E0
    CWideString __fastcall GetEngineCacheDir();          // 0x0041A200
    CWideString __fastcall GetWriteableEngineCacheDir(); // 0x0041A220
}

class C3DVector
{
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CRGBFloatColour
{
public:
    float R, G, B, A;
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
};

class CEngineMemoryInit                                  // 0x28 bytes
{
public:
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
class CEnvironmentDef;
class CRenderManager;
class CIDrawWorld;
class CFontBank;
class CGraphicDataBank;
class CMeshDataBank;

class CEngineInit                                        // retail layout (0x64 bytes)
{
public:
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

enum ENullDef { ENullDef_DescriptiveNull = 0 };
template <class T> class CDefPointer
{
public:
    CDefPointer(ENullDef = ENullDef_DescriptiveNull) : Object(0) {}
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    __forceinline ~CDefPointer() { Release(); }
    template <class U> __forceinline void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    bool IsNull() const { return Object == 0; }
    __forceinline void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                             // +0x00
private:
    __forceinline void Assign(CDefPointeeBase* object)
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

class CDefClassBase;
class CDefinitionManager : public CDefinitionManagerPdb
{
public:
    template <class T> bool GetDef(const CCharString& name, long index, CDefPointer<const T>* def) const;   // 0x00430096 (T = CEngineDef)
    CDefPointer<const CDefClassBase> GetPDefFromClassIndex(const CCharString& name, long index) const;      // 0x009ADE10
};

// The template's body is part of this TU in retail (0x00430096 sits among the CNewFrontendGameComponent rows).
#pragma auto_inline(off)
template <class T>
__declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& name, long index, CDefPointer<const T>* def) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> base = GetPDefFromClassIndex(name, index);
    if (!base.IsNull()) {
        def->SetDynamicCast(base);
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CStartupDefinitionManager : public CDefinitionManager
{
public:
    static CStartupDefinitionManager& __fastcall Get();  // 0x0043368D
};

class CDisplayManager                                    // local model (no engine header)
{
public:
    void* __vftable_unused;                              // +0x00
    void* PSystemManager;                                // +0x04
    CRenderManager* PRenderManager;                      // +0x08 (scoped_ptr<CRenderManager>)
};
class CSystemManager                                     // local model (no engine header)
{
public:
    unsigned char _pad[0x60];
    CDisplayManager* DisplayManager;                     // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();         // 0x009A4EC0

class CCamera; class C2DBoxF; class CEngineInterpolationInfo; class C2DVector; class CEngineVideoOptions;
class C3DBoundingSphere; class C3DBoundingBox; class CEnginePrimitive; class CEnginePrimitiveHandle;
class CEnginePrimitiveAnimation; class CEnginePrimitivePosition; class CEnginePrimitiveAttachmentInfo;
class C3DAnimationInfo; class CTexture; class CStaticMapGenerationVisibiilityInfo; class C2DBoxI; class CRGBColour;
class CEngineColourFilterSettings; class C2DExtentsI; class CPixelFormat;

class CInitBaseClass
{
public:
    virtual ~CInitBaseClass();                                               // slot 0
    virtual void Validate() const;                                           // slot 1
    bool Valid;
};
class CIEngine : public CInitBaseClass                   // virtual slots 2..76 (landed CEngine ctor 0x00B260B0 table)
{
public:
    virtual bool Initialise(const CEngineInit& init);                       // 2
    virtual void Release();                                                  // 3
    virtual void SetCamera(const CCamera*);                                  // 4
    virtual void SetWindow(const C2DBoxF*);                                  // 5
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool);                  // 6
    virtual void SetWorld(const CIDrawWorld*);                               // 7
    virtual void Render(const CEngineInterpolationInfo&, unsigned long);     // 8
    virtual void WorldFrameUpdate(bool);                                     // 9
    virtual void PausedWorldFrameUpdate();                                   // 10
    virtual bool ConvertScreenPosToWorldPos(const C2DVector&, C3DVector&) const;                       // 11
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float, const C2DVector&, C3DVector&) const;  // 12
    virtual bool ConvertWorldPosToScreenPos(const C3DVector&, C2DVector&) const;                       // 13
    virtual bool SetVideoOptions(const CEngineVideoOptions&);               // 14
    virtual bool PreChangeResolution();                                      // 15
    virtual bool PostChangeResolution(unsigned long, unsigned long, unsigned long);   // 16
    virtual void Refresh();                                                  // 17
    virtual bool CheckPrimitiveVisibility(unsigned __int64);                 // 18
    virtual float GetPrimitiveDrawDistanceFactor() const;                    // 19
    virtual void AddPrimitive(const CEnginePrimitive&);                      // 20
    virtual bool ClipSphere(const C3DBoundingSphere&) const;                 // 21
    virtual bool ClipBox(const C3DBoundingBox&) const;                       // 22
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long);   // 23
    virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle&, const CEnginePrimitiveAnimation&);       // 24
    virtual void SetPrimitivePosition(CEnginePrimitiveHandle&, const CEnginePrimitivePosition&);         // 25
    virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long);   // 26
    virtual void SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long);   // 27
    virtual void SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, const CEnginePrimitive&, unsigned long);   // 28
    virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&);  // 29
    virtual void ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long);                             // 30
    virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle&);                                        // 31
    virtual void SetAttachedPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, const CEnginePrimitiveAttachmentInfo&, const CEnginePrimitiveHandle&, long, unsigned long);   // 32
    virtual void CollectPrimitiveStatsForArea();                             // 33 (map/enum parameters elided: not called here)
    virtual void ProcessPrimitiveStats();                                    // 34 (parameters elided: not called here)
    virtual const C3DAnimationInfo* GetEditorAnimation(const CWideString&) const;   // 35
    virtual unsigned long AddCustomMorphTexture(CTexture&, const CCharString&) const;   // 36
    virtual long GetStaticMapUpdatePassCount();                              // 37
    virtual void StartStaticMapUpdatePass();                                 // 38 (parameters elided: not called here)
    virtual bool IsStaticMapNeedingUpdate(long, const CCharString&, unsigned long) const;   // 39
    virtual void UpdateStaticMap(long, const CCharString&, unsigned long, long);         // 40
    virtual void CloseStaticMapFile();                                       // 41
    virtual void AddMap(long, long);                                         // 42
    virtual void RemoveMap(long);                                            // 43
    virtual void SetMapAsLoaded(long);                                       // 44
    virtual void SetMapAsUnloaded(long);                                     // 45
    virtual void SetMapVisibilityAllMaps(bool);                              // 46
    virtual void SetMapVisibility(long, bool);                               // 47
    virtual void AreaChanged(const C2DBoxI&);                                // 48
    virtual void OnActivate();                                               // 49
    virtual void OnDeactivate();                                             // 50
    virtual bool IsActive() const;                                           // 51
    virtual void SetStaticMapFileForUse(const CWideString&);                 // 52
    virtual void SetLandscapeDebugTintColour(long, CRGBColour);              // 53
    virtual void ReloadLocalDetailDefs();                                    // 54
    virtual void StartNewScene(unsigned long, float);                        // 55
    virtual void CutscenePreload(bool, bool);                                // 56
    virtual void CutsceneAddCameraAngle(const CCamera&);                     // 57
    virtual void ChangeCameraAngle();                                        // 58
    virtual bool IsPreloaded() const;                                        // 59
    virtual void UpdateTextureMorphCache();                                  // 60 (parameters elided: not called here)
    virtual void SetScreenColourFilter(const CEngineColourFilterSettings&);  // 61
    virtual void SetSpotlightShadowMode(bool);                               // 62
    virtual void SetSeaBodyToRender(CCharString);                            // 63
    virtual void UpdateDebugOverlay(long, const CRGBColour*, const C2DBoxI&);   // 64
    virtual void EnableDebugOverlay(long, bool, bool);                       // 65
    virtual void SetGlobalLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&);   // 66
    virtual void Set2DPrimitiveLightingParameters(const C3DVector& direction, const CRGBFloatColour& diffuse,
                                                  const CRGBFloatColour& ambient, const CRGBFloatColour& specular);   // 67 (+0x10C)
    virtual void SetRenderTargetTexture(const CTexture*);                    // 68
    virtual CTexture* AllocateTextureFromEngineResourcePools(unsigned long, const C2DExtentsI&, long, const CPixelFormat&);   // 69
    virtual void FreeTextureFromEngineResourcePools(CTexture*);              // 70
    virtual void* GetLoadingMemoryPool();                                    // 71
    virtual void* GetAnimationMemoryPool();                                  // 72
    virtual void* GetScratchBufferTextureManager();                          // 73
    virtual bool DeallocateShadowBuffer();                                   // 74
    virtual void ReallocateShadowBuffer();                                   // 75
    virtual void Enable3DSceneRendering(bool enable);                        // 76 (+0x130)
    bool Active;
};
CIEngine* __fastcall GFAllocEngine();                    // 0x00B26340

template <class T> class CCountedPointer
{
public:
    T*    Data;
    void* Info;
    void Reset(T* object);                               // 0x0042FD04 (T = CIEngine)
    T* operator->() const { return Data; }
};

class CNewFrontendGameComponent                          // Ego_r.pdb offsets (no engine header)
{
private:
    unsigned char                     _pad00[0x10];
    CCountedPointer<CFontBank>        m_pMenuFont;              // +0x10 (CCountedPointer<const CFontBank>)
    unsigned char                     _pad18[0x40 - 0x18];
    CCountedPointer<CGraphicDataBank> m_pFrontEndGraphicBank;   // +0x40
    unsigned char                     _pad48[0x58 - 0x48];
    CCountedPointer<CIEngine>         m_pEngine;                // +0x58
    void InitialiseEngine();
};

void CNewFrontendGameComponent::InitialiseEngine()
{
    m_pEngine.Reset(GFAllocEngine());
    CDefPointer<const CEngineDef> engine_def;
    CStartupDefinitionManager::Get().GetDef("ENGINE", 1, &engine_def);

    CEngineInit init;
    init.RenderManager = GFGetSystemManager()->DisplayManager->PRenderManager;
    init.MeshBank = 0;
    init.Font = m_pMenuFont.Data;
    init.MainGraphicBank = m_pFrontEndGraphicBank.Data;
    init.GraphicsResourcePath = NResourceDirectoryNames::GetGraphicBankDir();
    init.MeshResourcePath = NResourceDirectoryNames::GetMeshBankDir();
    init.ShaderResourcePath = NResourceDirectoryNames::GetShaderBankDir();
    init.EngineCacheResourcePath = NResourceDirectoryNames::GetEngineCacheDir();
    init.DefinitionManager = &CStartupDefinitionManager::Get();
    init.WriteableEngineCacheResourcePath = NResourceDirectoryNames::GetWriteableEngineCacheDir();
    init.EngineDef = (CEngineDef*)engine_def.Object;
    init.MemoryInit = g_DefaultEngineMemoryInit;
    init.InitFlags |= g_WADBuildEnabled ? 4 : 0;
    if (g_bUseLegacyFrontend)
        init.InitFlags |= 0x20;

    m_pEngine->Initialise(init);
    m_pEngine->Enable3DSceneRendering(false);
    m_pEngine->Set2DPrimitiveLightingParameters(C3DVector(1.0f, 1.0f, 1.0f), CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f),
                                                CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f), CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f));
}
