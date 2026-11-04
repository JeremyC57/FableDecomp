// NUISystem::CMorphingSprite::Draw(CIEngine& engine, CEnginePrimitiveHandle* parentHandle, long layer, long* childIndex, CComponent* parent) -- morphing sprite render: one engine primitive per render pass (RenderCount picks / grows the Handles list), the current graphic (GraphicIndices[TargetState] or SpriteGraphicIndex) preloaded, then a 2D sprite primitive set as a child of the parent handle or as the pass's persistent primitive
// Grade: DIFFER(1524v1572) -- residue: (1) VC7.1's per-site inliner calls CEnginePrimitiveHandle::Clear (Handle.Clear at the RenderCount==0 exit) and the handle ctor (the NoRender push_back) out of line where retail expands both (c2 static frequency too low there; a do{}while(0) or a __forceinline Clear/ctor closes it, 46 lines left); (2) then register colouring inside the inlined CGraphicInfoFrame::GetFrame touch block and the sprite argument loads; 73 diff lines, 27 grades
// Leaf: ?Draw@CMorphingSprite@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z
// Flags: none
// Retail: Fable.exe @ 0x00554740, 1572 bytes (`ret 14h`). Primary vtable 0x0124A49C slot 2.
// Retail-asm: 0x005549B5 `fld dword ptr [esp+14h]` + 0x005549B9 `fistp dword ptr [esp+18h]` (and 0x005549C7 / 0x005549CB for X): the engine's
//   GFFloatToLongNear inline asm (value copied to the helper's frame slot, bare dword fistp); C++ attempts table in NOTES.md.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIENGINE
#include "engine/NUISystem_CMorphingSprite.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitive.h"
#include "engine/C2DBoxF.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CArray.h"
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

// ---- graphics bank model (the landed CGraphicInfoFrame::GetFrame 0x0041C260 model; header debt) ----
class CResourceList;
class CResource { public:
    virtual void v00();
    virtual void Release();
    unsigned long  RefCount;                                // +0x04
    CResourceList* ResourceList;                            // +0x08
    CResource*     PrevResource;                            // +0x0c
    CResource*     NextResource;                            // +0x10
    unsigned long  ResourceSize;                            // +0x14
    unsigned long  LastUsedFrame;                           // +0x18
private:
    void ListRemove();                                      // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);      // 0x009FBF90
public:
    __forceinline void Touch();
};
class CResourceList { public:
    unsigned long _pad00;
    CResource     Head;                                     // +0x04
    unsigned long ResourceCount;                            // +0x20
    long          AllocatedMemory;                          // +0x24
    long          MaximumMemory;                            // +0x28
    unsigned long CurrentFrame;                             // +0x2c
};
__forceinline void CResource::Touch()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}
struct CCPInfo { long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
struct CTexturePointer {
    void* Data;
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
class CGraphicFrameData { public:
    unsigned long   ReferenceCount;                         // +0x00
    void*           Data;                                   // +0x04
    CTexturePointer PTexture;                               // +0x08
    unsigned short  TLX, TLY, Width, Height, Pitch;         // +0x10
    unsigned char   Alpha : 1;                              // +0x1a
    unsigned char   BooleanAlpha : 1;
    unsigned char   UsesSurface : 1;
    unsigned char   SectionOfTPage : 1;
    unsigned char   IsInitialised : 1;
    unsigned char   ScreenGraphic : 1;
    void Uninitialise();                                    // 0x009FCBA0
    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    __forceinline void DecRef()
    {
        if (--ReferenceCount == 0) {
            this->~CGraphicFrameData();
            operator delete(this);
        }
    }
};
class CGraphicFrame { public:
    CGraphicFrameData* FrameData;
    CGraphicFrame() : FrameData(0) { }
    __forceinline ~CGraphicFrame() { if (FrameData) FrameData->DecRef(); }
    __forceinline CGraphicFrame& operator=(const CGraphicFrame& other)
    {
        if (FrameData)
            FrameData->DecRef();
        FrameData = other.FrameData;
        if (FrameData)
            ++FrameData->ReferenceCount;
        return *this;
    }
};
class CManagedTexture { public:
    unsigned long _pad00;
    CResource*    Data;                                     // +0x04
    __forceinline void Touch() { if (Data) Data->Touch(); }
};
class CGraphicInfoFrame;
class CGraphicDataBank;
class CGraphicInfo { public:                                // Ego_r.pdb, 0x2C bytes
    unsigned char      Header[0x1c];                        // +0x00 CGraphicHeader
    long               TextureType;                         // +0x1c
    CGraphicDataBank*  Bank;                                // +0x20
    long               Index;                               // +0x24
    CGraphicInfoFrame* Frames;                              // +0x28
    const CGraphicInfoFrame* PeekInfoFrame(unsigned long n) const;
};
class CGraphicDataBank { public:
    unsigned char _pad00[0x194];
    unsigned long CurrentFrame;                             // +0x194
    unsigned char _pad198[0x1e0 - 0x198];
    CArray<CGraphicInfo> GraphicList;                       // +0x1e0
    const CGraphicInfo* PeekGraphic(unsigned long n) const { return n < GraphicList.size() ? &GraphicList[n] : 0; }
    const CGraphicInfoFrame* PeekGraphicInfoFrame(unsigned long n, unsigned long frame) const;
};
template <class T> struct CFrameDataPointer {
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
class CGraphicInfoFrame { public: class CFrameData; public:
    unsigned long     _pad00;
    unsigned short    FrameNum;                             // +0x04
    signed char       SmallMipmapLevel;                     // +0x06
    mutable CFrameDataPointer<CFrameData> Data;             // +0x08
    CGraphicInfo*     GraphicInfo;                          // +0x0c
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags) const;   // 0x009FE620
private:
    bool InternalGetFrameForcedLoading(unsigned long flags) const;            // 0x009FF6B0
public:
    bool GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags) const;
};
class CGraphicInfoFrame::CFrameData : public CResource { public:
    CGraphicFrame   Frame;                                  // +0x1c
    CManagedTexture ManagedTexture;                         // +0x20
    CManagedTexture LoadingManagedTexture;                  // +0x28
    void*           FileData[2];                            // +0x30
    signed char     CurrentMipmapLevel;                     // +0x38
    signed char     LoadingMipmapLevel;                     // +0x39
    signed char     TouchedMipmapLevel;                     // +0x3a
    unsigned long   TouchedFrame;                           // +0x3c
    CGraphicInfoFrame* GraphicInfoFrame;                    // +0x40
    CFrameData(const CGraphicInfoFrame* frame);             // 0x009FE380
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FDE80
    __forceinline void DecRef() { if (--RefCount == 0) Release(); }
};
inline const CGraphicInfoFrame* CGraphicInfo::PeekInfoFrame(unsigned long n) const { return Frames + n; }
__forceinline const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long n, unsigned long frame) const
{
    return PeekGraphic(n) ? PeekGraphic(n)->PeekInfoFrame(frame) : 0;
}
__forceinline bool CGraphicInfoFrame::GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags) const
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

enum EEnginePrimitiveBlendMode { };
class CEnginePrimitive2DSprite : public CEnginePrimitive { public:     // Ego_r.pdb, 0x54 bytes
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box, EEngine2DLayer layer, unsigned long bankIndex,
        unsigned long frame, EEnginePrimitiveBlendMode blendMode, EEngineInterpolationSet interpolationSet,
        float startAngle, float endAngle, unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue,
        unsigned long flags, const C2DVector& uvOffsetsStart, const C2DVector& uvOffsetsEnd);   // 0x0041BEB0
    unsigned char _body[0x54 - 0xc];
};

namespace NUISystem {
void CMorphingSprite::Draw(CIEngine& engine, CEnginePrimitiveHandle* parentHandle, long layer, long* childIndex, CComponent* parent)
{
    if (RenderColour.A > 0 && (!(RenderZoom.X <= 0.0f) || !(RenderZoom.Y <= 0.0f))) {
        NoRenderNextFrame = 0;
    } else {
        if (NoRenderNextFrame == 1) {
            if (RenderCount == 0) {
                if (parentHandle == 0) {
                    Handle.Clear();
                    return;
                }
            } else {
                if (Handles.size() >= (unsigned long)RenderCount) {
                    std::list<CEnginePrimitiveHandle*>::iterator it = Handles.begin();
                    for (unsigned long i = 0; i < (unsigned long)(RenderCount - 1) && it != Handles.end(); ++i)
                        ++it;
                    (*it)->Clear();
                    delete *it;
                    Handles.erase(it);
                } else {
                    Handles.push_back(new CEnginePrimitiveHandle);
                }
                if (parentHandle == 0)
                    return;
            }
            engine.ClearChildPrimitive(*parentHandle, (*childIndex)++);
            return;
        }
        NoRenderNextFrame = 1;
    }
    CEnginePrimitiveHandle* drawHandle;
    if (RenderCount > 0) {
        if (Handles.size() < (unsigned long)RenderCount) {
            drawHandle = new CEnginePrimitiveHandle;
            Handles.push_back(drawHandle);
        } else {
            std::list<CEnginePrimitiveHandle*>::iterator it = Handles.begin();
            for (unsigned long i = 0; i < (unsigned long)(RenderCount - 1) && it != Handles.end(); ++i)
                ++it;
            drawHandle = *it;
        }
    } else {
        drawHandle = &Handle;
    }
    ++RenderCount;
    C2DVector position((float)GFFloatToLongNear(RelativeRenderPosition.X), (float)GFFloatToLongNear(RelativeRenderPosition.Y));
    C2DVector size(Size.X * RelativeRenderZoom.X, Size.Y * RelativeRenderZoom.Y);
    C2DBoxF box(position.X, position.Y, position.X + size.X, position.Y + size.Y);
    CRGBColour colour = RenderColour;
    bool independent = IsIndependent();
    bool layerIndependent = IsLayerIndependent();
    EEngine2DLayer engineLayer;
    if (!independent && !layerIndependent)
        engineLayer = CManager::GetInstance()->GetLayer(Layer + layer);
    else
        engineLayer = CManager::GetInstance()->GetLayer(Layer);
    unsigned long graphicIndex = GraphicIndices[TargetState];
    if (SpriteGraphicIndex != 0)
        graphicIndex = SpriteGraphicIndex;
    if (graphicIndex > 0) {
        const CGraphicInfoFrame* frame = CManager::GetInstance()->GetGraphicsBank()->PeekGraphicInfoFrame(graphicIndex, 0);
        frame->BeginLoading(0, 0x10);
        CGraphicFrame graphicFrame;
        frame->GetFrame(graphicFrame, 0, 0x11);
    }
    CEnginePrimitive2DSprite sprite(0, box, engineLayer, graphicIndex, 0, (EEnginePrimitiveBlendMode)2, (EEngineInterpolationSet)0,
        0.0f, 0.0f, colour.A, colour.R, colour.G, colour.B, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    if (parentHandle && childIndex)
        engine.SetChildPrimitive(*parentHandle, (*childIndex)++, sprite, 0xc0);
    else
        engine.SetPersistentPrimitive(*drawHandle, sprite, 0xc0);
}
}
