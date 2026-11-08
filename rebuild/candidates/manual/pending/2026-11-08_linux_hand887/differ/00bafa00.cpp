// CEnginePrimitiveManager2DSprites::RenderClockPrim -- clip a 2D clock sprite, fetch its two frames and the 2D alpha mask, build the seven-vertex clock fan and queue it
// Residue: DIFFER(3884v3946), 1373 normalized diff lines (2 graded sources, first draft): the whole structure is decoded (mask frame first, two sprite frames, texture-2 UVs only when texture 1 is missing, UV offset lerp, the linear-format scale of the first linear texture, seven-vertex fan, GFClamp by reference, the 4-segment fmod fraction, GFFloatToLongCeil segment, 18-index loop, Texture[0] = frame-2 texture, colours swapped) but the frame is 0x7c (`mov ebp,esp`) where retail opens an ebp-offset frame of 0x8c with `lea ebp,[esp-0x6c]`, and the clock pointer/flags/layer roles differ (retail edi = primitive then ebx, flags a memory bool at [ebp+0x68]).
// Next: grade the corner points and half sizes as C2DVector/C2DBoxF temporaries to grow the frame to retail's 0x8c (the many distinct slots [ebp+0x20..0x68] are the corner locals), keep `flags` a named local read as `(EnginePrimitiveFlags >> 7) & 1` but used only through GetFrame, then fix register roles with c2prio.
// Leaf: ?RenderClockPrim@CEnginePrimitiveManager2DSprites@@MAEXAAVCEngineInternalPrimitiveBase@@AAVCEngineSubPrimitiveChain@@W4ESubPrimitiveType@@@Z
// Header debt: engine/CGraphicFrame.h, engine/CGraphicInfoFrame.h, engine/CEngineInternalPrimitive2DSprite.h and the 2D scene / resource
// singletons are opaque or flattened; the models below are the landed GetFrame 0x0041C260 / Get2DAlphaMaskTexture 0x00BA9A70 models
// (Ego_r.pdb layouts), kept local until those headers grow retail branches.
#include "fable_compiler.h"
#include "engine/CEngineInternalPrimitiveBase.h"
class CTexture;
void __cdecl operator delete(void* block);                              // 0x00BFE9BC

class CResourceList;
class CResource { public:                                                // Ego_r.pdb, 0x1C bytes
    virtual void v00();
    virtual void Release();
    unsigned long  RefCount;                                             // +0x04
    CResourceList* ResourceList;                                         // +0x08
    CResource*     PrevResource;                                         // +0x0c
    CResource*     NextResource;                                         // +0x10
    unsigned long  ResourceSize;                                         // +0x14
    unsigned long  LastUsedFrame;                                        // +0x18
    void ListRemove();                                                   // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);                   // 0x009FBF90
    FABLE_FORCEINLINE void Touch();
};
class CResourceList { public:
    unsigned long _pad00;
    CResource     Head;                                                  // +0x04
    unsigned long ResourceCount;                                         // +0x20
    long          AllocatedMemory;                                       // +0x24
    long          MaximumMemory;                                         // +0x28
    unsigned long CurrentFrame;                                          // +0x2c
};
FABLE_FORCEINLINE void CResource::Touch()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}
struct CCPInfo { long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
struct CTexturePointer {                                                 // CCountedPointer<CTexture>
    CTexture* Data;
    CCPInfo*  Info;
    FABLE_FORCEINLINE ~CTexturePointer()
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
struct CTextureQuadUVs { float U[4]; float V[4]; };                     // TL, TR, BR, BL
class CGraphicFrameData { public:                                        // Ego_r.pdb, 27 bytes
    unsigned long   ReferenceCount;                                      // +0x00
    void*           Data;                                                // +0x04
    CTexturePointer PTexture;                                            // +0x08
    unsigned short  TLX, TLY, Width, Height, Pitch;                      // +0x10
    unsigned char   Alpha : 1;                                           // +0x1a
    unsigned char   BooleanAlpha : 1;
    unsigned char   UsesSurface : 1;
    unsigned char   SectionOfTPage : 1;
    unsigned char   IsInitialised : 1;
    unsigned char   ScreenGraphic : 1;
    void Uninitialise();                                                 // 0x009FCBA0
    CTextureQuadUVs GetTextureQuadUVs() const;                           // 0x009FC810
    FABLE_FORCEINLINE ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    FABLE_FORCEINLINE void DecRef()
    {
        if (--ReferenceCount == 0) {
            this->~CGraphicFrameData();
            operator delete(this);
        }
    }
};
class CGraphicFrame { public:                                            // intrusive pointer to CGraphicFrameData
    CGraphicFrameData* FrameData;
    CGraphicFrame() : FrameData(0) { }
    FABLE_FORCEINLINE ~CGraphicFrame() { if (FrameData) FrameData->DecRef(); }
    FABLE_FORCEINLINE CGraphicFrame& operator=(const CGraphicFrame& other)
    {
        if (FrameData)
            FrameData->DecRef();
        FrameData = other.FrameData;
        if (FrameData)
            ++FrameData->ReferenceCount;
        return *this;
    }
    CGraphicFrameData* operator->() const { return FrameData; }
};
class CManagedTexture { public:
    unsigned long _pad00;
    CResource*    Data;                                                  // +0x04
    FABLE_FORCEINLINE void Touch() { if (Data) Data->Touch(); }
};
class CGraphicDataBank { public:
    unsigned char _pad00[0x194];
    unsigned long CurrentFrame;                                          // +0x194
};
class CGraphicInfo;
template <class T> struct CFrameDataPointer {                            // CIVCountedPointer<CFrameData> (landed 0x0041C260 model)
    T* P;
    FABLE_FORCEINLINE CFrameDataPointer& operator=(T* p)
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
class CGraphicInfoFrame { public:
    class CFrameData;
    unsigned long  _pad00;
    unsigned short FrameNum;                                             // +0x04
    signed char    SmallMipmapLevel;                                     // +0x06
    mutable CFrameDataPointer<CFrameData> Data;                                // +0x08 CIVCountedPointer<CFrameData>
    CGraphicInfo*  GraphicInfo;                                          // +0x0c
    bool InternalGetFrameForcedLoading(unsigned long flags) const;       // 0x009FF6B0
    FABLE_FORCEINLINE bool GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags) const;   // ?GetFrame@CGraphicInfoFrame@@QBE_NAAVCGraphicFrame@@KK@Z
};
class CGraphicInfoFrame::CFrameData : public CResource { public:         // Ego_r.pdb, 0x44 bytes
    CGraphicFrame   Frame;                                               // +0x1c
    CManagedTexture ManagedTexture;                                      // +0x20
    CManagedTexture LoadingManagedTexture;                               // +0x28
    void*           FileData[2];                                         // +0x30
    signed char     CurrentMipmapLevel;                                  // +0x38
    signed char     LoadingMipmapLevel;                                  // +0x39
    signed char     TouchedMipmapLevel;                                  // +0x3a
    unsigned long   TouchedFrame;                                        // +0x3c
    CGraphicInfoFrame* GraphicInfoFrame;                                 // +0x40
    CFrameData(const CGraphicInfoFrame* frame);                          // 0x009FE380
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FDE80
    FABLE_FORCEINLINE void DecRef() { if (--RefCount == 0) Release(); }
};
class CGraphicInfo { public:                                             // Ego_r.pdb, 0x2c bytes
    unsigned char      _pad00[0x20];
    CGraphicDataBank*  Bank;                                             // +0x20
    long               Index;                                            // +0x24
    CGraphicInfoFrame* Frames;                                           // +0x28
};
template<class T> struct SVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    FABLE_FORCEINLINE unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    FABLE_FORCEINLINE const T& operator[](unsigned long n) const { return *(_M_start + n); }
};
class CGraphicDataBankFull : public CGraphicDataBank { public:
    unsigned char         _pad01[0x1e0 - 0x198];
    SVector<CGraphicInfo> GraphicList;                                   // +0x1e0
    FABLE_FORCEINLINE const CGraphicInfo* PeekGraphic(unsigned long index) const { return index < GraphicList.size() ? &GraphicList[index] : 0; }   // ?PeekGraphic@CGraphicDataBank@@QBEPBVCGraphicInfo@@K@Z
    FABLE_FORCEINLINE const CGraphicInfoFrame* PeekGraphicInfoFrame(unsigned long index, unsigned long frame) const   // ?PeekGraphicInfoFrame@CGraphicDataBank@@QBEPBVCGraphicInfoFrame@@KK@Z
    {
        return PeekGraphic(index) ? &PeekGraphic(index)->Frames[frame] : 0;
    }
};
// The landed CGraphicInfoFrame::GetFrame 0x0041C260 body (inline here).
FABLE_FORCEINLINE bool CGraphicInfoFrame::GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags) const
{
    if (Data) {
        if (Data->Frame.FrameData) {
            if (!(flags & 8)) {
                unsigned long frame = GraphicInfo->Bank->CurrentFrame;
                if (Data->TouchedFrame != frame || mipmapLevel <= (unsigned long)Data->TouchedMipmapLevel) {
                    Data->TouchedFrame = frame;
                    Data->ManagedTexture.Touch();
                    if (mipmapLevel <= (unsigned long)Data->CurrentMipmapLevel)
                        Data->Touch();
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

#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_FORMATQUERIES
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_ISLINEAR
#include "engine/CTexture.h"
struct CEngineResourceManager {                                          // 0x01436E98 (g_pResourceManager)
    unsigned char         _pad00[0x954];
    CGraphicDataBankFull* SpriteBank;                                    // +0x954 (descriptive)
    unsigned char         _pad958[0x2adc - 0x958];
    CTexture              SatRGBATexture;                                // +0x2adc
};
extern CEngineResourceManager* g_pResourceManager;                       // 0x01436E98

#ifndef FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#endif
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DExtentsI.h"
#include "engine/CRGBColour.h"
struct CTVertexColTex2 { float X, Y, Z; unsigned long DiffuseColour; float U1, V1, U2, V2; };   // Ego_r.pdb CTVertexColTex2Base, 0x20

class CDisplayManager { public:                                          // 0x01436E1C
    unsigned char _pad[0x1f4];
    C2DBoxI       Viewport;                                              // +0x1f4 (descriptive)
    const C2DExtentsI GetRenderTargetDimensions();                       // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayManager;                               // 0x01436E1C
struct C2DClipRegion {                                                   // 0x01436E84 ->+0x94 ->+0xc (descriptive)
    void*         Owner;                                                 // +0x00
    unsigned long MaskBankIndex;                                         // +0x04
    unsigned long MaskFrame;                                             // +0x08
    unsigned char _pad0c[0xc];
    C2DBoxF       Box;                                                   // +0x18
    bool          Enabled;                                               // +0x28
};
struct CEngine2DScene { unsigned char _pad[0xc]; C2DClipRegion Clip; };
struct CEngine2DSceneOwner { unsigned char _pad[0x94]; CEngine2DScene* Scene; };
extern CEngine2DSceneOwner* g_pEngine2DSceneOwner;                       // 0x01436E84

enum EEngine2DLayer { };
class CEnginePrimitiveManager;
class CEngineSubPrimitiveBase { public:
    void*                         __vftable;
    unsigned long                 RenderFlags;                           // +0x04
    unsigned char                 Alpha;                                 // +0x08
    CEngineInternalPrimitiveBase* Parent;                                // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive;                      // +0x10
    CEnginePrimitiveManager*      Manager;                               // +0x14
    static void* __fastcall operator new(unsigned int size);             // 0x00BEB540
};
class CEnginePrimitiveRenderer2D { public:                               // 0x01436E7C
    unsigned char _pad[0x320];
    bool          LayerEnabled[1];                                       // +0x320
    float Layer2DToSortDistance(EEngine2DLayer layer);                   // 0x00B4A6A0
    float Layer2DToDistance(EEngine2DLayer layer);                       // 0x00B4A6B0
    float Layer2DDistanceToZ(float distance);                            // 0x00B4A6C0
};
extern CEnginePrimitiveRenderer2D* g_pPrimitiveRenderer2D;               // 0x01436E7C
class CEngineSubPrimitiveRenderer { public:
    void AddSubPrimitive(long list, CEngineSubPrimitiveBase* subPrimitive, float distance, CEnginePrimitiveManager* manager);   // 0x00B84720
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;       // 0x01436E74
struct CEngineSettings { unsigned char _pad[0xee]; bool DisplacementPassEnabled; };   // +0xee
extern CEngineSettings* g_pEngineSettings;                               // 0x01436E38
struct CEnginePrimitiveRendererShared { unsigned char _pad[0x25c]; bool DrawDebug2DBoxes; };   // +0x25c (descriptive)
extern CEnginePrimitiveRendererShared* g_pPrimitiveRenderer;             // 0x01436E78
class CRenderManager2D { public: bool Draw2DBoxOutline(const C2DBoxF& box, const CRGBColour& colour, unsigned long flags); };   // 0x009DE140 ?Draw2DBoxOutline@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCRGBColour@@K@Z
extern CRenderManager2D* g_pRenderManager2D;                                 // 0x01436E18
float __fastcall GFAXToVX(float x);                                      // 0x009E1DA0
float __fastcall GFAYToVY(float y);                                      // 0x009E1DD0
struct CEngineInterpolationSet { float Start; float Factor; float End; }; // 12 bytes (descriptive)
extern CEngineInterpolationSet g_EngineInterpolationSets[];             // 0x0143B930 (Factor at 0x0143B934)
class CEngineSubPrimitiveChain;
enum ESubPrimitiveType { };
#include "engine/GFFloatToLongNear.h"
extern float g_CosTable[1025];                                           // 0x013CD550: cos(2*pi*k/1024), k = 0..1024
FABLE_FORCEINLINE float __fastcall GFFastCos(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i &= 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}
FABLE_FORCEINLINE float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

#include <math.h>
#include "engine/GFFloatToLongCeil.h"

class CEngineInternalPrimitive2DClockSprite : public CEngineInternalPrimitiveBase { public:   // Ego_r.pdb, 0x94 bytes
    C2DBoxF       ClipBox;          // +0x48
    long          Layer;            // +0x58
    unsigned long BankIndex;        // +0x5c
    unsigned long Frame;            // +0x60
    CRGBColour    Colour;           // +0x64
    unsigned long BankIndex2;       // +0x68
    unsigned long Frame2;           // +0x6c
    CRGBColour    Colour2;          // +0x70
    float         AngleStart;       // +0x74
    float         AngleEnd;         // +0x78
    bool          Filter;           // +0x7c
    C2DVector     UVOffsetsStart;   // +0x7d
    C2DVector     UVOffsetsEnd;     // +0x85
    bool          WrapTexture;      // +0x8d
    unsigned char _pad_0x8e[2];
    long          BlendMode;        // +0x90
};
class CEngineSubPrimitive2DClockSprite : public CEngineSubPrimitiveBase { public:   // Ego_r.pdb, 0x13c bytes
    CTVertexColTex2 Vertices[7];    // +0x18
    unsigned short  Indices[18];    // +0xf8
    unsigned char   IndexOffset;    // +0x11c
    unsigned char   _pad_0x11d[3];
    const CTexture* Texture[2];     // +0x120
    CRGBColour      Colour;         // +0x128
    CRGBColour      Colour2;        // +0x12c
    const CTexture* AlphaMask;      // +0x130
    bool            Filter;         // +0x134
    bool            WrapTexture;    // +0x135
    long            BlendMode;      // +0x138
    CEngineSubPrimitive2DClockSprite(CEngineInternalPrimitive2DClockSprite* parent);   // 0x00BB1780
};
template <class T> FABLE_FORCEINLINE const T& GFClamp(const T& value, const T& low, const T& high)
{
    return value < low ? low : (value > high ? high : value);
}

class CEnginePrimitiveManager2DSprites {
protected:
    virtual void RenderClockPrim(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type);
};

void CEnginePrimitiveManager2DSprites::RenderClockPrim(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type)
{
    CEngineInternalPrimitive2DClockSprite& clock = static_cast<CEngineInternalPrimitive2DClockSprite&>(primitive);
    const C2DBoxI& viewport = g_pDisplayManager->Viewport;
    if (viewport.BRX < clock.ClipBox.TLX || viewport.TLX > clock.ClipBox.BRX || viewport.BRY < clock.ClipBox.TLY || viewport.TLY > clock.ClipBox.BRY)
        return;
    CEngine2DScene* scene = g_pEngine2DSceneOwner->Scene;
    if (scene->Clip.MaskBankIndex && scene->Clip.Owner && scene->Clip.Enabled) {
        if (clock.ClipBox.TLX > scene->Clip.Box.BRX || clock.ClipBox.BRX < scene->Clip.Box.TLX || clock.ClipBox.TLY > scene->Clip.Box.BRY || clock.ClipBox.BRY < scene->Clip.Box.TLY)
            return;
    }
    if (clock.Colour.A == 0)
        return;
    if (!g_pPrimitiveRenderer2D->LayerEnabled[clock.Layer])
        return;
    unsigned long flags = (clock.EnginePrimitiveFlags >> 7) & 1;

    const CTexture* alpha_mask;
    if (scene->Clip.MaskBankIndex && scene->Clip.Owner) {
        const CGraphicInfoFrame* mask_info = g_pResourceManager->SpriteBank->PeekGraphicInfoFrame(scene->Clip.MaskBankIndex, 0);
        CGraphicFrame mask;
        if (!mask_info->GetFrame(mask, 0, flags))
            return;
        alpha_mask = mask->PTexture.Data;
    } else {
        alpha_mask = &g_pResourceManager->SatRGBATexture;
    }

    CTextureQuadUVs uvs;
    const CTexture* texture;
    if (clock.BankIndex) {
        const CGraphicInfoFrame* frame_info = g_pResourceManager->SpriteBank->PeekGraphicInfoFrame(clock.BankIndex, clock.Frame);
        CGraphicFrame frame;
        if (!frame_info->GetFrame(frame, 0, flags))
            return;
        texture = frame->PTexture.Data;
        uvs = frame->GetTextureQuadUVs();
        float du = 0.5f / frame->Width;
        float dv = 0.5f / frame->Height;
        uvs.U[1] += du;
        uvs.U[2] += du;
        uvs.V[2] += dv;
        uvs.V[3] += dv;
    } else {
        texture = 0;
    }
    const CTexture* texture2;
    if (clock.BankIndex2) {
        const CGraphicInfoFrame* frame_info = g_pResourceManager->SpriteBank->PeekGraphicInfoFrame(clock.BankIndex2, clock.Frame2);
        CGraphicFrame frame;
        if (!frame_info->GetFrame(frame, 0, flags))
            return;
        texture2 = frame->PTexture.Data;
        if (!texture) {
            uvs = frame->GetTextureQuadUVs();
            float du = 0.5f / frame->Width;
            float dv = 0.5f / frame->Height;
            uvs.U[1] += du;
            uvs.U[2] += du;
            uvs.V[2] += dv;
            uvs.V[3] += dv;
        }
    } else {
        texture2 = 0;
    }
    if (!texture && !texture2) {
        uvs.U[0] = 0.0f; uvs.V[0] = 0.0f;
        uvs.U[1] = 1.0f; uvs.V[1] = 0.0f;
        uvs.U[2] = 1.0f; uvs.V[2] = 1.0f;
        uvs.U[3] = 0.0f; uvs.V[3] = 1.0f;
    } else {
        float factor = g_EngineInterpolationSets[*(long*)clock.InterpolationSet].Factor;
        C2DVector offset = (clock.UVOffsetsEnd - clock.UVOffsetsStart) * factor + clock.UVOffsetsStart;
        uvs.U[0] += offset.X;
        uvs.V[0] += offset.Y;
        uvs.U[1] += offset.X;
        uvs.V[1] += offset.Y;
        uvs.U[2] += offset.X;
        uvs.V[2] += offset.Y;
        uvs.U[3] += offset.X;
        uvs.V[3] += offset.Y;
        if (texture && texture->GetPixelFormat().IsLinear()) {
            float width = (float)texture->GetWidth(0);
            uvs.U[0] *= width;
            uvs.U[1] *= width;
            uvs.U[2] *= width;
            uvs.U[3] *= width;
            float height = (float)texture->GetHeight(0);
            uvs.V[0] *= height;
            uvs.V[1] *= height;
            uvs.V[2] *= height;
            uvs.V[3] *= height;
        } else if (texture2 && texture2->GetPixelFormat().IsLinear()) {
            float width = (float)texture2->GetWidth(0);
            uvs.U[0] *= width;
            uvs.U[1] *= width;
            uvs.U[2] *= width;
            uvs.U[3] *= width;
            float height = (float)texture2->GetHeight(0);
            uvs.V[0] *= height;
            uvs.V[1] *= height;
            uvs.V[2] *= height;
            uvs.V[3] *= height;
        }
    }

    float half_width = (clock.ClipBox.BRX - clock.ClipBox.TLX) * 0.5f;
    float half_height = (clock.ClipBox.BRY - clock.ClipBox.TLY) * 0.5f;
    float z = g_pPrimitiveRenderer2D->Layer2DDistanceToZ(g_pPrimitiveRenderer2D->Layer2DToDistance((EEngine2DLayer)clock.Layer));
    C2DVector centre(half_width + clock.ClipBox.TLX, half_height + clock.ClipBox.TLY);
    C2DVector top_left(clock.ClipBox.TLX, clock.ClipBox.TLY);
    C2DVector top_right(clock.ClipBox.BRX, clock.ClipBox.TLY);
    C2DVector bottom_left(clock.ClipBox.TLX, clock.ClipBox.BRY);
    C2DVector bottom_right(clock.ClipBox.BRX, clock.ClipBox.BRY);
    const C2DExtentsI target = g_pDisplayManager->GetRenderTargetDimensions();
    float scale_x = 1.0f / (target.X * 0.5f);
    float scale_y = 1.0f / (target.Y * 0.5f);
    centre.X = centre.X * scale_x - 1.0f;              centre.Y = 1.0f - centre.Y * scale_y;
    top_left.X = top_left.X * scale_x - 1.0f;          top_left.Y = 1.0f - top_left.Y * scale_y;
    top_right.X = top_right.X * scale_x - 1.0f;        top_right.Y = 1.0f - top_right.Y * scale_y;
    bottom_left.X = bottom_left.X * scale_x - 1.0f;    bottom_left.Y = 1.0f - bottom_left.Y * scale_y;
    bottom_right.X = bottom_right.X * scale_x - 1.0f;  bottom_right.Y = 1.0f - bottom_right.Y * scale_y;

    CEngineSubPrimitive2DClockSprite* sub = new CEngineSubPrimitive2DClockSprite(&clock);
    sub->Vertices[0].X = centre.X;        sub->Vertices[0].Y = centre.Y;        sub->Vertices[0].Z = z;
    sub->Vertices[0].U1 = (uvs.U[2] - uvs.U[0]) * 0.5f + uvs.U[0];
    sub->Vertices[0].V1 = (uvs.V[2] - uvs.V[0]) * 0.5f + uvs.V[0];
    sub->Vertices[1].X = centre.X;        sub->Vertices[1].Y = top_left.Y;      sub->Vertices[1].Z = z;
    sub->Vertices[1].U1 = (uvs.U[1] - uvs.U[0]) * 0.5f + uvs.U[0];
    sub->Vertices[1].V1 = uvs.V[0];
    sub->Vertices[2].X = top_right.X;     sub->Vertices[2].Y = top_right.Y;     sub->Vertices[2].Z = z;
    sub->Vertices[2].U1 = uvs.U[1];       sub->Vertices[2].V1 = uvs.V[1];
    sub->Vertices[3].X = bottom_right.X;  sub->Vertices[3].Y = bottom_right.Y;  sub->Vertices[3].Z = z;
    sub->Vertices[3].U1 = uvs.U[2];       sub->Vertices[3].V1 = uvs.V[2];
    sub->Vertices[4].X = bottom_left.X;   sub->Vertices[4].Y = bottom_left.Y;   sub->Vertices[4].Z = z;
    sub->Vertices[4].U1 = uvs.U[3];       sub->Vertices[4].V1 = uvs.V[3];
    sub->Vertices[5].X = top_left.X;      sub->Vertices[5].Y = top_left.Y;      sub->Vertices[5].Z = z;
    sub->Vertices[5].U1 = uvs.U[0];       sub->Vertices[5].V1 = uvs.V[0];

    float angle = (clock.AngleEnd - clock.AngleStart) * g_EngineInterpolationSets[*(long*)clock.InterpolationSet].Factor + clock.AngleStart;
    angle = GFClamp(angle, 0.0f, 1.0f);
    float fraction;
    if (angle > 0.0 && angle < 1.0f) {
        if (angle >= 0.125f && angle <= 0.875f)
            fraction = (float)fmod((double)(angle - 0.125f), 0.25) * 4.0f;
        else
            fraction = (float)fmod((double)angle, 0.125) * 8.0f;
    } else {
        fraction = GFClamp(angle, 0.0f, 1.0f);
    }
    long segment = GFFloatToLongCeil(angle * 4.0f + 0.5f);
    long next = segment == 5 ? 1 : segment + 1;
    const CTVertexColTex2& from = sub->Vertices[segment];
    const CTVertexColTex2& to = sub->Vertices[next];
    sub->Vertices[6].X = (to.X - from.X) * fraction + from.X;
    sub->Vertices[6].Y = (to.Y - from.Y) * fraction + from.Y;
    sub->Vertices[6].Z = z;
    sub->Vertices[6].U1 = (to.U1 - from.U1) * fraction + from.U1;
    sub->Vertices[6].V1 = (to.V1 - from.V1) * fraction + from.V1;
    long index = 0;
    for (long i = 1; i < 6; ++i) {
        long following = i == 5 ? 1 : i + 1;
        if (segment == i) {
            sub->Indices[index++] = 0;
            sub->Indices[index++] = (unsigned short)i;
            sub->Indices[index++] = 6;
            sub->IndexOffset = (unsigned char)index;
            sub->Indices[index++] = 0;
            sub->Indices[index++] = 6;
        } else {
            sub->Indices[index++] = 0;
            sub->Indices[index++] = (unsigned short)i;
        }
        sub->Indices[index++] = (unsigned short)following;
    }
    sub->Texture[0] = texture2;
    sub->Texture[1] = texture;
    sub->Colour = clock.Colour2;
    sub->Colour2 = clock.Colour;
    sub->AlphaMask = alpha_mask;
    sub->Filter = clock.Filter;
    sub->WrapTexture = clock.WrapTexture;
    sub->Alpha = 0xff;
    sub->RenderFlags = 1;
    long list = (clock.EnginePrimitiveFlags & 0x400) ? 5 : type;
    g_pEngineSubPrimitiveRenderer->AddSubPrimitive(list, sub, g_pPrimitiveRenderer2D->Layer2DToSortDistance((EEngine2DLayer)clock.Layer), (CEnginePrimitiveManager*)this);
    if (g_pPrimitiveRenderer->DrawDebug2DBoxes) {
        C2DBoxF box;
        box.TLX = GFAXToVX(clock.ClipBox.TLX);
        box.TLY = GFAYToVY(clock.ClipBox.TLY);
        box.BRX = GFAXToVX(clock.ClipBox.BRX);
        box.BRY = GFAYToVY(clock.ClipBox.BRY);
        CRGBColour red;
        red.B = 0; red.G = 0; red.R = 0xff; red.A = 0xff;
        g_pRenderManager2D->Draw2DBoxOutline(box, red, 0);
    }
}
