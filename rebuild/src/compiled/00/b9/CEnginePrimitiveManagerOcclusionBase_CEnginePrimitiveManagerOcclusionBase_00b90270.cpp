// CEnginePrimitiveManagerOcclusionBase::CEnginePrimitiveManagerOcclusionBase(CArray<CEngineScene*>&) -- initialize the occlusion manager, shader, unit-box counted pointer and silhouette threshold; retail 00B90270, 274 bytes. Native RTTI COL/CHD for VT 012A3E14 proves the zero-offset chain OcclusionBase -> CEnginePrimitiveManager -> CEnginePrimitiveRenderUtil -> CBaseClassNonCopyable -> CBaseClass. PDB names the constructor and fields VertexShader +0x0c, UnitBox +0x14, SilhouetteTreshold +0x1c. The donor CEngineResourceManager header is retained under an alias; retail reads its shader resource at +0x29c8, while the donor layout differs. CMeshDataBank's donor MeshInfoBuffer is +0x404, but retail helper 00A24280 reads the native array at +0x380; the helper call view below records only that observed ABI. 00A5D5F0 returns CVertexShader by hidden result; 009CCDC0 is CBankFile::FindIndexBySymbol (confirmed exact-address source and PDB identity); 00A25E70 is C3DMeshInfo::GetMesh (PDB signature). The two CCharString values are retail literals.
// Leaf: CEnginePrimitiveManagerOcclusionBase
#define CEngineResourceManager CEngineResourceManager_PDB
#include "engine/CEngineResourceManager.h"
#undef CEngineResourceManager
#include "engine/CMeshDataBank.h"
#define CBankFile CBankFile_PDB
#include "engine/CBankFile.h"
#undef CBankFile
#define C3DMeshInfo C3DMeshInfo_PDB
#include "engine/C3DMeshInfo.h"
#undef C3DMeshInfo
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
struct C3DMesh2;
namespace NEngineShadowRenderer { struct CShadowSceneContext; }

enum ERendered { ERenderedNativeView = 0 };
enum EEngineRenderLayer { EEngineRenderLayerNativeView = 0 };
enum EEnginePrimitiveType { EEnginePrimitiveTypeNativeView = 0 };
enum ERenderPassType { ERenderPassTypeNativeView = 0 };
enum ESubPrimitiveType { ESubPrimitiveTypeNativeView = 0 };

struct CBaseClass {
    CBaseClass(); // native 0099A2D0
    virtual ~CBaseClass();
};
struct CBaseClassNonCopyable : CBaseClass {
protected:
    CBaseClassNonCopyable(); // native 0099A2F0
    virtual ~CBaseClassNonCopyable();
};
struct CEnginePrimitiveRenderUtil : CBaseClassNonCopyable {
    CEnginePrimitiveRenderUtil(); // native 00B8B5E0
    virtual ~CEnginePrimitiveRenderUtil();
};
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
    CEnginePrimitiveManager(CArray<CEngineScene*>&); // native 00BACDB0
};

struct CShaderBase {
protected:
    CShaderBase();
    __forceinline virtual ~CShaderBase() { }
    virtual CCharString GetDebugName() const;
};
struct CVertexShaderData {
    virtual ~CVertexShaderData();
    void* VertexDeclaration;
    void* VertexShader;
    const void* FormatDecl;
    unsigned long ReferenceCount;
    void ReleaseReference() { if (--ReferenceCount == 0) delete this; }
};
struct CVertexShader : CShaderBase {
    CVertexShaderData* Data;
    CVertexShader(); // native 00A62FB0
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
    virtual CCharString GetDebugName() const;
    virtual unsigned long CountReferences();
};
struct CShaderResourceCallView {
    CVertexShader* GetVertexShader(CVertexShader* result, const CCharString&) const; // hidden result, native 00A5D5F0
};
struct CEngineResourceManagerRetailAccess {
    static __forceinline CShaderResourceCallView* ShaderResourceAt29C8(CEngineResourceManager_PDB* engine) {
        return *reinterpret_cast<CShaderResourceCallView**>(reinterpret_cast<unsigned char*>(engine) + 0x29c8);
    }
};
extern CEngineResourceManager_PDB* g_pEngineData; // native global 01436E98

// Native CCPPointerInfo evidence in the landed renderer and surface-composition
// sources: 32-bit RefCount at +0, __fastcall DeleteFunc at +4, Data at +8.
struct CCPPointerInfo {
    unsigned int RefCount;
    void (FABLE_FASTCALL *DeleteFunc)(void*);
    void* Data;
};
#ifdef _MSC_VER
typedef char CCPPointerInfoNativeSize[(sizeof(CCPPointerInfo) == 0x0c) ? 1 : -1];
#endif
template<class T> struct CCountedPointerRetail {
    T* Object;
    CCPPointerInfo* Info;
    CCountedPointerRetail() : Object(0), Info(0) { }
    CCountedPointerRetail(const CCountedPointerRetail& other)
        : Object(other.Object), Info(other.Info) {
        if (Info) ++Info->RefCount;
    }
    CCountedPointerRetail& operator=(const CCountedPointerRetail& other) {
        if (this != &other) {
            Release();
            Object = other.Object;
            Info = other.Info;
            if (Info) ++Info->RefCount;
        }
        return *this;
    }
    ~CCountedPointerRetail() { Release(); }
private:
    void Release() {
        if (Info) {
            CCPPointerInfo* info = Info;
            Object = 0;
            Info = 0;
            if (--info->RefCount == 0) {
                info->DeleteFunc(info->Data);
                delete info;
            }
        }
    }
};
struct CBankFileCallView : CBankFile_PDB {
    unsigned long FindIndexBySymbol(const CCharString&) const; // native 009CCDC0; inherited at offset zero by CMeshDataBank
};
struct CMeshDataBankCallView {
    C3DMeshInfo_PDB* MeshInfoAtIndex(unsigned long); // native 00A24280 reads the pointer array at +0x380
};
struct C3DMeshInfoCallView : C3DMeshInfo_PDB {
    bool GetMesh(CCountedPointerRetail<const C3DMesh2>&, unsigned long, unsigned long) const; // PDB method 00A25E70
};

struct CEnginePrimitiveManagerOcclusionBase : CEnginePrimitiveManager {
    CVertexShader VertexShader; // +0x0c, PDB
    CCountedPointerRetail<const C3DMesh2> UnitBox; // +0x14, PDB
    float SilhouetteTreshold; // +0x1c, PDB spelling
    CEnginePrimitiveManagerOcclusionBase(CArray<CEngineScene*>&);
    virtual ~CEnginePrimitiveManagerOcclusionBase();
    virtual void RenderBoundingVolume(CEngineSubPrimitiveBase&);
};

CEnginePrimitiveManagerOcclusionBase::CEnginePrimitiveManagerOcclusionBase(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManager(scenes), VertexShader(), UnitBox()
{
    {
        CCharString name("VSHADER_STATIC_UNLIT", -1);
        union ShaderReturnStorage {
            unsigned char Bytes[sizeof(CVertexShader)];
            void* Alignment;
        } result;
        CVertexShader* temporary = reinterpret_cast<CVertexShader*>(result.Bytes);
        CShaderResourceCallView* resource = CEngineResourceManagerRetailAccess::ShaderResourceAt29C8(g_pEngineData);
        CVertexShader* shader = resource->GetVertexShader(temporary, name);
        VertexShader = *shader;
        temporary->CVertexShader::~CVertexShader();
    }

    CMeshDataBank* meshBank = g_pEngineData->MeshBanks[2];
    if (meshBank) {
        CBankFileCallView* bankFile = reinterpret_cast<CBankFileCallView*>(meshBank);
        unsigned long meshIndex;
        {
            CCharString name("MESH_ENGINE_UNIT_BOX", -1);
            meshIndex = bankFile->FindIndexBySymbol(name);
        }
        if (meshIndex)
            reinterpret_cast<C3DMeshInfoCallView*>(reinterpret_cast<CMeshDataBankCallView*>(meshBank)->MeshInfoAtIndex(meshIndex))->GetMesh(UnitBox, 0, 1);
    }
    SilhouetteTreshold = 0.1f;
}

#ifdef _MSC_VER
typedef char CEnginePrimitiveManagerNativeSize[(sizeof(CEnginePrimitiveManager) == 0x0c) ? 1 : -1];
typedef char CVertexShaderNativeSize[(sizeof(CVertexShader) == 8) ? 1 : -1];
typedef char CEnginePrimitiveManagerOcclusionBaseNativeSize[(sizeof(CEnginePrimitiveManagerOcclusionBase) == 0x20) ? 1 : -1];
#endif