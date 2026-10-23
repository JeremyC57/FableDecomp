// CEnginePrimitiveManagerWeaponTrails::CEnginePrimitiveManagerWeaponTrails(CArray<CEngineScene*>&) -- initializes scene, shader and vertex-buffer pool; retail 00BCC5F0, 271 bytes. RTTI COL/CHD for VT 012A6AEC proves the zero-offset chain WeaponTrails -> Manager -> RenderUtil -> NonCopyable -> BaseClass; PDB signatures and retail constructor bodies establish each base constructor/dtor. The Manager vtable 012A53E8 has 13 real slots and the derived table overrides slots 0,1,2,5,7. The global at 01436E98 is CEngineResourceManager: its donor header is included under an alias, while the native shader-resource access at +0x2A28 is represented by a bounded ABI projection because donor field offsets differ. Pool vptrs 012A69DC/012A69EC are documented ABI views with three untyped native entries, not invented methods. Calls: 00BACDB0 manager ctor, 00A62FB0 shader ctor, 00BF2880 CBase ctor, 00BF3180 tree Initialise(3), 00BACD80 enable hook, 0099EBF0/0099EAE0 CCharString ctor/dtor, 00A5D5F0 by-value GetVertexShader(const CCharString&). Strings are retail literals.
#define CEngineResourceManager CEngineResourceManager_PDB
#include "engine/CEngineResourceManager.h"
#undef CEngineResourceManager
#define CVertexShader CVertexShader_PDB
#include "engine/CVertexShader.h"
#undef CVertexShader
#define CShaderDataBank CShaderDataBank_PDB
#include "engine/CShaderDataBank.h"
#undef CShaderDataBank
#include "fable_string.h"

#ifndef _MSC_VER
#define __forceinline inline
#endif

struct CEngineScene;
template<class T> struct CArray;
struct CEngineInternalPrimitiveBase;
struct CEngineSubPrimitiveBase;
struct CEngineSubPrimitiveChain;
struct CEnginePrimitiveTypeList;
struct C3DVector;
struct CNodeHeader;
struct IDirect3DVertexDeclaration9;
struct IDirect3DVertexShader9;
struct _D3DVERTEXELEMENT9;
namespace NEngineShadowRenderer { struct CShadowSceneContext; }

enum ERendered { ERenderedNativeView = 0 };
enum EEngineRenderLayer { EEngineRenderLayerNativeView = 0 };
enum EEnginePrimitiveType { EEnginePrimitiveTypeNativeView = 0 };
enum ERenderPassType { ERenderPassTypeNativeView = 0 };
enum ESubPrimitiveType { ESubPrimitiveTypeNativeView = 0 };
enum EVertexType { EVertexTypeNativeView = 0 };

// RTTI COL/CHD for vtable 012A6AEC gives the zero-offset chain
// WeaponTrails -> Manager -> RenderUtil -> NonCopyable -> BaseClass. The two
// engine PDB headers flatten that ancestry; keep its real virtual destructors
// and constructors here, with no synthetic members in these bases.
struct CBaseClass {
    CBaseClass(); // FableWin PDB 03044700; retail native base initializer 0099A2D0/E0
    virtual ~CBaseClass(); // FableWin PDB 03044720
};
struct CBaseClassNonCopyable : CBaseClass {
protected:
    CBaseClassNonCopyable(); // FableWin PDB 03044740; retail native base initializer 0099A2F0
    virtual ~CBaseClassNonCopyable(); // FableWin PDB 030447B0
};
struct CEnginePrimitiveRenderUtil : CBaseClassNonCopyable {
    CEnginePrimitiveRenderUtil(); // FableWin PDB 02DBE420; retail 00B8B5E0
    virtual ~CEnginePrimitiveRenderUtil(); // FableWin PDB 02CED260; retail vtable slot 00B8B600
};

// Manager extends the actual RenderUtil base chain and owns only the fields
// proven at +4/+8 by BACDB0; native base extent remains 0x0C.
struct CEnginePrimitiveManager : CEnginePrimitiveRenderUtil {
    virtual ~CEnginePrimitiveManager();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType) = 0;
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList&) const = 0;
    virtual void FrameStart();
    virtual void FrameEnd();
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase&, ERenderPassType);
    virtual void RenderBoundingVolume(CEngineSubPrimitiveBase&);
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&) = 0;
    virtual ERendered RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase&);
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase&, const C3DVector&, unsigned long);
    virtual void Render2DPrimitive(CEngineInternalPrimitiveBase&, CEngineSubPrimitiveChain&, ESubPrimitiveType);
    virtual void RenderNonsortedPrimitives();
    virtual void AddStaticShadowData(CEngineInternalPrimitiveBase&, NEngineShadowRenderer::CShadowSceneContext&);
    CArray<CEngineScene*>* Scenes;
    bool Enable;
    CEnginePrimitiveManager(CArray<CEngineScene*>&); // 00BACDB0
    void AddEnableConsoleVariable(const CCharString&); // 00BACD80, retail hook
};

struct CShaderBase {
protected:
    CShaderBase(); // FableWin PDB: 02C938E0
    __forceinline virtual ~CShaderBase() { } // FableWin PDB: 02C8DBF0, empty body
    virtual CCharString GetDebugName() const; // FableWin PDB: 02C8DC10
};

struct CVertexShaderDataTestFactory;
struct CVertexShaderData {
    virtual ~CVertexShaderData(); // FableWin PDB: 032F39A0
    IDirect3DVertexDeclaration9* VertexDeclaration; // +04
    IDirect3DVertexShader9* VertexShader; // +08
    const _D3DVERTEXELEMENT9* FormatDecl; // +0C
    unsigned long ReferenceCount; // +10
    void ReleaseReference() { if (--ReferenceCount == 0) delete this; }
private:
    CVertexShaderData(IDirect3DVertexDeclaration9*, IDirect3DVertexShader9*,
        const _D3DVERTEXELEMENT9*, const CCharString&, const CCharString&); // genuine private FableWin PDB ctor 032F3880
    friend struct CVertexShaderDataTestFactory;
};

// CVertexShader.h is a flattened donor record; retail RTTI/vtable plus PDB
// signatures prove the real CShaderBase -> CVertexShader virtual hierarchy.
struct CVertexShader : CShaderBase {
    CVertexShaderData* Data;
    CVertexShader(); // retail helper 00A62FB0; FableWin PDB 032F34F0
    CVertexShader(const CVertexShader& other) : CShaderBase(other), Data(other.Data) {
        if (Data) ++Data->ReferenceCount;
    }
    CVertexShader& operator=(const CVertexShader& other) {
        if (Data) Data->ReleaseReference();
        Data = other.Data;
        ++Data->ReferenceCount;
        return *this;
    }
    __forceinline virtual ~CVertexShader() { if (Data) Data->ReleaseReference(); }
    virtual CCharString GetDebugName() const; // PDB: 032F37E0
    virtual unsigned long CountReferences(); // PDB: 009AD1C0
};

struct CShaderResourceTestFactory;
class CShaderResource {
public:
    virtual ~CShaderResource();
    const CShaderDataBank_PDB* ShaderBank; // +04
    unsigned char NativeShaderArray[0x10]; // +08, retail CArray<CShaderBase*>
    unsigned char NativeVertexShaderInput[0x1c]; // +18, donor input is 0x24; retail setters begin at +34
    const _D3DVERTEXELEMENT9* VertexShaderDeclaration; // +34
    EVertexType VertexFormat; // +38
    CVertexShader* GetVertexShader(CVertexShader* result, const CCharString&) const; // ABI view of FableWin PDB by-value method 032EA750; hidden result, retail helper 00A5D5F0
private:
    CShaderResource(); // genuine private FableWin PDB default ctor 032E99A0
    friend struct CShaderResourceTestFactory;
};

namespace N_00bf2880 { struct CBase {
    CNodeHeader* BaseNode;
    CBase(); // 00BF2880
}; } using N_00bf2880::CBase;
struct CEngineTreeFilerBase : CBase {
    long KeySize;
    void Initialise(long); // 00BF3180
};

extern char CEngineDynamic3DResourceManagerNativeVTable[]; // 012A69DC, three audited native slots
extern char CEngineVertexBufferPoolNativeVTable[]; // 012A69EC, three audited native slots

// Native ABI projection from the donor's CEngineDynamic3DResourceManager<...>
// layout: AllocCount/+4, FreePoolMem/+8, TotalMemAllocated/+C, tree filer/+10.
// The actual three vtable targets are known but their signatures are not in PDB;
// the explicit vptr is therefore data, not invented C++ virtual methods.
struct CEngineDynamic3DResourceManagerNativeView {
    void* NativeVptr;
    long AllocCount;
    long FreePoolMem;
    long TotalMemAllocated;
    CEngineTreeFilerBase ResourceMappingTree;
    __forceinline CEngineDynamic3DResourceManagerNativeView()
        : NativeVptr(CEngineDynamic3DResourceManagerNativeVTable), ResourceMappingTree() {
        AllocCount = 0;
        FreePoolMem = 0;
        TotalMemAllocated = 0;
        ResourceMappingTree.Initialise(3);
    }
};
struct CEngineVertexBufferPoolNativeView : CEngineDynamic3DResourceManagerNativeView {
    long SizeGranularity;
    __forceinline CEngineVertexBufferPoolNativeView()
        : CEngineDynamic3DResourceManagerNativeView() {
        NativeVptr = CEngineVertexBufferPoolNativeVTable;
        SizeGranularity = 0x400;
    }
};

struct CEnginePrimitiveManagerWeaponTrails : CEnginePrimitiveManager {
    CVertexShader VertexShaderZSortedUnlit; // +0x0C, PDB layout
    CEngineVertexBufferPoolNativeView VertexBufferPool; // +0x14, PDB layout / native vptr audit
    CEnginePrimitiveManagerWeaponTrails(CArray<CEngineScene*>&);
    virtual ~CEnginePrimitiveManagerWeaponTrails();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList&) const;
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase&, ERenderPassType);
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&);
};

// Retail callers identify 01436E98 as CEngineResourceManager and load its
// shader-resource pointer at +0x2A28. The included donor manager header is a
// distinct build layout (its ShaderResourcePixel field is at +0x39CC), so this
// view exposes only the proven native prefix and access. Its bytes stand for
// pre-existing engine state; it is not constructed as a C++ manager object.
struct CEngineResourceManagerRetailProjection {
    unsigned char NativeResourceManagerState[0x2a28];
    CShaderResource* ShaderResourceAt2A28;
};
extern CEngineResourceManagerRetailProjection* g_pEngineData; // 01436E98

CEnginePrimitiveManagerWeaponTrails::CEnginePrimitiveManagerWeaponTrails(
    CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManager(scenes), VertexShaderZSortedUnlit(), VertexBufferPool()
{
    {
        CCharString name("EnableWeaponTrails", -1);
        AddEnableConsoleVariable(name);
    }
    {
        CCharString name("VSHADER_TRANSFORM_COL_TEX0", -1);
        union ShaderReturnStorage {
            unsigned char Bytes[sizeof(CVertexShader)];
            void* Alignment;
        } result;
        CVertexShader* temporary = reinterpret_cast<CVertexShader*>(result.Bytes);
        CEngineResourceManagerRetailProjection* resourceManager = g_pEngineData;
        CShaderResource* resource = resourceManager->ShaderResourceAt2A28;
        CVertexShader* shader = resource->GetVertexShader(temporary, name);
        VertexShaderZSortedUnlit = *shader;
        temporary->CVertexShader::~CVertexShader();
    }
}

#ifdef _MSC_VER
typedef char CShaderBaseNativeSize[(sizeof(CShaderBase) == 4) ? 1 : -1];
typedef char CVertexShaderNativeSize[(sizeof(CVertexShader) == 8) ? 1 : -1];
typedef char CVertexShaderDataNativeSize[(sizeof(CVertexShaderData) == 0x14) ? 1 : -1];
typedef char CEngineTreeFilerBaseNativeSize[(sizeof(CEngineTreeFilerBase) == 8) ? 1 : -1];
typedef char CEngineDynamicManagerNativeSize[(sizeof(CEngineDynamic3DResourceManagerNativeView) == 0x18) ? 1 : -1];
typedef char CEngineVertexBufferPoolNativeSize[(sizeof(CEngineVertexBufferPoolNativeView) == 0x1c) ? 1 : -1];
typedef char CEnginePrimitiveManagerBaseNativeSize[(sizeof(CEnginePrimitiveManager) == 0x0c) ? 1 : -1];
typedef char CBaseClassNativeSize[(sizeof(CBaseClass) == 4) ? 1 : -1];
typedef char CBaseClassNonCopyableNativeSize[(sizeof(CBaseClassNonCopyable) == 4) ? 1 : -1];
typedef char CEnginePrimitiveRenderUtilNativeSize[(sizeof(CEnginePrimitiveRenderUtil) == 4) ? 1 : -1];
typedef char CEnginePrimitiveManagerWeaponTrailsNativeSize[(sizeof(CEnginePrimitiveManagerWeaponTrails) == 0x30) ? 1 : -1];
typedef char CEngineResourceManagerNativeProjectionOffset[(offsetof(CEngineResourceManagerRetailProjection, ShaderResourceAt2A28) == 0x2a28) ? 1 : -1];
#endif