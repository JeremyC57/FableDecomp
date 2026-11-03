// CEngineWaterRenderer::RenderEnvironmentMap() -- DRAFT
// Leaf: ?RenderEnvironmentMap@CEngineWaterRenderer@@AAEXXZ
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C2DVECTOR
#include <math.h>
#include "engine/CEngineWaterRenderer.h"
#include "engine/CRenderManagerCore.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineCamera.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsI.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
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

extern CRenderManagerCore* g_pRenderManagerCore;         // 0x01436E18
extern CShaderRenderManager* g_pShaderRenderManager;     // 0x01436E14
extern CEngineResourceManager* g_pResourceManager;       // 0x01436E98
extern CEngineWorldMap* g_pEngineWorldMap;               // 0x01436E8C
extern CEngineCamera* g_pEngineCamera;                   // 0x01436EA0
extern CDisplayManager* g_pDisplayManager_01436e1c;      // 0x01436E1C

static inline float Clamp(float v, float lo, float hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

void CEngineWaterRenderer::RenderEnvironmentMap()
{
    const CIDrawEnvironmentParams& params = g_pEngineWorldMap->GameWorld->DrawGetEnvironment()->PeekInterpolatedParams();
    unsigned long skyTexture = params.ThemeChannels[0].TextureSets[0].Textures[0];
    unsigned long skyBlendTexture = params.ThemeChannels[0].TextureSets[0].Textures[1];
    const CGraphicDataBank* bank = g_pResourceManager->GraphicBanks[0];
    if (!skyTexture)
        return;
    CGraphicFrame frame;
    CGraphicInfoFrame* infoFrame = bank->GetGraphicInfo(skyTexture) ? bank->GetGraphicInfo(skyTexture)->Frames : 0;
    if (!infoFrame->GetFrame(frame, 0, 0))
        return;
    CTexture* texture = frame.GetTexture();
    g_pRenderManagerCore->AttachTextureToStage(0, texture);
    if (skyBlendTexture) {
        CGraphicInfoFrame* blendFrame = bank->GetGraphicInfo(skyBlendTexture) ? bank->GetGraphicInfo(skyBlendTexture)->Frames : 0;
        if (blendFrame->GetFrame(frame, 0, 0))
            texture = frame.GetTexture();
    }
    g_pRenderManagerCore->AttachTextureToStage(1, texture);

    g_pShaderRenderManager->ApplyPixelShader(SkyMapPixelShader);
    g_pShaderRenderManager->ApplyVertexShader(SkyMapVertexShader, false);
    float alpha = (1.0f - params.ThemeChannels[0].TextureSets[0].Blends[0]) * 255.0f;
    float blend = Clamp(alpha / 255.0f, 0.0f, 1.0f);
    C4DVector colour;
    colour.X = Settings.Colour.R;
    colour.Y = Settings.Colour.G;
    colour.Z = Settings.Colour.B;
    colour.W = params.ThemeChannels[0].Scalers[2] * Settings.ColourToReflectionFactor;
    C4DVector premultiplied = colour;
    premultiplied.X *= premultiplied.W;
    premultiplied.Y *= premultiplied.W;
    premultiplied.Z *= premultiplied.W;
    premultiplied.W = 1.0f - premultiplied.W;
    g_pShaderRenderManager->SetPixelShaderConstant(5, premultiplied);
    g_pShaderRenderManager->SetPixelShaderConstant(6, C4DVector(blend, blend, blend, blend));

    CRGBFloatColour topColour = params.ThemeChannels[0].Colours[12];
    CRGBFloatColour bottomColour = params.ThemeChannels[0].Colours[13];
    const C3DVector& look = g_pEngineCamera->PeekLookVector();
    float lookZ = look.Z;
    C2DVector direction(look.X, look.Y);
    direction.AccurateNormalise();
    float fovFraction = g_pEngineCamera->PeekHFOV() / 6.2831855f;
    float angle = acos(direction.Y) / 6.2831855f;
    if (direction.X < 0.0f)
        angle = 1.0f - angle;
    float halfFOV = g_pEngineCamera->PeekHFOV() * 0.5f;
    float start = angle - halfFOV / 6.2831855f;
    float vfov = g_pEngineCamera->PeekVFOV();
    g_pShaderRenderManager->SetUserVertexShaderConstant(0, topColour);
    g_pShaderRenderManager->SetUserVertexShaderConstant(1, bottomColour);
    g_pShaderRenderManager->SetUserVertexShaderConstant(2, start, fovFraction,
        tan(1.5707964f - acos(lookZ) + vfov * 0.5f) * 0.9285714f + 0.071428575f, 0.0f);
    g_pRenderManagerCore->SetTextureUVModeU(0, TEXTURE_UV_MODE_WRAP);
    g_pRenderManagerCore->SetTextureUVModeV(0, TEXTURE_UV_MODE_CLAMP);
    g_pRenderManagerCore->SetTextureUVModeU(1, TEXTURE_UV_MODE_WRAP);
    g_pRenderManagerCore->SetTextureUVModeV(1, TEXTURE_UV_MODE_CLAMP);
    g_pShaderRenderManager->PrepareForRender(0);
    g_pRenderManagerCore->DrawCurrentBuffersAsIndexedTris(0, 14, 18, 2);

    g_pShaderRenderManager->ApplyPixelShader(EnvironmentMapPixelShader);
    g_pShaderRenderManager->ApplyVertexShader(EnvironmentMapVertexShader, false);
    g_pRenderManagerCore->SetSourceAlphaBlendMode(ALPHA_BLEND_MODE_SRC_ALPHA);
    g_pRenderManagerCore->SetDestAlphaBlendMode(ALPHA_BLEND_MODE_INV_SRC_ALPHA);
    g_pRenderManagerCore->SetAlphaBlendEnable(true);
    CTexture& backBuffer = g_pDisplayManager_01436e1c->CopyBackBufferToTexture();
    g_pRenderManagerCore->AttachTextureToStage(0, &backBuffer);
    unsigned long width = backBuffer.GetDimensions(0).X;
    unsigned long height = backBuffer.GetDimensions(0).Y;
    g_pShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
    g_pShaderRenderManager->SetUserVertexShaderConstant(1, (float)width, (float)height, 0.0f, 0.0f);
    g_pRenderManagerCore->SetTextureUVModeU(0, TEXTURE_UV_MODE_CLAMP);
    g_pRenderManagerCore->SetTextureUVModeV(0, TEXTURE_UV_MODE_CLAMP);
    g_pShaderRenderManager->PrepareForRender(0);
    g_pRenderManagerCore->DrawCurrentBuffersAsIndexedTris(0, 14, 6, 4);
}
