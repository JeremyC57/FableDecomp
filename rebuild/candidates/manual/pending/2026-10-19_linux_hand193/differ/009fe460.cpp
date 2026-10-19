// CGraphicInfoFrame::CFrameData::FreeTextureMemory(bool force) -- texture memory pressure: a frame unused long enough
// Grade: DIFFER(ours v retail) -- 231v231: eax/ecx/edx rotation only -- the threshold chain loads GraphicInfo/bank into ecx/edx (retail edx/eax), the SmallMipmapLevel compare uses eax (retail ecx) and the texture manager is loaded after the pushes (retail before)
// drops one mipmap level when allowed (true), or with `force` resets its texture and discards the frame's device
// resources (true); otherwise only refresh the recorded resource size (false)
// Leaf: FreeTextureMemory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FE460, length 230 bytes (padded to 240), `ret 4`.
// Callees: CResource::GetUnusedFrames @ 0x009FBF70 (on the managed texture's data and on `this`),
// CTextureManager::ReduceMipmapLevel(CGraphicFrame*, CManagedTexture*, unsigned long) @ 0x00A69F70,
// CFrameData::Sizeof @ 0x009FDBE0 (the inline member emitted out of line for its first call), CResource::
// SetResourceSize @ 0x009FBF20, CFrameData::ResetTexture @ 0x009FE440, CGraphicInfoFrame::
// DiscardDeviceIndependentResources @ 0x00A00680 (labelled CAnimatedBlock), CGraphicFrameData::Sizeof @ 0x009FCBF0,
// CManagedTexture::Sizeof @ 0x00A69E60. Graphics layer.
// Layout: see 0x009FDCB0 (CFrameData derives from CResource; ManagedTexture +0x20 with its CResource data +0x24,
// CurrentMipmapLevel +0x38); CGraphicInfoFrame GraphicInfo +0xC; CGraphicInfo flags byte +0x12, data bank +0x20;
// CGraphicDataBank unused-frame thresholds +0x26C (free) / +0x270 (reduce), texture manager +0x280.
// Shape: the reduce threshold read into a local before the texture's GetUnusedFrames call, this object's unused-frame
// count a local before the free-threshold chain; the level a `long + 1`
// compared unsigned (`ja`); Sizeof() is an in-class inline called twice (out of line in the reduce arm, inlined in the
// final arm).

class CSurface {
public:
    virtual void v00();
    void* PD3DSurface;
    unsigned long AllocationSource;
    void* PAllocatedMemory;
    ~CSurface() { Uninitialise(); }
    void Uninitialise();                            // 0x009F2E20
    unsigned long GetByteLength() const;            // 0x009F2FC0
};

class CTexture {
public:
    long GetNoLevels() const;                       // 0x009F9DF0
    CSurface GetMipmapSurface(long level) const;    // 0x009F9E00
};

class CGraphicFrameData {
public:
    unsigned char _pad[8];
    CTexture* Texture;                              // +0x08
    unsigned long Sizeof() const;                   // 0x009FCBF0
};
class CGraphicFrame {
public:
    CGraphicFrameData* FrameData;
    CTexture* GetTexture() const { return FrameData->Texture; }
    unsigned long Sizeof() const { return FrameData ? FrameData->Sizeof() + sizeof(*this) : sizeof(*this); }
};
class CResource;
class CManagedTexture {
public:
    void* __vftable;
    CResource* Data;
    unsigned long Sizeof() const;                   // 0x00A69E60
};

class CResource {
public:
    virtual void v00();
    unsigned long GetUnusedFrames() const;          // 0x009FBF70
    void SetResourceSize(unsigned long size);       // 0x009FBF20
};
class CTextureManager {
public:
    bool ReduceMipmapLevel(CGraphicFrame* frame, CManagedTexture* texture, unsigned long levels);   // 0x00A69F70
};
class CGraphicDataBank {
public:
    unsigned char _pad[0x26c];
    unsigned long MaxUnusedFramesBeforeFree;        // +0x26C
    unsigned long MinUnusedFramesBeforeReduce;      // +0x270
    unsigned char _pad274[0xc];
    CTextureManager* TextureManager;                // +0x280
};
class CGraphicInfo {
public:
    unsigned char _pad[0x12];
    unsigned char Flags;                            // +0x12 (bit 0: keep every mipmap level)
    unsigned char _pad13[0xd];
    CGraphicDataBank* Bank;                         // +0x20
};
class CGraphicInfoFrame {
public:
    void* __vftable;
    unsigned short FrameNum;                        // +0x04
    signed char SmallMipmapLevel;                   // +0x06
    unsigned char _pad07[5];
    CGraphicInfo* GraphicInfo;                      // +0x0C
    void DiscardDeviceIndependentResources();       // 0x00A00680

    class CFrameData : public CResource {
    public:
        unsigned char _base[0x18];
        CGraphicFrame Frame;                        // +0x1C
        CManagedTexture ManagedTexture;             // +0x20 (Data +0x24 is a CResource)
        CManagedTexture LoadingManagedTexture;      // +0x28
        void* FileData[2];                          // +0x30
        signed char CurrentMipmapLevel;             // +0x38
        signed char LoadingMipmapLevel;             // +0x39
        signed char TouchedMipmapLevel;             // +0x3A
        unsigned long TouchedFrame;                 // +0x3C
        CGraphicInfoFrame* GraphicInfoFrame;        // +0x40
        unsigned long Sizeof() const
        {
            unsigned long size = Frame.Sizeof() + sizeof(*this);
            if (LoadingManagedTexture.Data)
                size += LoadingManagedTexture.Sizeof();
            return size;
        }
        void ResetTexture();                        // 0x009FE440
        bool FreeTextureMemory(bool force);
    };
};

bool CGraphicInfoFrame::CFrameData::FreeTextureMemory(bool force)
{
    if (Frame.FrameData) {
        unsigned long reduceAfter = GraphicInfoFrame->GraphicInfo->Bank->MinUnusedFramesBeforeReduce;
        if (ManagedTexture.Data->GetUnusedFrames() >= reduceAfter) {
            unsigned long unused = GetUnusedFrames();
            if (unused <= GraphicInfoFrame->GraphicInfo->Bank->MaxUnusedFramesBeforeFree)
                return false;
            unsigned long level = CurrentMipmapLevel + 1;
            if (!(GraphicInfoFrame->GraphicInfo->Flags & 1) && level <= (unsigned long)GraphicInfoFrame->SmallMipmapLevel) {
                GraphicInfoFrame->GraphicInfo->Bank->TextureManager->ReduceMipmapLevel(&Frame, &ManagedTexture, 1);
                CurrentMipmapLevel = (signed char)level;
                SetResourceSize(Sizeof());
                return true;
            }
            if (force) {
                ResetTexture();
                GraphicInfoFrame->DiscardDeviceIndependentResources();
                return true;
            }
        }
    }
    SetResourceSize(Sizeof());
    return false;
}
