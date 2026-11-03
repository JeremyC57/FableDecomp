#ifdef FABLE_RETAIL_CENGINERESOURCEMANAGER
#ifndef FABLE_ENGINE_CENGINERESOURCEMANAGER_H
#define FABLE_ENGINE_CENGINERESOURCEMANAGER_H
// Retail branch (pass 12 header wave, 0x00B3BE10): RTTI CEngineResourceManager : CEngineComponent, CAsyncLoadingProcessCallback
// (the dtor 0x00B3B8E0 stores 0x012A1B3C at +0 and 0x012A1B34 at +0x0C). Members in Ego_r TPI order; retail sizes from the
// dtor: VShaderBanks stride 0x128 (not the donor's 0x1A8 or the default's 0x13C), ExternalTexturesAllocatedFromEnginePools
// +0x31D4 (the map the dtor clears), sizeof 0x31E0 (the generated default below says 0x3410). Unused composites stay opaque.
// BuildAlphaStippleTexture: Ego_r / FableWin ?BuildAlphaStippleTexture@CEngineResourceManager@@AAEXJAAV?$CArray@E@@@Z (private).
#include "rebuild_abi.h"
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include "engine/CEngineComponent.h"
#include "engine/CAsyncLoadingProcessCallback.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
class CMeshDataBank; class CGraphicDataBank; class CFontBank;
class CShaderResource; class CGPUScratchBufferTextureManager;
enum EShaderBankNo;                                          // MS forward enums (chunk 723; no enumerator name recovered)
enum EIndexBufferPool;
class CEngineResourceManager : public CEngineComponent, public CAsyncLoadingProcessCallback {
public:
    virtual ~CEngineResourceManager();
    virtual void ProcessLoading();
    virtual unsigned long PeekSceneFilterFlags() const;
    // Chunk 723 (rows 0x00BC14E0 / 0x00B71880; folded into this branch at landing): FableWin.pdb inlines
    // ?GetVShaderResource@CEngineResourceManager@@QBEPBVCShaderResource@@W4EShaderBankNo@@@Z (0x00BC14E0 reads bank 21 at +0x2A68),
    // ?GetPShaderResource@CEngineResourceManager@@QBEPBVCShaderResource@@XZ (+0x2A98) and the scratch-manager member read
    // (+0x3190: 0x00B71880 / 0x00B6D630 / 0x00B23F60); Ego_r.pdb / FableWin.pdb out-of-line GetIndexBufferMemPool 0x00B3B0A0,
    // GetIndexBufferUsage 0x00B3B060, GetVertexBufferUsage 0x00B3B030.
    EIndexBufferPool GetIndexBufferMemPool();                            // 0x00B3B0A0
    unsigned long GetIndexBufferUsage();                                 // 0x00B3B060
    unsigned long GetVertexBufferUsage();                                // 0x00B3B030
    const CShaderResource* GetVShaderResource(EShaderBankNo bank) const { return VShaderResources[bank].Get(); }
    const CShaderResource* GetPShaderResource() const { return ShaderResourcePixel.Get(); }
    CGPUScratchBufferTextureManager& GetGPUScratchBufferTextureManager()
    {
        return *(CGPUScratchBufferTextureManager*)GPUScratchBufferTextureManager;
    }
private:
    void BuildAlphaStippleTexture(long,CArray<unsigned char>&);
    CMeshDataBank* MeshBanks[3];                             // +0x10
    CMeshDataBank* MainMeshBank;                             // +0x1C
    unsigned char EditorMeshBank[0x460];                     // +0x20 CMeshDataBank
    unsigned char EngineMeshBank[0x460];                     // +0x480 CMeshDataBank
    unsigned char LoadingMemoryPool[8];                      // CCountedPointer<CMovableResourceMemoryPool>
    unsigned char LoadingMemoryFailureHandler[8];            // CCountedPointer<CLoadingMemoryFailureHandler>
    unsigned char AnimationMemoryPool[8],ClothMemoryPool[8],MeshDataMemoryPool[8];   // CCountedPointer<CMovableResourceMemoryPool>
    unsigned char AnimationManager[0x4c];                    // C3DAnimationManager
    CGraphicDataBank* GraphicBanks[1];
    CGraphicDataBank* MainGraphicBank;
    const CFontBank* EngineFont;
    unsigned char VShaderBanks[27*0x128],ShaderBankPixel[0x128];   // CShaderDataBank
    CCountedPointer<CShaderResource> VShaderResources[27];  // +0x29C0 (chunk 723: 0x00BC14E0 bank 21 at +0x2A68)
    CCountedPointer<CShaderResource> ShaderResourcePixel;    // +0x2A98
    double MaxLoadingTime;
    unsigned long VertexBufferUsageFlags;
    unsigned char StipleFadeTexture[8];                      // CTexture
    CArray<unsigned long> HWInverseStipplePatterns;
    unsigned char DitherTextureList[12];                     // CArray<CTexture>
    unsigned char EngineCachePath[4],WriteableEngineCachePath[4];  // CWideString
    unsigned char ZeroRGBATexture[8],SatRGBATexture[8];      // CTexture
    unsigned char VSConstantLayoutBasic[0x14c];              // CEngineVSConstantLayoutBasic
    unsigned char VSConstantLayoutLights[0x14c];             // CEngineVSConstantLayoutLights
    unsigned char VSConstantLayoutLightsBones[0x14c];        // CEngineVSConstantLayoutLightsBones
    unsigned char VSConstantLayoutRepeatedMesh[0x17c];       // CEngineVSConstantLayoutRepeatedMesh
    unsigned char VSConstantLayoutParticles[0x14c];          // CEngineVSConstantLayoutParticles
    unsigned char GPUScratchBufferTextureManager[0x44];      // CGPUScratchBufferTextureManager
    unsigned char ExternalTexturesAllocatedFromEnginePools[12];   // +0x31D4 map<CTexture*,CExternalTextureAllocationInfo>
};
FABLE_STATIC_ASSERT(sizeof(CEngineResourceManager)==0x31e0);
#endif // FABLE_ENGINE_CENGINERESOURCEMANAGER_H
#else // generated flattened default (unchanged)
#ifndef FABLE_ENGINE_CENGINERESOURCEMANAGER_H
#define FABLE_ENGINE_CENGINERESOURCEMANAGER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineResourceManager = 0x4150.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineResourceManager = 0x3410.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCPPointerInfo;
class CFontBank;
class CGraphicDataBank;
class CLoadingMemoryFailureHandler;
class CMeshDataBank;
class CMovableResourceMemoryPool;
class CWideStringData;
struct IDirect3DTexture9;

#pragma pack(push, 1)
class CEngineResourceManager {
public:
    void*                         __vftable;                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char                 _pad_0x04[0x4];            // +0x04
    unsigned char                 _pad_0x08[0x4];            // +0x08
    unsigned char                 _pad_0x0c[0x4];            // +0x0c
    CMeshDataBank*                MeshBanks[3];              // +0x10
    CMeshDataBank*                MainMeshBank;              // +0x1c
    unsigned char                 EditorMeshBank[0x460];     // +0x20 CMeshDataBank
    unsigned char                 EngineMeshBank[0x460];     // +0x480 CMeshDataBank
    CMovableResourceMemoryPool*   LoadingMemoryPool_Data;    // +0x8e0 CCountedPointer<CMovableResourceMemoryPool>::Data
    CCPPointerInfo*               LoadingMemoryPool_Info;    // +0x8e4 CCountedPointer<CMovableResourceMemoryPool>::Info
    CLoadingMemoryFailureHandler* LoadingMemoryFailureHandler_Data; // +0x8e8 CCountedPointer<CLoadingMemoryFailureHandler>::Data
    CCPPointerInfo*               LoadingMemoryFailureHandler_Info; // +0x8ec CCountedPointer<CLoadingMemoryFailureHandler>::Info
    CMovableResourceMemoryPool*   AnimationMemoryPool_Data;  // +0x8f0 CCountedPointer<CMovableResourceMemoryPool>::Data
    CCPPointerInfo*               AnimationMemoryPool_Info;  // +0x8f4 CCountedPointer<CMovableResourceMemoryPool>::Info
    CMovableResourceMemoryPool*   ClothMemoryPool_Data;      // +0x8f8 CCountedPointer<CMovableResourceMemoryPool>::Data
    CCPPointerInfo*               ClothMemoryPool_Info;      // +0x8fc CCountedPointer<CMovableResourceMemoryPool>::Info
    CMovableResourceMemoryPool*   MeshDataMemoryPool_Data;   // +0x900 CCountedPointer<CMovableResourceMemoryPool>::Data
    CCPPointerInfo*               MeshDataMemoryPool_Info;   // +0x904 CCountedPointer<CMovableResourceMemoryPool>::Info
    unsigned char                 AnimationManager[0x4c];    // +0x908 C3DAnimationManager
    CGraphicDataBank*             GraphicBanks[1];           // +0x954
    CGraphicDataBank*             MainGraphicBank;           // +0x958
    CFontBank*                    EngineFont;                // +0x95c
    unsigned char                 VShaderBanks[0x2154];      // +0x960 CShaderDataBank[27]
    unsigned char                 ShaderBankPixel[0x13c];    // +0x2ab4 CShaderDataBank
    unsigned char                 VShaderResources[0xd8];    // +0x2bf0 CCountedPointer<CShaderResource>[27]
    unsigned char                 ShaderResourcePixel[0x8];  // +0x2cc8 CCountedPointer<CShaderResource>
    double                        MaxLoadingTime;            // +0x2cd0
    unsigned long                 VertexBufferUsageFlags;    // +0x2cd8
    unsigned char                 StipleFadeTexture[0x8];    // +0x2cdc CTexture
    unsigned char                 HWInverseStipplePatterns[0xc];  // +0x2ce4 CArray<unsigned_long>
    unsigned char                 DitherTextureList[0xc];    // +0x2cf0 CArray<CTexture>
    CWideStringData*              EngineCachePath_PStringData; // +0x2cfc CWideString::PStringData
    CWideStringData*              WriteableEngineCachePath_PStringData; // +0x2d00 CWideString::PStringData
    unsigned char                 ZeroRGBATexture[0x8];      // +0x2d04 CTexture
    unsigned char                 SatRGBATexture[0x8];       // +0x2d0c CTexture
    unsigned char                 VSConstantLayoutBasic[0x14c]; // +0x2d14 CEngineVSConstantLayoutBasic
    unsigned char                 VSConstantLayoutLights[0x14c]; // +0x2e60 CEngineVSConstantLayoutLights
    unsigned char                 VSConstantLayoutLightsBones[0x14c]; // +0x2fac CEngineVSConstantLayoutLightsBones
    unsigned char                 VSConstantLayoutRepeatedMesh[0x17c]; // +0x30f8 CEngineVSConstantLayoutRepeatedMesh
    unsigned char                 VSConstantLayoutParticles[0x14c]; // +0x3274 CEngineVSConstantLayoutParticles
    unsigned char                 GPUScratchBufferTextureManager[0x44]; // +0x33c0 CGPUScratchBufferTextureManager
    unsigned char                 ExternalTexturesAllocatedFromEnginePools[0xc];  // +0x3404 map<CTexture_*,CEngineResourceManager::CExternalTextureAllocationInfo,std::less<CTexture_*>,std::allocator<std::pair<CTexture_*_const,CEngineResourceManager::CExternalTextureAllocationInfo>_>_>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineResourceManager) == 0x3410);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, MeshBanks) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, MainMeshBank) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, EditorMeshBank) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, EngineMeshBank) == 0x480);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, LoadingMemoryPool_Data) == 0x8e0);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, LoadingMemoryPool_Info) == 0x8e4);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, LoadingMemoryFailureHandler_Data) == 0x8e8);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, LoadingMemoryFailureHandler_Info) == 0x8ec);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, AnimationMemoryPool_Data) == 0x8f0);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, AnimationMemoryPool_Info) == 0x8f4);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, ClothMemoryPool_Data) == 0x8f8);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, ClothMemoryPool_Info) == 0x8fc);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, MeshDataMemoryPool_Data) == 0x900);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, MeshDataMemoryPool_Info) == 0x904);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, AnimationManager) == 0x908);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, GraphicBanks) == 0x954);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, MainGraphicBank) == 0x958);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, EngineFont) == 0x95c);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, VShaderBanks) == 0x960);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, ShaderBankPixel) == 0x2ab4);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, VShaderResources) == 0x2bf0);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, ShaderResourcePixel) == 0x2cc8);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, MaxLoadingTime) == 0x2cd0);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, VertexBufferUsageFlags) == 0x2cd8);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, StipleFadeTexture) == 0x2cdc);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, HWInverseStipplePatterns) == 0x2ce4);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, DitherTextureList) == 0x2cf0);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, EngineCachePath_PStringData) == 0x2cfc);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, WriteableEngineCachePath_PStringData) == 0x2d00);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, ZeroRGBATexture) == 0x2d04);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, SatRGBATexture) == 0x2d0c);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, VSConstantLayoutBasic) == 0x2d14);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, VSConstantLayoutLights) == 0x2e60);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, VSConstantLayoutLightsBones) == 0x2fac);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, VSConstantLayoutRepeatedMesh) == 0x30f8);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, VSConstantLayoutParticles) == 0x3274);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, GPUScratchBufferTextureManager) == 0x33c0);
FABLE_STATIC_ASSERT(offsetof(CEngineResourceManager, ExternalTexturesAllocatedFromEnginePools) == 0x3404);

#endif // FABLE_ENGINE_CENGINERESOURCEMANAGER_H
#endif // FABLE_RETAIL_CENGINERESOURCEMANAGER
