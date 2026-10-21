// CGraphicInfoFrame::GetFrame(CGraphicFrame& frame, unsigned long mipLevel, unsigned long flags) const -- make the frame data resident for the requested mip level (creating and loading it on first use, touching its resources once per bank frame), hand back the loaded frame, or force-load it when asked
// Grade: DIFFER(419v417) -- two spots: retail reloads Data for the BankFrame store (movsx into ecx) and leaves a dead `add eax,20h` after loading the texture resource at +0x24; the frame-data release loads Data once into ecx and uses the dec flags, ours reloads. Six tries (inline helpers, goto-shared load path, Release() forms). `this` is CGraphicInfoFrame by the PDB.
// Leaf: GetFrame
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B66C10, 415 bytes, `ret 0xc`.
// Callees: CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90, CFrameData::BeginLoading 0x009FDE80,
//   operator new 0x00BFEA1A, CFrameData::CFrameData(CGraphicInfoFrame*) 0x009FE380, InternalGetFrameForcedLoading 0x009FF6B0;
//   the frame data's virtual slot 1 (destroy at zero references).
// Layout: `this` is CGraphicInfoFrame by the PDB (Data CIVCountedPointer<CFrameData> +8, GraphicInfo +0xc; the catalog's
//   CGraphicInfo label names the owner class); CFrameData is a 0x44-byte CResource (descriptive member names past +0x1c).
struct CResourceList;
struct CResource {
    virtual void v00();
    virtual void DeleteThis();             // slot 1
    unsigned long  RefCount;               // +4
    CResourceList* ResourceList;           // +8
    CResource*     PrevResource;           // +0xc
    CResource*     NextResource;           // +0x10
    unsigned long  ResourceSize;           // +0x14
    unsigned long  LastUsedFrame;          // +0x18
    void ListRemove();                     // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);   // 0x009FBF90
    void Touch();
};
struct CResourceList {
    long          m_pad0;                  // +0
    CResource     Head;                    // +4
    unsigned long ResourceCount;           // +0x20
    long          AllocatedMemory;         // +0x24
    long          MaximumMemory;           // +0x28
    unsigned long CurrentFrame;            // +0x2c
};
__forceinline void CResource::Touch()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}
struct CGraphicFrame { void* Data; };
struct CFileFrame { long m0; long m4; void* Frame; };   // +8 handed out
struct CTextureHolder { long m0; CResource* Texture; CResource* Get() const { return Texture; } };
struct CGraphicInfoFrame;
struct CFrameData : CResource {                          // 0x44 bytes
    CFileFrame*    FileData;               // +0x1c
    CTextureHolder TextureHolder;          // +0x20
    long           m28, m2c;
    void*          PendingLoad;            // +0x30
    long           m34;
    signed char    LoadedMipLevel;         // +0x38
    signed char    m39;
    signed char    LowestMipRequested;     // +0x3a
    signed char    m3b;
    long           BankFrame;              // +0x3c
    long           m40;
    CFrameData(CGraphicInfoFrame* owner);  // 0x009FE380
    void BeginLoading(unsigned long mipLevel, unsigned long flags);   // 0x009FDE80
};
struct CGraphicDataBank { unsigned char Data[0x194]; long CurrentFrame; };
struct CGraphicInfo { unsigned char Data[0x20]; CGraphicDataBank* Bank; };
struct CGraphicInfoFrame {
    long          m0;
    unsigned short FrameNum;               // +4
    signed char   SmallMipmapLevel;        // +6
    mutable CFrameData* Data;              // +8 CIVCountedPointer<CFrameData>
    CGraphicInfo* GraphicInfo;             // +0xc
    bool InternalGetFrameForcedLoading(unsigned long flags) const;   // 0x009FF6B0
    bool GetFrame(CGraphicFrame& frame, unsigned long mipLevel, unsigned long flags) const;
    __forceinline bool LoadFrame(CGraphicFrame& frame, unsigned long mipLevel, unsigned long flags) const
    {
        Data->BeginLoading(mipLevel, flags);
        if ((flags & 1) && InternalGetFrameForcedLoading(flags)) {
            frame.Data = Data->FileData->Frame;
            return true;
        }
        return false;
    }
};

bool CGraphicInfoFrame::GetFrame(CGraphicFrame& frame, unsigned long mipLevel, unsigned long flags) const
{
    if (Data) {
        if (Data->FileData) {
            if (!(flags & 8)) {
                long bankFrame = GraphicInfo->Bank->CurrentFrame;
                if (Data->BankFrame != bankFrame || mipLevel > (unsigned long)Data->LowestMipRequested) {
                    Data->BankFrame = bankFrame;
                    CResource* texture = Data->TextureHolder.Get();
                    if (texture)
                        texture->Touch();
                    CFrameData* data = Data;
                    if (mipLevel <= (unsigned long)data->LoadedMipLevel)
                        data->Touch();
                    if ((unsigned long)Data->LowestMipRequested > mipLevel)
                        Data->LowestMipRequested = (signed char)mipLevel;
                }
            }
            if (mipLevel < (unsigned long)Data->LoadedMipLevel) {
                Data->BeginLoading(mipLevel, flags);
                if ((flags & 0x10) && Data->PendingLoad)
                    goto load;
            }
            frame.Data = Data->FileData->Frame;
            return true;
        }
load:
        Data->BeginLoading(mipLevel, flags);
    } else {
        if (flags & 4)
            return false;
        CFrameData* data = new CFrameData(const_cast<CGraphicInfoFrame*>(this));
        if (Data) {
            if (--Data->RefCount == 0)
                Data->DeleteThis();
            Data = 0;
        }
        Data = data;
        Data->BeginLoading(mipLevel, flags);
    }
    if ((flags & 1) && InternalGetFrameForcedLoading(flags)) {
        frame.Data = Data->FileData->Frame;
        return true;
    }
    return false;
}
