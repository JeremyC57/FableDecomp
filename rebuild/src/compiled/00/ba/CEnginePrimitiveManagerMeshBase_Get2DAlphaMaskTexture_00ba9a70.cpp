// CEnginePrimitiveManagerMeshBase::Get2DAlphaMaskTexture(unsigned long bankIndex, unsigned long mipmap, bool& preloading, const CGraphicDataBank* bank, bool force) -- the texture of a 2D alpha-mask graphic frame at the wanted mipmap: the engine resource manager's saturated RGBA texture when there is no mask, else the per-frame GetFrame body of CGraphicInfoFrame (touch the resource LRU, begin loading, force the load when the renderer blocks on loads or `force` is set) returning the frame's texture, or the saturated texture while it is still loading
// Leaf: ?Get2DAlphaMaskTexture@CEnginePrimitiveManagerMeshBase@@SIPBVCTexture@@KKAA_NPBVCGraphicDataBank@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA9A70, 563 bytes, static __fastcall (bankIndex ecx, mipmap edx; preloading, bank, force on the
//   stack; `ret 0Ch`); `preloading` is not read. Speed TU (no pragma).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?Get2DAlphaMaskTexture@CEnginePrimitiveManagerMeshBase@@SIPBVCTexture@@KKAA_NPBVCGraphicDataBank@@_N@Z
//   (Ego_r donor order GetDiffuseTexture 00a5fff0 / GetBumpTexture 00a60220 / GetReflectionTexture 00a60440 / Get2DAlphaMaskTexture
//   00a60640 = retail 0x00BA9360 / 0x00BA95D0 / 0x00BA9830 / 0x00BA9A70); the body is the parked GetReflectionTexture 0x00BA9830
//   model (2026-10-27_linux_hand683) with the frame-less result taken from the engine resource manager instead of the bank, and
//   the eight callers 0x00BA323C.. 0x00BD4AA6 are the 2D sprite / mesh managers. Catalog "_global::Get2DAlphaMaskTexture" is right
//   but for the class.
// Callees: CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90, CFrameData::BeginLoading 0x009FDE80, CFrameData::CFrameData(
//   const CGraphicInfoFrame*) 0x009FE380 (operator new 0x00BFEA1A, 0x44 bytes), CGraphicInfoFrame::InternalGetFrameForcedLoading
//   0x009FF6B0; globals 0x01436E80 (((CEnginePrimitiveRendererSharedData*)g_pEnginePrimitiveRenderer), BlockOnLoad +4) and 0x01436E98 (the CEngineResourceManager
//   singleton: retail +0x2ADC is SatRGBATexture -- Ego_r 14876 / engine header +0x2d0c, retail = header - 0x230 as the landed
//   BuildHWStipplePatterns 0x00B3C4B0 pins HWInverseStipplePatterns at +0x2AB4; the mask's "blank" is the all-ones texture).
// Shape: as the Reflection differ (STLport-style vector member for the double size computation; `flags = 0; if (BlockOnLoad ||
//   force) flags = 0x11;`), the frame body inlined per row because its frame-less result differs between the four siblings.
#include "fable_compiler.h"
class CTexture;
enum ETextureType { TEXTURE_TYPE_0 = 0 };
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
    FABLE_FORCEINLINE void Touch();
};
class CResourceList { public:                                 // Ego_r.pdb, 0x3C bytes
    unsigned long _pad00;
    CResource     Head;                                // +0x04
    unsigned long ResourceCount;                       // +0x20
    long          AllocatedMemory;                     // +0x24
    long          MaximumMemory;                       // +0x28
    unsigned long CurrentFrame;                        // +0x2c
};
FABLE_FORCEINLINE void CResource::Touch()
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
    FABLE_FORCEINLINE ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    FABLE_FORCEINLINE void DecRef()
    {
        if (--ReferenceCount == 0) {
            this->~CGraphicFrameData();
            operator delete(this);
        }
    }
};
class CGraphicFrame { public:                                 // intrusive pointer to CGraphicFrameData
    CGraphicFrameData* FrameData;
    FABLE_FORCEINLINE CGraphicFrame& operator=(const CGraphicFrame& other)
    {
        if (FrameData)
            FrameData->DecRef();
        FrameData = other.FrameData;
        if (FrameData)
            ++FrameData->ReferenceCount;
        return *this;
    }
};
class CManagedTexture { public:                               // Ego_r.pdb, 8 bytes
    unsigned long _pad00;
    CResource*    Data;                                // +0x04 CIVCountedPointer<CManagedTextureData>
    FABLE_FORCEINLINE void Touch() { if (Data) Data->Touch(); }
};
class CGraphicInfoFrame;
class CGraphicDataBank { public:
    unsigned char _pad00[0x194];
    unsigned long CurrentFrame;                        // +0x194
};
template <class T> struct CFrameDataPointer {                             // CIVCountedPointer<CFrameData>
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
class CGraphicInfo;
class CGraphicInfoFrame { public: class CFrameData; public:                             // Ego_r.pdb, 16 bytes
    unsigned long     _pad00;
    unsigned short    FrameNum;                        // +0x04
    signed char       SmallMipmapLevel;                // +0x06
    CFrameDataPointer<CFrameData> Data;                            // +0x08
    CGraphicInfo*     GraphicInfo;                     // +0x0c
private:
    bool InternalGetFrameForcedLoading(unsigned long flags) const;   // 0x009FF6B0 (Ego_r.pdb ?InternalGetFrameForcedLoading@CGraphicInfoFrame@@ABE_NK@Z, private)   // 0x009FF6B0
public:
    FABLE_FORCEINLINE const CTexture* GetTexture(unsigned long mipmapLevel, unsigned long flags);   // per-row inline body (the frame-less result differs between the four siblings)
    const CTexture* GetBlankTexture() const;                      // 0x009FD3F0 ?GetBlankTexture@CGraphicInfoFrame@@QBEPBVCTexture@@XZ
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
    FABLE_FORCEINLINE void DecRef() { if (--RefCount == 0) Release(); }
};   // Ego_r.pdb: nested in CGraphicInfoFrame (RTTI .?AVCFrameData@CGraphicInfoFrame@@), 0x44 bytes
typedef CGraphicInfoFrame::CFrameData CFrameData;   // the row's free code keeps the unqualified name


class CGraphicInfo { public:                                  // Ego_r.pdb, 0x2c bytes
    unsigned char      _pad00[0x1c];                   // +0x00 CGraphicHeader Header
    ETextureType       TextureType;                    // +0x1c
    CGraphicDataBank*  Bank;                           // +0x20
    long               Index;                          // +0x24
    CGraphicInfoFrame* Frames;                         // +0x28
};
template<class T> struct SVector {                  // STLport std::vector<T> (start, finish, end of storage)
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    FABLE_FORCEINLINE unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    FABLE_FORCEINLINE const T& operator[](unsigned long n) const { return *(_M_start + n); }
};
class CGraphicDataBankFull : public CGraphicDataBank { public:   // the bank's graphic list (std::vector<CGraphicInfo>) and blank textures
    unsigned char      _pad01[0x1e0 - 0x198];
    SVector<CGraphicInfo> GraphicList;                 // +0x1e0
    unsigned char      _pad02[0x2b4 - 0x1ec];
    const CTexture* GetBlankTexture(ETextureType type) const;   // 0x009FD150 ?GetBlankTexture@CGraphicDataBank@@QBEPBVCTexture@@W4ETextureType@1@@Z
    FABLE_FORCEINLINE unsigned long GetNoGraphics() const { return GraphicList.size(); }
    FABLE_FORCEINLINE const CGraphicInfo* GetGraphicInfo(unsigned long index) const { return index < GraphicList.size() ? &GraphicList[index] : 0; }
};
class C3DMeshMaterial { public:                              // Ego_r.pdb, 0x30 bytes
    void*          __vftable;
    long           MaterialIndex;                      // +0x04
    void*          Name;                               // +0x08 CCharString
    long           DecalID;                            // +0x0c
    unsigned long  DiffuseMapBankIndex;                // +0x10
    unsigned long  BumpMapBankIndex;                   // +0x14
    unsigned long  ReflectionMapBankIndex;             // +0x18
};
struct CEnginePrimitiveRendererSharedData { void* _pad00; bool BlockOnLoad; };   // +0x04
class CEnginePrimitiveRenderer;
extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;   // 0x01436E80 (the engine's primitive-renderer component; Codex name fix 2026-11-10)
// The members read here are modelled by the local CEnginePrimitiveRendererSharedData view of that object (name debt: the shared
// engine/CEnginePrimitiveRenderer.h retail branch).


struct IDirect3DTexture9;
class CTexture { public: IDirect3DTexture9* PD3DTexture; unsigned long Flags; };   // engine/CTexture.h, 8 bytes
struct CEngineResourceManager_01436e98 {                  // the engine resource manager singleton, SatRGBATexture at retail +0x2ADC
    unsigned char _pad00[0x2adc];
    CTexture      SatRGBATexture;                       // +0x2adc (Ego_r 14876; retail = generated header - 0x230)
};
extern CEngineResourceManager_01436e98* g_pEngineResourceManager_01436e98;   // 0x01436E98

struct CEnginePrimitiveManagerMeshBase {
    static const CTexture* __fastcall Get2DAlphaMaskTexture(unsigned long bankIndex, unsigned long mipmap, bool& preloading, const CGraphicDataBank* bank, bool force);
};

FABLE_FORCEINLINE const CTexture* CGraphicInfoFrame::GetTexture(unsigned long mipmapLevel, unsigned long flags)
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
                if (!(flags & 0x10) || !Data->FileData[0])
                    return Data->Frame.FrameData->PTexture.Data;
            } else {
                return Data->Frame.FrameData->PTexture.Data;
            }
        }
        Data->BeginLoading(mipmapLevel, flags);
    } else {
        if (flags & 4)
            return &g_pEngineResourceManager_01436e98->SatRGBATexture;
        Data = new CFrameData(this);
        Data->BeginLoading(mipmapLevel, flags);
    }
    if (!(flags & 1) || !InternalGetFrameForcedLoading(flags))
        return &g_pEngineResourceManager_01436e98->SatRGBATexture;
    return Data->Frame.FrameData->PTexture.Data;
}

const CTexture* __fastcall CEnginePrimitiveManagerMeshBase::Get2DAlphaMaskTexture(unsigned long bankIndex, unsigned long mipmap, bool& preloading, const CGraphicDataBank* bank, bool force)
{
    const CGraphicDataBankFull* fullBank = static_cast<const CGraphicDataBankFull*>(bank);
    if (bankIndex == 0)
        return &g_pEngineResourceManager_01436e98->SatRGBATexture;
    CGraphicInfoFrame* frame = fullBank->GetGraphicInfo(bankIndex) ? fullBank->GetGraphicInfo(bankIndex)->Frames : 0;
    unsigned long flags = (((CEnginePrimitiveRendererSharedData*)g_pEnginePrimitiveRenderer)->BlockOnLoad || force) ? 0x11 : 0;
    return frame->GetTexture(mipmap, flags);
}