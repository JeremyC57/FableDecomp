// CEnginePrimitiveManagerMeshBase::PreloadReflectionTexture(const C3DMeshMaterial*, unsigned long, bool&, const CGraphicDataBank*) -- preload a material's reflection map
// Leaf: ?PreloadReflectionTexture@CEnginePrimitiveManagerMeshBase@@
#include <new>
class CResource;
class CResourceList {
public:
    void*          __vftable;                            // +0x00
    unsigned char  HeadStorage[0x1c];                    // +0x04 CResource Head (PrevResource at +0x10)
    unsigned long  ResourceCount;                        // +0x20
    long           AllocatedMemory;                      // +0x24
    long           MaximumMemory;                        // +0x28
    unsigned long  CurrentFrame;                         // +0x2c
};
class CResource {
public:
    virtual void v00();
    virtual void v01();
    long           RefCount;                             // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0c
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
    void ListInsert(CResource* prev, CResource* next);   // 0x009FBF90
    void ListRemove();                                   // 0x009FBFB0
    __forceinline void Touch()
    {
        if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
            ListRemove();
            ListInsert(*(CResource**)(ResourceList->HeadStorage + 0x0c), (CResource*)ResourceList->HeadStorage);
            LastUsedFrame = ResourceList->CurrentFrame;
        }
    }
};
class CManagedTexture {
public:
    void*          __vftable;                            // +0x00
    CResource*     Data;                                 // +0x04 CIVCountedPointer<CManagedTextureData>
    __forceinline void Touch() { if (Data) Data->Touch(); }
};
class CGraphicInfoFrame;
class CFrameData : public CResource {
public:
    void*          Frame;                                // +0x1c CGraphicFrame
    CManagedTexture ManagedTexture;                      // +0x20
    CManagedTexture LoadingManagedTexture;               // +0x28
    unsigned char  FileData[8];                          // +0x30
    signed char    CurrentMipmapLevel;                   // +0x38
    signed char    LoadingMipmapLevel;                   // +0x39
    signed char    TouchedMipmapLevel;                   // +0x3a
    unsigned long  TouchedFrame;                         // +0x3c
    CGraphicInfoFrame* GraphicInfoFrame;                 // +0x40
    explicit CFrameData(CGraphicInfoFrame* frame);       // 0x009FE380
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FDE80
};
struct CGraphicLoadingContext { unsigned char _pad[0x194]; unsigned long CurrentFrame; };
struct CGraphicInfo { unsigned char _pad[0x20]; CGraphicLoadingContext* Bank; };
class CGraphicInfoFrame {
public:
    void*          __vftable;                            // +0x00
    unsigned short FrameNum;                             // +0x04
    signed char    SmallMipmapLevel;                     // +0x06
    CFrameData*    Data;                                 // +0x08 CIVCountedPointer<CFrameData>
    CGraphicInfo*  GraphicInfo;                          // +0x0c
    __forceinline long GetCurrentLoadedMipmapLevel() const { return Data ? Data->CurrentMipmapLevel : -1; }
};
struct CGraphicDataBankEntry { unsigned char _pad[0x28]; CGraphicInfoFrame* InfoFrame; };
class CGraphicDataBank {
public:
    unsigned char  _pad[0x1e0];
    CGraphicDataBankEntry* EntriesBegin;                 // +0x1e0
    CGraphicDataBankEntry* EntriesEnd;                   // +0x1e4
    __forceinline unsigned long Size() const { return EntriesEnd - EntriesBegin; }
    __forceinline const CGraphicDataBankEntry* GetEntry(unsigned long i) const { return Size() > i ? &EntriesBegin[i] : 0; }
};
class C3DMeshMaterial {
public:
    void*          __vftable;
    long           MaterialIndex;                        // +0x04
    void*          Name;                                 // +0x08
    long           DecalID;                              // +0x0c
    unsigned long  DiffuseMapBankIndex;                  // +0x10
    unsigned long  BumpMapBankIndex;                     // +0x14
    unsigned long  ReflectionMapBankIndex;               // +0x18
};
class CEnginePrimitiveManagerMeshBase {
public:
    static unsigned long __fastcall GetAcceptablePreloadMipmap(unsigned long mipmap, const CGraphicInfoFrame* frame);   // 0x00BEBBE0
    static void __fastcall PreloadReflectionTexture(const C3DMeshMaterial* material, unsigned long mipmap, bool& preloading, const CGraphicDataBank* bank);
};

void __fastcall CEnginePrimitiveManagerMeshBase::PreloadReflectionTexture(const C3DMeshMaterial* material, unsigned long mipmap, bool& preloading, const CGraphicDataBank* bank)
{
    unsigned long index = material->ReflectionMapBankIndex;
    if (index == 0)
        return;
    if (index < bank->Size() && &bank->EntriesBegin[index]) {
        CGraphicInfoFrame* frame = bank->GetEntry(index)->InfoFrame;
        if (frame) {
            unsigned long acceptable = GetAcceptablePreloadMipmap(mipmap, frame);
            if (frame->Data) {
                if (frame->Data->Frame) {
                    unsigned long currentFrame = frame->GraphicInfo->Bank->CurrentFrame;
                    if (frame->Data->TouchedFrame != currentFrame || mipmap <= (unsigned long)frame->Data->TouchedMipmapLevel) {
                        frame->Data->TouchedFrame = currentFrame;
                        frame->Data->ManagedTexture.Touch();
                        if (mipmap <= (unsigned long)frame->Data->CurrentMipmapLevel)
                            frame->Data->Touch();
                        if ((unsigned long)frame->Data->TouchedMipmapLevel > mipmap)
                            frame->Data->TouchedMipmapLevel = (signed char)mipmap;
                    }
                    if (mipmap < (unsigned long)frame->Data->CurrentMipmapLevel)
                        frame->Data->BeginLoading(mipmap, 0);
                    if ((unsigned long)frame->GetCurrentLoadedMipmapLevel() > acceptable)
                        preloading = true;
                } else {
                    frame->Data->BeginLoading(mipmap, 0);
                    preloading = true;
                    return;
                }
            } else {
                CFrameData* data = new CFrameData(frame);
                CFrameData* old = frame->Data;
                if (old) {
                    if (--old->RefCount == 0)
                        old->v01();
                    frame->Data = 0;
                }
                frame->Data = data;
                data->BeginLoading(mipmap, 0);
                preloading = true;
                return;
            }
        }
    }
}
