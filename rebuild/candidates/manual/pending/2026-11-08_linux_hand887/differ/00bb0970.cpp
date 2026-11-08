// CEnginePrimitiveManager2DSprites::RenderSpritePrim -- clip a 2D sprite, fetch its frame and the 2D alpha mask, rotate the quad and queue a sub-primitive
// Residue: DIFFER(3269v3202), 359 normalized diff lines / 182 frame-blind (19 graded sources): VC7.1 packs `flags` (and the half-width that shares its slot) into the dead `primitive` argument home ([esp+0x80], disp32 on 27 accesses = the +67 bytes) where retail keeps both at local 0 with an 8-byte-larger frame (0x74 vs 0x6c); the copy of the GetTextureQuadUVs result uses the returned eax (`mov esi,eax`) where retail uses `lea esi,[temp]`; a zero register (xor ebp,ebp) over the texture/mask blocks where retail has ebp = mask bank index; the vertex-copy colour stores scheduled differently.
// Next: find what keeps retail off the argument home: try a by-value C2DVector half size plus the corners as C2DVector operator temporaries (the corner CSE of centre-across / centre+across suggests operator chains), and c2prio --trace for the ebp role; the 0x400 copy loop and debug box already line up.
// Leaf: ?RenderSpritePrim@CEnginePrimitiveManager2DSprites@@MAEXAAVCEngineInternalPrimitiveBase@@AAVCEngineSubPrimitiveChain@@W4ESubPrimitiveType@@@Z
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


class CEngineInternalPrimitive2DSprite : public CEngineInternalPrimitiveBase { public:   // Ego_r.pdb
    C2DBoxF       ClipBox;          // +0x48
    long          Layer;            // +0x58
    unsigned long BankIndex;        // +0x5c
    unsigned long Frame;            // +0x60
    float         StartAngle;       // +0x64
    float         EndAngle;         // +0x68
    CRGBColour    Colour;           // +0x6c
    CTexture*     Texture;          // +0x70
    bool          Filter;           // +0x74
    C2DVector     UVOffsetsStart;   // +0x75
    C2DVector     UVOffsetsEnd;     // +0x7d
    bool          WrapTexture;      // +0x85
    unsigned char _pad_0x86[2];
    long          BlendMode;        // +0x88
};
class CEngineSubPrimitive2DSprite : public CEngineSubPrimitiveBase { public:   // Ego_r.pdb, 0xa8 bytes
    CTVertexColTex2 Vertices[4];    // +0x18
    const CTexture* Texture;        // +0x98
    const CTexture* AlphaMask;      // +0x9c
    bool            Filter;         // +0xa0
    bool            WrapTexture;    // +0xa1
    long            BlendMode;      // +0xa4
    CEngineSubPrimitive2DSprite(CEngineInternalPrimitive2DSprite* parent);   // 0x00BACE50
};
class CEnginePrimitiveManager2DSprites {
protected:
    virtual void RenderSpritePrim(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type);
};

FABLE_FORCEINLINE const C2DClipRegion& Peek2DClipRegion() { return g_pEngine2DSceneOwner->Scene->Clip; }

void CEnginePrimitiveManager2DSprites::RenderSpritePrim(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type)
{
    CEngineInternalPrimitive2DSprite& sprite = static_cast<CEngineInternalPrimitive2DSprite&>(primitive);
    const CTexture* texture = sprite.Texture;
    const C2DBoxI& viewport = g_pDisplayManager->Viewport;
    if (viewport.BRX < sprite.ClipBox.TLX || viewport.TLX > sprite.ClipBox.BRX || viewport.BRY < sprite.ClipBox.TLY || viewport.TLY > sprite.ClipBox.BRY)
        return;
    const C2DClipRegion& clip = Peek2DClipRegion();
    if (clip.MaskBankIndex && clip.Owner && clip.Enabled) {
        if (sprite.ClipBox.TLX > clip.Box.BRX || sprite.ClipBox.BRX < clip.Box.TLX || sprite.ClipBox.TLY > clip.Box.BRY || sprite.ClipBox.BRY < clip.Box.TLY)
            return;
    }
    if (sprite.Colour.A == 0)
        return;
    if (!g_pPrimitiveRenderer2D->LayerEnabled[sprite.Layer])
        return;
    unsigned long flags = (sprite.EnginePrimitiveFlags & 0x80) ? 0x11 : 0;

    CTextureQuadUVs uvs;
    if (sprite.BankIndex) {
        const CGraphicDataBankFull* bank = g_pResourceManager->SpriteBank;
        const CGraphicInfoFrame* frame_info = bank->PeekGraphicInfoFrame(sprite.BankIndex, sprite.Frame);
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
        float factor = g_EngineInterpolationSets[*(long*)sprite.InterpolationSet].Factor;
        C2DVector offset = (sprite.UVOffsetsEnd - sprite.UVOffsetsStart) * factor + sprite.UVOffsetsStart;
        uvs.U[0] += offset.X;
        uvs.V[0] += offset.Y;
        uvs.U[1] += offset.X;
        uvs.V[1] += offset.Y;
        uvs.U[2] += offset.X;
        uvs.V[2] += offset.Y;
        uvs.U[3] += offset.X;
        uvs.V[3] += offset.Y;
    } else {
        uvs.U[0] = 0.0f; uvs.V[0] = 0.0f;
        uvs.U[1] = 1.0f; uvs.V[1] = 0.0f;
        uvs.U[2] = 1.0f; uvs.V[2] = 1.0f;
        uvs.U[3] = 0.0f; uvs.V[3] = 1.0f;
    }
    if (texture) {
        if (texture->GetPixelFormat().IsLinear()) {
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

        }
    }

    const CTexture* alpha_mask;
    const C2DClipRegion& mask_region = Peek2DClipRegion();
    if (mask_region.MaskBankIndex && mask_region.Owner) {
        const CGraphicDataBankFull* bank = g_pResourceManager->SpriteBank;
        const CGraphicInfoFrame* mask_info = bank->PeekGraphicInfoFrame(mask_region.MaskBankIndex, 0);

        CGraphicFrame mask;
        if (!mask_info->GetFrame(mask, 0, flags))
            return;
        alpha_mask = mask->PTexture.Data;
    } else {
        alpha_mask = &g_pResourceManager->SatRGBATexture;
    }

    float half_width = (sprite.ClipBox.BRX - sprite.ClipBox.TLX) * 0.5f;
    float half_height = (sprite.ClipBox.BRY - sprite.ClipBox.TLY) * 0.5f;
    float angle = (sprite.EndAngle - sprite.StartAngle) * g_EngineInterpolationSets[*(long*)sprite.InterpolationSet].Factor + sprite.StartAngle;
    float cosine = GFFastCos(angle);
    float sine = GFFastSin(angle);
    C2DVector across(cosine * half_width, -(half_width * sine));
    C2DVector down(-(sine * half_height), -(cosine * half_height));
    C2DVector centre(half_width + sprite.ClipBox.TLX, half_height + sprite.ClipBox.TLY);
    C2DVector corner0 = centre - across + down;
    C2DVector corner1 = centre + across + down;
    C2DVector corner2 = centre + across - down;
    C2DVector corner3 = centre - across - down;
    const C2DExtentsI target = g_pDisplayManager->GetRenderTargetDimensions();
    float scale_x = 1.0f / (target.X * 0.5f);
    float scale_y = 1.0f / (target.Y * 0.5f);
    corner0.X = corner0.X * scale_x - 1.0f;  corner0.Y = 1.0f - corner0.Y * scale_y;
    corner1.X = corner1.X * scale_x - 1.0f;  corner1.Y = 1.0f - corner1.Y * scale_y;
    corner2.X = corner2.X * scale_x - 1.0f;  corner2.Y = 1.0f - corner2.Y * scale_y;
    corner3.X = corner3.X * scale_x - 1.0f;  corner3.Y = 1.0f - corner3.Y * scale_y;
    unsigned long colour = sprite.Colour.IntValue;
    colour = ((colour >> 1) & 0x7f7f7f) | (colour & 0xff000000);
    float z = g_pPrimitiveRenderer2D->Layer2DDistanceToZ(g_pPrimitiveRenderer2D->Layer2DToDistance((EEngine2DLayer)sprite.Layer));

    CEngineSubPrimitive2DSprite* sub = new CEngineSubPrimitive2DSprite(&sprite);
    sub->Vertices[0].X = corner0.X; sub->Vertices[0].Y = corner0.Y; sub->Vertices[0].Z = z; sub->Vertices[0].DiffuseColour = colour; sub->Vertices[0].U1 = uvs.U[0]; sub->Vertices[0].V1 = uvs.V[0];
    sub->Vertices[1].X = corner1.X; sub->Vertices[1].Y = corner1.Y; sub->Vertices[1].Z = z; sub->Vertices[1].DiffuseColour = colour; sub->Vertices[1].U1 = uvs.U[1]; sub->Vertices[1].V1 = uvs.V[1];
    sub->Vertices[2].X = corner2.X; sub->Vertices[2].Y = corner2.Y; sub->Vertices[2].Z = z; sub->Vertices[2].DiffuseColour = colour; sub->Vertices[2].U1 = uvs.U[2]; sub->Vertices[2].V1 = uvs.V[2];
    sub->Vertices[3].X = corner3.X; sub->Vertices[3].Y = corner3.Y; sub->Vertices[3].Z = z; sub->Vertices[3].DiffuseColour = colour; sub->Vertices[3].U1 = uvs.U[3]; sub->Vertices[3].V1 = uvs.V[3];
    sub->Texture = texture;
    sub->BlendMode = sprite.BlendMode;
    sub->AlphaMask = alpha_mask;
    sub->Filter = sprite.Filter;
    sub->WrapTexture = sprite.WrapTexture;
    sub->Alpha = 0xff;
    sub->RenderFlags = 1;
    if (sprite.EnginePrimitiveFlags & 0x400) {
        if (g_pEngineSettings->DisplacementPassEnabled) {
            g_pEngineSubPrimitiveRenderer->AddSubPrimitive(5, sub, g_pPrimitiveRenderer2D->Layer2DToSortDistance((EEngine2DLayer)sprite.Layer), (CEnginePrimitiveManager*)this);
            CEngineSubPrimitive2DSprite* copy = new CEngineSubPrimitive2DSprite(&sprite);
            for (long n = 0; n < 4; ++n)
                copy->Vertices[n] = sub->Vertices[n];
            copy->Texture = sub->Texture;
            copy->AlphaMask = sub->AlphaMask;
            copy->Filter = sub->Filter;
            copy->WrapTexture = sub->WrapTexture;
            copy->BlendMode = sub->BlendMode;
            g_pEngineSubPrimitiveRenderer->AddSubPrimitive(6, copy, g_pPrimitiveRenderer2D->Layer2DToSortDistance((EEngine2DLayer)sprite.Layer), (CEnginePrimitiveManager*)this);
        }
    } else {
        g_pEngineSubPrimitiveRenderer->AddSubPrimitive(type, sub, g_pPrimitiveRenderer2D->Layer2DToSortDistance((EEngine2DLayer)sprite.Layer), (CEnginePrimitiveManager*)this);
    }
    if (g_pPrimitiveRenderer->DrawDebug2DBoxes) {
        C2DBoxF box;
        box.TLX = GFAXToVX(sprite.ClipBox.TLX);
        box.TLY = GFAYToVY(sprite.ClipBox.TLY);
        box.BRX = GFAXToVX(sprite.ClipBox.BRX);
        box.BRY = GFAYToVY(sprite.ClipBox.BRY);
        CRGBColour red;
        red.B = 0; red.G = 0; red.R = 0xff; red.A = 0xff;
        g_pRenderManager2D->Draw2DBoxOutline(box, red, 0);
    }
}
