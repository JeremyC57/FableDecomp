// CGraphicInfoFrame::GetVolumeTexture(CVolumeTexture** out, unsigned long mipLevel) -- make sure the frame is loaded (start loading at that mip level, finish any pending read and upload it); when it has volume data, refresh the resources' LRU positions once per frame-stamp (unless this mip level is already resident) and return the volume texture through `out`
// Leaf: GetVolumeTexture
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FF710, length 330 bytes, `ret 8`.
// Callees: CGraphicInfoFrame::BeginLoading(unsigned long, unsigned long) @ 0x009FE620, CBankFileAsyncData::IsFinished
//   @ 0x009D54E0 / WaitUntilFinished @ 0x009D5B60, CFrameData's load completion @ 0x009FC440 and UploadFrame(bool)
//   @ 0x009FF220 (as the landed InternalGetFrameForcedLoading 0x009FF6B0), CResource::ListRemove @ 0x009FBFB0 /
//   ListInsert(CResource*, CResource*) @ 0x009FBF90, operator delete @ 0x00BFE9BC (the texture's count block).
// Layout: Ego_r.pdb CGraphicInfoFrame (Data +0x08, GraphicInfo +0x0C); CFrameData as used here: CResource base (owner list
//   +0x08, list stamp +0x18), volume holder +0x20 (resource pointer +0x24), AsyncRead +0x30, resident mip limits +0x38
//   / +0x3A (signed char), last frame stamp +0x3C; the volume resource holds a counted texture pointer at +0x28/+0x2C; the
//   resource list's head +0x04, tail +0x10 and stamp +0x2C; the graphic info's frame stamp at (+0x20)->+0x194.
// Shape: the LRU refresh is a `__forceinline` CResource member applied to the volume then to the frame data; the texture
//   pointer is read through a full-expression CTexturePointer copy (+1 then -1 on its count, before the null test); the
//   success path is nested so the callee-saved pushes stay at entry and both failures share one `xor al,al` exit.
class CBankFileAsyncData { public:
public:
    bool IsFinished() const;   // 0x009D54E0
    bool WaitUntilFinished();   // 0x009D5B60
};
class CResource;
class CResourceList { public:
    unsigned char _pad00[4];
    CResource*    Tail;                            // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CResource*    Head;                            // +0x10
    unsigned char _pad14[0x2c - 0x14];
    unsigned long Stamp;                           // +0x2c
};
class CResource { public:
    unsigned char  _pad00[8];
    CResourceList* List;                           // +0x08
    unsigned char  _pad0C[0x18 - 0x0c];
    unsigned long  Stamp;                          // +0x18
    void ListRemove();                             // 0x009FBFB0
    void ListInsert(CResource* before, CResource* after);   // 0x009FBF90
    __forceinline void Touch()
    {
        if (List && Stamp != List->Stamp) {
            ListRemove();
            ListInsert(List->Head, (CResource*)&List->Tail);
            Stamp = List->Stamp;
        }
    }
};
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
class CVolumeTexture;
struct CTexturePointer {
    CVolumeTexture* m_pObject;
    SCountBlock*    m_pCount;
    CTexturePointer(const CTexturePointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CTexturePointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
};
struct CVolumeResource : CResource {
    unsigned char   _pad1C[0x28 - 0x1c];
    CTexturePointer Texture;                       // +0x28
};
struct CVolumeHolder {
    void*            _word00;                      // +0x00
    CVolumeResource* Volume;                       // +0x04
};
struct CFrameData : CResource {
    unsigned char       _pad1C[0x20 - 0x1c];
    CVolumeHolder       VolumeHolder;              // +0x20
    unsigned char       _pad28[0x30 - 0x28];
    CBankFileAsyncData* AsyncRead;                 // +0x30
    unsigned char       _pad34[0x38 - 0x34];
    signed char         SmallestResidentMip;       // +0x38
    unsigned char       _pad39;
    signed char         LowestRequestedMip;        // +0x3a
    unsigned char       _pad3B;
    unsigned long       LastFrameStamp;            // +0x3c
    void CompleteLoad();                           // 0x009FC440
    bool UploadFrame(bool force);                  // 0x009FF220
};
struct CFrameCounter {
    unsigned char _pad[0x194];
    unsigned long FrameStamp;                      // +0x194
};
class CGraphicInfo { public:
    unsigned char  _pad[0x20];
    CFrameCounter* Counter;                        // +0x20
};
class CGraphicInfoFrame {
public:
    void*          _word00;                        // +0x00
    unsigned short FrameNum;                       // +0x04
    signed char    SmallMipmapLevel;               // +0x06
    CFrameData*    Data;                           // +0x08
    CGraphicInfo*  GraphicInfo;                    // +0x0c
    void BeginLoading(unsigned long mipLevel, unsigned long flags);   // 0x009FE620
    bool GetVolumeTexture(CVolumeTexture** out, unsigned long mipLevel);
};

bool CGraphicInfoFrame::GetVolumeTexture(CVolumeTexture** out, unsigned long mipLevel)
{
    BeginLoading(mipLevel, 0);
    if (Data && Data->AsyncRead) {
        if (!Data->AsyncRead->IsFinished())
            Data->AsyncRead->WaitUntilFinished();
        Data->CompleteLoad();
        Data->UploadFrame(true);
    }
    if (Data && Data->VolumeHolder.Volume) {
        unsigned long stamp = GraphicInfo->Counter->FrameStamp;
        if (Data->LastFrameStamp != stamp || mipLevel <= (unsigned long)Data->LowestRequestedMip) {
            Data->LastFrameStamp = stamp;
            CVolumeHolder& holder = Data->VolumeHolder;
            if (holder.Volume)
                holder.Volume->Touch();
            if (mipLevel <= (unsigned long)Data->SmallestResidentMip)
                Data->Touch();
            if ((unsigned long)Data->LowestRequestedMip > mipLevel)
                Data->LowestRequestedMip = (signed char)mipLevel;
        }
        CVolumeTexture* result = CTexturePointer(Data->VolumeHolder.Volume->Texture).m_pObject;
        if (result) {
            *out = result;
            return true;
        }
    }
    return false;
}