// CEngineWaterRenderer::RenderForeground(float time) -- open a render-state capture block; fetch the environment's water-edge alpha
// map frame from graphic bank 0 (skip the pass when it is not loaded); bind the water bump map (stage 0), the environment map
// (stages 1 and 2) and the edge-alpha texture (stage 3) with their address modes; upload the foreground shader constants (swell,
// soft-edge span, four shore-lookup bone matrices, the Fresnel pixel constant) and flush them; apply the foreground vertex shader
// and index buffer; then draw every foreground water patch twice (refraction pixel shader with constant 3 = (bump,0,1,0), then the
// foreground pixel shader with (bump,0,0,1)).
// Grade: DIFFER(2531v2531) -- 6 of the 8 inlined CRenderStateManager::SetRenderState blocks (the stage 0..2 U/V address modes,
//   @0x00B764F7..0x00B76718) pick the other commutative operand order for the bookmark test/or (`test esi,eax ; or eax,esi` in
//   retail v `test eax,esi ; or esi,eax`; the stage-3 pair matches); every other instruction matches (GOTCHAS: pure SetState
//   test/or operand swaps are a known VC7.1 lane gap; mask-local / no-local / swapped-operand / `x = mask | x` spellings all
//   compile identically).
// Leaf: ?RenderForeground@CEngineWaterRenderer@@AAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B761D0, length 2531 bytes, `ret 4` (time unused).
// Name: Ego_r.pdb ?RenderForeground@CEngineWaterRenderer@@AAEXM@Z; the two private helpers it expands are Ego_r.pdb
//   ?SetForegroundTexturesAndUVModes@CEngineWaterRenderer@@AAE_NXZ (no retail copy) and
//   ?SetupForegroundShaderConstantsAndFog@CEngineWaterRenderer@@AAEXXZ (out-of-line copy 0x00B7EEC0, landed; its body is this
//   row's 0x00B7688C.. block), both `__forceinline` in the TU.
// Shape: the edge-alpha frame is CGraphicInfoFrame::GetFrame expanded with mipmap 0 / flags 0 (the landed 0x0041C260 body: its
//   TouchedFrame / mipmap tests fold away); the bank lookup is `GetGraphicInfo(index) ? GetGraphicInfo(index)->Frames : 0`
//   with the index and the bank read into locals, declared index first (declared bank first, the two global loads take
//   eax / edx instead of retail's ecx / eax); the frame touch reads the bank's CurrentFrame twice in
//   the source (`!= ... ; TouchedFrame = ...`), which is what gives retail's register choice. The frame is a CGraphicFrame local
//   (FableWin.pdb ctor / dtor / private ReleaseData, CGraphicFrameData::Release = `delete this` on the last reference): the
//   dtor's null test is dropped by VC7.1 because the stage-3 attach dereferenced FrameData first. The capture block is the RAII
//   CRenderStateCaptureBlock (StartCaptureBlock / RestoreCaptureBlock 0x00A05840).
// Callees: CRenderStateManager::RestoreCaptureBlock 0x00A05840; CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90;
//   CGraphicInfoFrame::CFrameData::BeginLoading 0x009FDE80 / CFrameData(const CGraphicInfoFrame*) 0x009FE380 (operator new
//   0x00BFEA1A, 0x44 bytes); CGraphicInfoFrame::InternalGetFrameForcedLoading 0x009FF6B0; CGraphicFrameData::Uninitialise
//   0x009FCBA0; operator delete 0x00BFE9BC; CShaderRenderManager::SetUserVertexShaderConstant 0x00989B00, std::vector<
//   CPreTransposedBoneMatrix>::resize 0x00B7EDF0, SetBoneTransform 0x0098A7F0, ApplyVertexShader 0x00988020, ApplyPixelShader
//   0x00988140 and the PrepareForRender(0x604) callees; CWaterPatchMesh::Render(long) const 0x00BEACA0; IDirect3DDevice9
//   SetTexture / SetIndices through the vtable.
// Globals: CRenderStateCaptureBlock::StateManager 0x013CB508, g_pRenderManagerCore 0x01436E18, g_pShaderRenderManager 0x01436E14,
//   g_pResourceManager 0x01436E98 (no PDB name; GraphicBanks[0] at +0x954), g_pEngineEnvironmentDef 0x01436E24 (descriptive:
//   the engine's environment def, WaterEdgeAlphaMap +0xD0), the TEXTURE_UV_MODE table 0x01396DC8 (g_textureUVModeToD3D).
// Layout debt: CResource / CResourceList / CGraphicFrame have engine headers, but their retail branches keep the list head and
//   frame counter private and CIVCountedPointeeBase's slot 1 is Delete(); the graphic-bank chain (CGraphicDataBank, CGraphicInfo,
//   CGraphicInfoFrame + CFrameData, CGraphicFrameData, CManagedTexture) has no header; these are the landed 0x0041C260 /
//   0x00BA9360 local models (vptrs as `_pad00`), and CEnvironmentDef is a one-member local projection. CWaterPatchMesh is the
//   engine header with Render(long) const added by a layout rename.
// Headers: needs the hand-back overlay (CRenderStateManager / CRenderManagerCore / CShaderRenderManager / CEngineWaterRenderer /
//   CEngineResourceManager retail branches).
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#include "engine/CEngineWaterRenderer.h"
#include "engine/CRenderManagerCore.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineResourceManager.h"
#define CWaterPatchMesh CWaterPatchMeshLayout
#include "engine/CWaterPatchMesh.h"
#undef CWaterPatchMesh
class CWaterPatchMesh : public CWaterPatchMeshLayout { public:
    void Render(long triangleCount) const;               // 0x00BEACA0 ?Render@CWaterPatchMesh@@QBEXJ@Z
};
void __cdecl operator delete(void* block);             // 0x00BFE9BC

class CResourceList;
class CResource { public:                                     // Ego_r.pdb, 0x1C bytes
    virtual void v00();
    virtual void Release();                            // slot 1: the CIVCountedPointer release on the last reference
    unsigned long  RefCount;                           // +0x04 (CIVCountedPointer count)
    CResourceList* ResourceList;                       // +0x08
    CResource*     PrevResource;                       // +0x0c
    CResource*     NextResource;                       // +0x10
    unsigned long  ResourceSize;                       // +0x14
    unsigned long  LastUsedFrame;                      // +0x18
private:
    void ListRemove();                                 // 0x009FBFB0
public:
private:
    void ListInsert(CResource*a0, CResource*a1); // 0x009FBF90
public:
    __forceinline void TouchResource();   // Ego_r.pdb ?TouchResource@CResource@@QAEXXZ
};
class CResourceList { public:                                 // Ego_r.pdb, 0x3C bytes
    unsigned long _pad00;
    CResource     Head;                                // +0x04
    unsigned long ResourceCount;                       // +0x20
    long          AllocatedMemory;                     // +0x24
    long          MaximumMemory;                       // +0x28
    unsigned long CurrentFrame;                        // +0x2c
};
__forceinline void CResource::TouchResource()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}

struct CCPInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CTexturePointer {                               // CCountedPointer<CTexture>
    CTexture* Data;
    CCPInfo* Info;
    __forceinline ~CTexturePointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CGraphicFrameData { public:                             // Ego_r.pdb, 27 bytes
    unsigned long   ReferenceCount;                    // +0x00
    void*           Data;                              // +0x04
    CTexturePointer PTexture;                          // +0x08
    unsigned short  TLX, TLY, Width, Height, Pitch;    // +0x10
    unsigned char   Alpha : 1;                         // +0x1a
    unsigned char   BooleanAlpha : 1;
    unsigned char   UsesSurface : 1;
    unsigned char   SectionOfTPage : 1;
    unsigned char   IsInitialised : 1;
    unsigned char   ScreenGraphic : 1;
    void Uninitialise();                               // 0x009FCBA0
    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    // ?Release@CGraphicFrameData@@QAEXXZ (inline): drop a reference, deleting the frame data on the last one (the deleting
    // destructor ??_GCGraphicFrameData@@QAEPAXI@Z has an out-of-line copy at 0x0041BDF0).
    __forceinline void Release()
    {
        if (--ReferenceCount == 0)
            delete this;
    }
};
class CGraphicFrame { public:                                 // intrusive pointer to CGraphicFrameData (FableWin.pdb members)
    CGraphicFrameData* FrameData;
    __forceinline CGraphicFrame() : FrameData(0) { }                     // ??0CGraphicFrame@@QAE@XZ
    __forceinline ~CGraphicFrame() { ReleaseData(); }                    // ??1CGraphicFrame@@QAE@XZ
private:
    __forceinline void ReleaseData()                                     // ?ReleaseData@CGraphicFrame@@AAEXXZ
    {
        if (FrameData) {
            FrameData->Release();
            FrameData = 0;
        }
    }
public:
    __forceinline CGraphicFrame& operator=(const CGraphicFrame& other)
    {
        if (FrameData)
            FrameData->Release();
        FrameData = other.FrameData;
        if (FrameData)
            ++FrameData->ReferenceCount;
        return *this;
    }
};
class CManagedTexture { public:                               // Ego_r.pdb, 8 bytes
    unsigned long _pad00;
    CResource*    Data;                                // +0x04 CIVCountedPointer<CManagedTextureData>
    __forceinline void TouchTexture() { if (Data) Data->TouchResource(); }   // Ego_r.pdb ?TouchTexture@CManagedTexture@@QAEXXZ
};
class CGraphicInfoFrame;
class CGraphicInfo;
class CGraphicDataBank { public:
    unsigned char _pad00[0x194];
    unsigned long CurrentFrame;                        // +0x194
    unsigned char _pad198[0x1e0 - 0x198];
    CArray<CGraphicInfo> GraphicList;                  // +0x1e0 (0x2c-byte records)
    __forceinline const CGraphicInfo* GetGraphicInfo(unsigned long index) const;
};
class CGraphicInfo { public:
    unsigned char      _pad00[0x20];
    CGraphicDataBank*  Bank;                           // +0x20
    long               Index;                          // +0x24
    CGraphicInfoFrame* Frames;                         // +0x28
};
template <class T> struct CFrameDataPointer {                             // CIVCountedPointer<CFrameData>
    T* P;
    __forceinline CFrameDataPointer& operator=(T* p)
    {
        if (P) {
            P->DecRef();
            P = 0;
        }
        P = p;
        return *this;
    }
    T* operator->() const { return P; }
    operator bool() const { return P != 0; }
};
class CGraphicInfoFrame { public: class CFrameData; public:                             // Ego_r.pdb, 16 bytes
    unsigned long     _pad00;
    unsigned short    FrameNum;                        // +0x04
    signed char       SmallMipmapLevel;                // +0x06
    CFrameDataPointer<CFrameData> Data;                            // +0x08
    CGraphicInfo*     GraphicInfo;                     // +0x0c
private:
    bool InternalGetFrameForcedLoading(unsigned long flags) const;   // 0x009FF6B0 (Ego_r.pdb ?InternalGetFrameForcedLoading@CGraphicInfoFrame@@ABE_NK@Z, private)   // 0x009FF6B0
public:
    __forceinline bool GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags);
};
class CGraphicInfoFrame::CFrameData : public CResource { public:                        // Ego_r.pdb, 0x44 bytes
    CGraphicFrame   Frame;                             // +0x1c
    CManagedTexture ManagedTexture;                    // +0x20
    CManagedTexture LoadingManagedTexture;             // +0x28
    void*           FileData[2];                       // +0x30 CCountedPointer<CBankFileAsyncData>
    signed char     CurrentMipmapLevel;                // +0x38
    signed char     LoadingMipmapLevel;                // +0x39
    signed char     TouchedMipmapLevel;                // +0x3a
    unsigned long   TouchedFrame;                      // +0x3c
    CGraphicInfoFrame* GraphicInfoFrame;               // +0x40
    CFrameData(const CGraphicInfoFrame* frame);              // 0x009FE380
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FDE80
    __forceinline void DecRef() { if (--RefCount == 0) Release(); }
};   // Ego_r.pdb: nested in CGraphicInfoFrame (RTTI .?AVCFrameData@CGraphicInfoFrame@@), 0x44 bytes
typedef CGraphicInfoFrame::CFrameData CFrameData;   // the row's free code keeps the unqualified name

__forceinline const CGraphicInfo* CGraphicDataBank::GetGraphicInfo(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}

__forceinline bool CGraphicInfoFrame::GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags)
{
    if (Data) {
        if (Data->Frame.FrameData) {
            if (!(flags & 8)) {
                if (Data->TouchedFrame != GraphicInfo->Bank->CurrentFrame || mipmapLevel <= (unsigned long)Data->TouchedMipmapLevel) {
                    Data->TouchedFrame = GraphicInfo->Bank->CurrentFrame;
                    Data->ManagedTexture.TouchTexture();
                    if (mipmapLevel <= (unsigned long)Data->CurrentMipmapLevel)
                        Data->TouchResource();
                    if ((unsigned long)Data->TouchedMipmapLevel > mipmapLevel)
                        Data->TouchedMipmapLevel = (signed char)mipmapLevel;
                }
            }
            if (mipmapLevel < (unsigned long)Data->CurrentMipmapLevel) {
                Data->BeginLoading(mipmapLevel, flags);
                if (!(flags & 0x10) || !Data->FileData[0]) {
                    out = Data->Frame;
                    return true;
                }
            } else {
                out = Data->Frame;
                return true;
            }
        }
        Data->BeginLoading(mipmapLevel, flags);
    } else {
        if (flags & 4)
            return false;
        Data = new CFrameData(this);
        Data->BeginLoading(mipmapLevel, flags);
    }
    if (!(flags & 1) || !InternalGetFrameForcedLoading(flags))
        return false;
    out = Data->Frame;
    return true;
}
// Layout: CEnvironmentDef (Ego_r.pdb; no engine header): the engine's current environment def at 0x01436E24, its
// WaterEdgeAlphaMap graphic index at retail +0xD0 (Ego_r +0xD4: the def base is 4 bytes shorter in retail).
class CEnvironmentDef { public:
    unsigned char _pad00[0xd0];
    long          WaterEdgeAlphaMap;                   // +0xd0
};
extern CRenderManagerCore* g_pRenderManagerCore;         // 0x01436E18
extern CShaderRenderManager* g_pShaderRenderManager;     // 0x01436E14
extern CEngineResourceManager* g_pResourceManager;       // 0x01436E98
extern const CEnvironmentDef* g_pEngineEnvironmentDef;   // 0x01436E24 (descriptive)

static inline float Clamp(float v, float lo, float hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

__forceinline bool CEngineWaterRenderer::SetForegroundTexturesAndUVModes()
{
    CGraphicFrame edgeAlpha;
    unsigned long index = g_pEngineEnvironmentDef->WaterEdgeAlphaMap;
    const CGraphicDataBank* bank = g_pResourceManager->GraphicBanks[0];
    CGraphicInfoFrame* frame = bank->GetGraphicInfo(index) ? bank->GetGraphicInfo(index)->Frames : 0;
    if (!frame->GetFrame(edgeAlpha, 0, 0))
        return false;
    g_pRenderManagerCore->AttachTextureToStage(0, WaterBumpMap);
    g_pRenderManagerCore->AttachTextureToStage(1, EnvironmentMap);
    g_pRenderManagerCore->AttachTextureToStage(2, EnvironmentMap);
    g_pRenderManagerCore->AttachTextureToStage(3, edgeAlpha.FrameData->PTexture.Data);
    g_pRenderManagerCore->SetTextureUVModeU(0, TEXTURE_UV_MODE_WRAP);
    g_pRenderManagerCore->SetTextureUVModeV(0, TEXTURE_UV_MODE_WRAP);
    g_pRenderManagerCore->SetTextureUVModeU(1, TEXTURE_UV_MODE_MIRROR);
    g_pRenderManagerCore->SetTextureUVModeV(1, TEXTURE_UV_MODE_CLAMP);
    g_pRenderManagerCore->SetTextureUVModeU(2, TEXTURE_UV_MODE_MIRROR);
    g_pRenderManagerCore->SetTextureUVModeV(2, TEXTURE_UV_MODE_CLAMP);
    g_pRenderManagerCore->SetTextureUVModeU(3, TEXTURE_UV_MODE_CLAMP);
    g_pRenderManagerCore->SetTextureUVModeV(3, TEXTURE_UV_MODE_WRAP);
    return true;
}

__forceinline void CEngineWaterRenderer::SetupForegroundShaderConstantsAndFog()
{
    if (Settings.EnableSwell)
        g_pShaderRenderManager->SetUserVertexShaderConstant(0, Settings.SwellFactor, Settings.ShoreSwellFactor, Settings.RefOffset, 0.0f);
    else
        g_pShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
    g_pShaderRenderManager->SetUserVertexShaderConstant(1, Settings.SoftEdgeSpanFactor, 0.0f, 0.0f, 0.0f);
    g_pShaderRenderManager->SetBoneMatrixCount(4);
    g_pShaderRenderManager->SetBoneTransform(0, Settings.OtherShaderVars);
    g_pShaderRenderManager->SetBoneTransform(1, Settings.ShoreLookupMaskMatrix);
    g_pShaderRenderManager->SetBoneTransform(2, Settings.ShoreLookupMaskMatrixForBounds);
    g_pShaderRenderManager->SetBoneTransform(3, Settings.ShoreLookupMaskMatrix2);
    float fresnel;
    if (Settings.EnableFresnel)
        fresnel = Clamp(Settings.FresnelFactor * 0.25f, 0.0f, 1.0f);
    else
        fresnel = 0.0f;
    g_pShaderRenderManager->SetPixelShaderConstant(5, C4DVector(fresnel, fresnel, fresnel, fresnel));
    g_pShaderRenderManager->PrepareForRender(0x604);
}

void CEngineWaterRenderer::RenderForeground(float time)
{
    CRenderStateCaptureBlock captureBlock;
    if (SetForegroundTexturesAndUVModes()) {
        SetupForegroundShaderConstantsAndFog();
        g_pShaderRenderManager->ApplyVertexShader(ForegroundVertexShader, false);
        g_pRenderManagerCore->SelectIndexBuffer(&ForegroundIndexBuffers, 0);
        for (CWaterPatchMesh** it = ForegroundRenderList.begin(); it != ForegroundRenderList.end(); ++it) {
            CWaterPatchMesh* mesh = *it;
            float bump = Settings.BumpMatFactor * WaterBumpFactor;
            g_pShaderRenderManager->ApplyPixelShader(RefractionForegroundPixelShader);
            g_pShaderRenderManager->SetUserVertexShaderConstant(3, bump, 0.0f, 1.0f, 0.0f);
            mesh->Render(WaterForegroundPolyCount);
            g_pShaderRenderManager->SetUserVertexShaderConstant(3, bump, 0.0f, 0.0f, 1.0f);
            g_pShaderRenderManager->ApplyPixelShader(ForegroundPixelShader);
            mesh->Render(WaterForegroundPolyCount);
        }
    }
}
