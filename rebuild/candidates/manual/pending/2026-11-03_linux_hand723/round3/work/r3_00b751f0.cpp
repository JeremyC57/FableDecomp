// CEngineWaterRenderer::RenderIceForeground(float time) -- open a render-state capture block; read the shadow renderer's
// technique; fetch the environment's ice texture frame from graphic bank 0 (skip the pass when it is not loaded); bind the ice
// bump map (stage 0), the environment map (stage 1) and the ice texture (stage 2) with their address modes and, when shadows are
// on, the shadow-function states (stage 3, depth-pack parameters {4, 5, 3}); upload the ice foreground constants and flush them;
// update the global lighting states, enable specular and select the foreground index buffer; then for every ice patch fetch
// the (up to 3) local lights around its bounding box, set them up as shader point lights, apply the technique's ice shaders
// for that light count, flush and render the patch; finally disable the point lights.
// Grade: DIFFER(2722v2722) -- 6 of the 7 inlined CRenderStateManager::SetRenderState blocks (the six U/V address modes,
//   @0x00B754D5..0x00B756F6) pick the other commutative operand order for the bookmark test/or (retail `test esi,eax ; or
//   eax,esi`, built `test eax,esi ; or esi,eax`); every other instruction matches (lane gap, see engine/CRenderStateManager.h).
// Leaf: ?RenderIceForeground@CEngineWaterRenderer@@AAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B751F0, length 2722 bytes, `ret 4` (time unused).
// Name: Ego_r.pdb ?RenderIceForeground@CEngineWaterRenderer@@AAEXM@Z; the private helpers it expands are Ego_r.pdb
//   ?SetIceForegroundTexturesAndUVModes@CEngineWaterRenderer@@AAE_N_N@Z (no retail copy) and
//   ?SetupIceForegroundShaderConstantsAndFog@CEngineWaterRenderer@@AAEXXZ (out-of-line copy 0x00B7F3B0, landed; same body),
//   both `__forceinline` in the TU.
// Shape: the technique is read once before the frame lookup and passed as `technique != NONE`; the shadow parameters are an
//   aggregate-initialised CEngineShadowRenderer::CColorBufferDepthPackParameters; the ice frame is a CGraphicFrame local (its
//   release loses the null test after the stage-2 GetTexture() dereference); per patch the bounding box and the mesh are copied
//   into locals first, in that order, then the 3-byte theme-channel desc is zeroed (that order puts the mesh store above the
//   byte stores and the box at +0x24 with the mesh at +0x20, as retail); the vertex shader comes from the technique's
//   GetIceForeground*VertexShader(lights->LightCount) (0x00B6DF50 / 0x00B6DF90 / 0x00B6DFD0) after its pixel shader.
// Callees: CRenderStateManager::RestoreCaptureBlock 0x00A05840; CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90;
//   CGraphicInfoFrame::CFrameData::BeginLoading 0x009FDE80 / CFrameData(const CGraphicInfoFrame*) 0x009FE380 (operator new
//   0x00BFEA1A); CGraphicInfoFrame::InternalGetFrameForcedLoading 0x009FF6B0; CGraphicFrameData::Uninitialise 0x009FCBA0;
//   operator delete 0x00BFE9BC; CEngineShadowRenderer::SetupShadowFuncRenderStates 0x00B50510; CEngineLightingManager::
//   UpdateGlobalLightingStates 0x00B46C80, GetLightsInArea 0x00B46280, SetupShaderPointLights 0x00B44F20, DisablePointLights
//   0x00B44D50; CShaderRenderManager::SetUserVertexShaderConstant 0x00989B00, std::vector<CPreTransposedBoneMatrix>::resize
//   0x00B7EDF0, SetBoneTransform 0x0098A7F0, ApplyVertexShader 0x00988020, ApplyPixelShader 0x00988140 and the
//   PrepareForRender(0x614 / 0x14) callees (incl. UpdateWorldSpaceLighting 0x0098A540); CWaterPatchMesh::Render(long) const
//   0x00BEACA0; IDirect3DDevice9 SetTexture / SetIndices through the vtable.
// Globals: CRenderStateCaptureBlock::StateManager 0x013CB508, g_pRenderManagerCore 0x01436E18, g_pShaderRenderManager 0x01436E14,
//   g_pResourceManager 0x01436E98 (no PDB name), g_pEngineEnvironmentDef 0x01436E24 (descriptive; IceTexture +0xCC in retail),
//   g_pEngineLightingManager 0x01436E9C, g_pShadowRenderer_01436e60 0x01436E60, the TEXTURE_UV_MODE table 0x01396DC8; float pool 0.5f
//   0x0122F59C, 0.25f 0x0123078C, 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout debt: as 0x00B761D0 (graphic-bank chain and resource classes are the landed local models); CEnvironmentDef is a
//   one-member projection; CEngineLocalLightArray / CEnginePrimitiveThemeChannelDesc come from their engine headers (chunk 723c: they were
//   local stand-ins in round 2; the light elements stay opaque).
// Headers: needs the hand-back overlay (CRenderStateManager / CRenderManagerCore / CShaderRenderManager / CEngineWaterRenderer /
//   CEngineResourceManager / CEngineLightingManager / CEngineShadowRenderer retail branches).
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_C3DBOUNDINGBOX
#include "engine/CEngineWaterRenderer.h"
#include "engine/CRenderManagerCore.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineLocalLightArray.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/C3DBoundingBox.h"
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
    __forceinline void AddReference() { ++ReferenceCount; }               // ?AddReference@CGraphicFrameData@@QAEXXZ
    __forceinline CTexture* GetTexture() const { return PTexture.Data; }   // ?GetTexture@CGraphicFrameData@@QBEPAVCTexture@@XZ
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
    __forceinline const CGraphicFrame& operator=(const CGraphicFrame& other)   // ??4CGraphicFrame@@QAEABV0@ABV0@@Z
    {
        ReleaseData();
        FrameData = other.FrameData;
        if (FrameData)
            FrameData->AddReference();
        return *this;
    }
    __forceinline CTexture* GetTexture() const { return FrameData->GetTexture(); }   // ?GetTexture@CGraphicFrame@@QBEPAVCTexture@@XZ
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
// Layout: CEnvironmentDef (Ego_r.pdb; no engine header): the engine's current environment def at 0x01436E24; its graphic
// indices sit 4 bytes lower in retail than in Ego_r (IceTexture retail +0xCC).
class CEnvironmentDef { public:
    unsigned char _pad00[0xcc];
    long          IceTexture;                          // +0xcc (Ego_r +0xd0)
};
extern CRenderManagerCore* g_pRenderManagerCore;         // 0x01436E18
extern CShaderRenderManager* g_pShaderRenderManager;     // 0x01436E14
extern CEngineResourceManager* g_pResourceManager;       // 0x01436E98
extern const CEnvironmentDef* g_pEngineEnvironmentDef;   // 0x01436E24 (descriptive)
extern CEngineLightingManager* g_pEngineLightingManager;       // 0x01436E9C
extern CEngineShadowRenderer* g_pShadowRenderer_01436e60;         // 0x01436E60

static inline float Clamp(float v, float lo, float hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

__forceinline bool CEngineWaterRenderer::SetIceForegroundTexturesAndUVModes(bool shadowed)
{
    CGraphicFrame ice;
    unsigned long index = g_pEngineEnvironmentDef->IceTexture;
    const CGraphicDataBank* bank = g_pResourceManager->GraphicBanks[0];
    CGraphicInfoFrame* frame = bank->GetGraphicInfo(index) ? bank->GetGraphicInfo(index)->Frames : 0;
    if (!frame->GetFrame(ice, 0, 0))
        return false;
    g_pRenderManagerCore->AttachTextureToStage(0, IceBumpMap);
    g_pRenderManagerCore->AttachTextureToStage(1, EnvironmentMap);
    g_pRenderManagerCore->AttachTextureToStage(2, ice.GetTexture());
    g_pRenderManagerCore->SetTextureUVModeU(0, TEXTURE_UV_MODE_WRAP);
    g_pRenderManagerCore->SetTextureUVModeV(0, TEXTURE_UV_MODE_WRAP);
    g_pRenderManagerCore->SetTextureUVModeU(1, TEXTURE_UV_MODE_MIRROR);
    g_pRenderManagerCore->SetTextureUVModeV(1, TEXTURE_UV_MODE_CLAMP);
    g_pRenderManagerCore->SetTextureUVModeU(2, TEXTURE_UV_MODE_WRAP);
    g_pRenderManagerCore->SetTextureUVModeV(2, TEXTURE_UV_MODE_WRAP);
    if (shadowed) {
        CEngineShadowRenderer::CColorBufferDepthPackParameters params = { 4, 5, 3 };
        g_pShadowRenderer_01436e60->SetupShadowFuncRenderStates(3, params);
    }
    return true;
}

__forceinline void CEngineWaterRenderer::SetupIceForegroundShaderConstantsAndFog()
{
    g_pShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
    g_pShaderRenderManager->SetUserVertexShaderConstant(1, Settings.SoftEdgeSpanFactor, 0.0f, 0.0f, 0.0f);
    float factor = 0.0f;
    if (Settings.EnableBumpMap)
        factor = Settings.BumpMatFactor * 0.5f;
    g_pShaderRenderManager->SetUserVertexShaderConstant(3, factor, factor, factor, factor);
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
    g_pShaderRenderManager->PrepareForRender(0x614);
}

void CEngineWaterRenderer::RenderIceForeground(float time)
{
    CRenderStateCaptureBlock captureBlock;
    EShadowRenderingTechnique technique = g_pShadowRenderer_01436e60->RenderingTechnique;
    if (SetIceForegroundTexturesAndUVModes(technique != SHADOW_RENDERING_TECHNIQUE_NONE)) {
        SetupIceForegroundShaderConstantsAndFog();
        g_pEngineLightingManager->UpdateGlobalLightingStates();
        g_pRenderManagerCore->SetSpecularEnable(true);
        g_pRenderManagerCore->SelectIndexBuffer(&ForegroundIndexBuffers, 0);
        for (std::pair<C3DBoundingBox, CWaterPatchMesh*>* it = IceForegroundRenderList.begin(); it != IceForegroundRenderList.end(); ++it) {
            C3DBoundingBox area = it->first;
            CWaterPatchMesh* mesh = it->second;
            CEnginePrimitiveThemeChannelDesc channel;
            channel.ChannelIndex0 = 0;
            channel.ChannelIndex1 = 0;
            channel.ChannelBlendFactor = 0;
            CEngineLocalLightArray* lights = g_pEngineLightingManager->GetLightsInArea(area, 3, channel);
            g_pEngineLightingManager->SetupShaderPointLights(*lights);
            switch (technique) {
            case SHADOW_RENDERING_TECHNIQUE_NONE:
                g_pShaderRenderManager->ApplyPixelShader(IceForegroundPixelShader);
                g_pShaderRenderManager->ApplyVertexShader(GetIceForegroundVertexShader(lights->LightCount), false);
                break;
            case SHADOW_RENDERING_TECHNIQUE_SHADOW_BUFFER:
                g_pShaderRenderManager->ApplyPixelShader(IceForegroundShadowedPixelShader);
                g_pShaderRenderManager->ApplyVertexShader(GetIceForegroundShadowedVertexShader(lights->LightCount), false);
                break;
            case SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER:
                g_pShaderRenderManager->ApplyPixelShader(IceForegroundColbuffShadowedPixelShader);
                g_pShaderRenderManager->ApplyVertexShader(GetIceForegroundColbuffShadowedVertexShader(lights->LightCount), false);
                break;
            }
            g_pShaderRenderManager->PrepareForRender(0x14);
            mesh->Render(WaterForegroundPolyCount);
        }
        g_pEngineLightingManager->DisablePointLights();
    }
}
