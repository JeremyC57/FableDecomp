// Grade: DIFFER(965v971) -- register allocation: retail holds info in ebx then overwrites it with the cache (info re-read from its argument slot on the cache path), source frame in ebp; ours keeps info in ebp and spills the cache into the info argument slot; the completed-bank GetFrame and the mip-width loop then schedule differently
// CEngineSurfaceCompositionManager::GetComposedSurface(CGraphicFrame& frame, CCompositeSurfaceInfo& info, unsigned long level, bool wait) -- hand out the composed surface for a morph: from the completed bank when the morph has a baked entry (look it up once, adjust the mip level to the base texture's width, touch/stream the bank frame and return its graphic frame once resident), else from the surface cache when its cached morph is nearly identical and resident at this level
// Leaf: GetComposedSurface
// Layout: local model; retail offsets: CompletedBankFile +0x398 (CBankFile Size +4, GraphicList records +0x1e0 -> +0x578), CurrentFrame +0x9d0,
//         ReuseTolerance +0x10c0; CCompositeSurfaceCache (a CResource: List +8, Stamp +0x18) CurrentMorph +0x20, CachedMorph +0x34,
//         MipmapLevel +0x48, GraphicFrame +0x4c, BankFileIndex +0x58, LastUsedFrame +0x5c; CCompositeSurfaceInfo Cache +4.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
void* operator new(unsigned int size);             // 0x00BFEA1A
extern "C" double __fastcall GFGetTime();          // 0x009E1BC0

struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void* object);
    void* m_pObject;
};
struct CTexture;
struct CCountedPointer_CTexture {
    CTexture*    m_pObject;
    SCountBlock* m_pCount;
    __forceinline ~CCountedPointer_CTexture()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct CGraphicFrameData {
    unsigned long            ReferenceCount;       // +0x00
    void*                    Data;                 // +0x04
    CCountedPointer_CTexture PTexture;             // +0x08
    unsigned char            _pad10[0xa];
    unsigned char            m_flags;              // +0x1a
    void Uninitialise();                           // 0x009FCBA0
    __forceinline ~CGraphicFrameData()
    {
        if (m_flags & 0x10)
            Uninitialise();
    }
};
struct CGraphicFrame {
    CGraphicFrameData* FrameData;
    __forceinline CGraphicFrame& operator=(const CGraphicFrame& other)
    {
        CGraphicFrameData* data = FrameData;
        if (data && --data->ReferenceCount == 0) {
            data->~CGraphicFrameData();
            operator delete(data);
        }
        FrameData = other.FrameData;
        if (FrameData)
            ++FrameData->ReferenceCount;
        return *this;
    }
};

struct CResource;
struct CResourceList {
    unsigned char _pad00[4];
    CResource*    Tail;                            // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CResource*    Head;                            // +0x10
    unsigned char _pad14[0x2c - 0x14];
    unsigned long Stamp;                           // +0x2c
};
struct CResource {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+4)
    long           m_references;                   // +0x04
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
struct CVolumeHolder {
    void*      _word00;                            // +0x00
    CResource* Volume;                             // +0x04
};
struct CGraphicInfoFrame;
struct CFrameData : CResource {
    CGraphicFrame Frame;                           // +0x1c
    CVolumeHolder VolumeHolder;                    // +0x20
    unsigned char _pad28[0x38 - 0x28];
    signed char   SmallestResidentMip;             // +0x38
    unsigned char _pad39;
    signed char   LowestRequestedMip;              // +0x3a
    unsigned char _pad3B;
    unsigned long LastFrameStamp;                  // +0x3c
    unsigned char _pad40[4];
    CFrameData(CGraphicInfoFrame* frame);          // 0x009FE380
    void BeginLoading(unsigned long mip, unsigned long flags);   // 0x009FDE80
};
struct CFrameCounter {
    unsigned char _pad[0x194];
    unsigned long FrameStamp;                      // +0x194
};
struct CGraphicInfo {                              // Header (CGraphicHeader) first
    unsigned short Width;                          // +0x00
    unsigned short Height;                         // +0x02
    unsigned short Depth;                          // +0x04
    unsigned short FrameWidth;                     // +0x06
    unsigned char  _pad08[0x20 - 0x08];
    CFrameCounter* Counter;                        // +0x20 (Bank)
};
template <class T> struct CIVCountedPointer {
    T* Data;
    void Reset()
    {
        T* p = Data;
        if (p) {
            if (--p->m_references == 0)
                p->Destroy();
            Data = 0;
        }
    }
    CIVCountedPointer& operator=(T* p) { Reset(); Data = p; return *this; }
    T* operator->() const { return Data; }
};
struct CGraphicInfoFrame {
    void*                         _word00;
    unsigned short                FrameNum;
    signed char                   SmallMipmapLevel;
    CIVCountedPointer<CFrameData> Data;            // +0x08
    CGraphicInfo*                 GraphicInfo;     // +0x0c
};
struct SBankGraphic {
    unsigned char      _pad[0x28];
    CGraphicInfoFrame* m_pFrame;                   // +0x28
};
template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
    unsigned long size() { return end() - begin(); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CGraphicDataBank {
    void*                       __vftable;
    unsigned long               Size;              // +0x04
    unsigned char               _pad[0x1e0 - 8];
    CRetailVector<SBankGraphic> GraphicList;       // +0x1e0
    unsigned char               _pad1ec[0x30c - 0x1ec];
    __forceinline SBankGraphic* Get(unsigned long index) { return index < GraphicList.size() ? &GraphicList[index] : 0; }
    __forceinline CGraphicInfoFrame* GetFrame(unsigned long index) { return Get(index) ? Get(index)->m_pFrame : 0; }
};

struct CEngineTextureMorph {
    long          OriginalTextureIndex;            // +0x00
    unsigned char _rest[0x10];
};
struct CCompositeSurfaceCache : CResource {
    void*               Owner;                     // +0x1c
    CEngineTextureMorph CurrentMorph;              // +0x20
    CEngineTextureMorph CachedMorph;               // +0x34
    unsigned long       MipmapLevel;               // +0x48
    CGraphicFrame       GraphicFrame;              // +0x4c
    unsigned char       ManagedTexture[8];         // +0x50
    unsigned long       BankFileIndex;             // +0x58
    unsigned long       LastUsedFrame;             // +0x5c
};
struct CCompositeSurfaceInfo {
    void*                   _word00;
    CCompositeSurfaceCache* Cache;                 // +0x04 (CCountedPointer data)
    void*                   CacheInfo;             // +0x08
};

struct CEngineSurfaceCompositionManager {
    unsigned char    _pad[0x398];
    CGraphicDataBank CompletedBankFile;            // +0x398
    unsigned char    _pad6a4[0x9d0 - 0x6a4];
    unsigned long    CurrentFrame;                 // +0x9d0
    unsigned char    _pad9d4[0x10c0 - 0x9d4];
    float            ReuseTolerance;               // +0x10c0
    CGraphicInfoFrame* GetGraphicInfo(unsigned long handle);   // 0x00B2B790
    void GetDimensionsAndAdjustMipmapLevel(long* dimensions, CGraphicInfoFrame* frame, unsigned long* level);   // 0x00B2AEF0
    unsigned long FindIndexInCompletedBank(const CEngineTextureMorph* morph);   // 0x00B2AE60
    bool IsNearlyIdentical(const CEngineTextureMorph& a, const CEngineTextureMorph& b, float tolerance) const;   // 0x00B2B050
    bool GetComposedSurface(CGraphicFrame& frame, CCompositeSurfaceInfo& info, unsigned long level, bool wait);
};

bool CEngineSurfaceCompositionManager::GetComposedSurface(CGraphicFrame& frame, CCompositeSurfaceInfo& info, unsigned long level, bool wait)
{
    const CEngineTextureMorph& morph = info.Cache->CurrentMorph;
    CGraphicInfoFrame* source = GetGraphicInfo(morph.OriginalTextureIndex);
    GFGetTime();
    long dimensions[2];
    GetDimensionsAndAdjustMipmapLevel(dimensions, source, &level);
    CCompositeSurfaceCache* cache = info.Cache;
    if (!cache)
        return false;
    if (!cache->BankFileIndex && CompletedBankFile.Size)
        cache->BankFileIndex = FindIndexInCompletedBank(&morph);
    unsigned long bankIndex = cache->BankFileIndex;
    if (bankIndex) {
        CGraphicInfoFrame* completed = CompletedBankFile.GetFrame(bankIndex);
        unsigned long width = completed->GraphicInfo->FrameWidth;
        if (level > 0) {
            unsigned long target = source->GraphicInfo->FrameWidth;
            do {
                if (width >= target)
                    break;
                width *= 2;
            } while (--level);
        }
        cache->LastUsedFrame = CurrentFrame;
        if (completed->Data.Data) {
            if (completed->Data->Frame.FrameData) {
                unsigned long stamp = completed->GraphicInfo->Counter->FrameStamp;
                if (completed->Data->LastFrameStamp != stamp || level <= (unsigned long)completed->Data->LowestRequestedMip) {
                    completed->Data->LastFrameStamp = stamp;
                    CVolumeHolder& holder = completed->Data->VolumeHolder;
                    if (holder.Volume)
                        holder.Volume->Touch();
                    if (level <= (unsigned long)completed->Data->SmallestResidentMip)
                        completed->Data->Touch();
                    if ((unsigned long)completed->Data->LowestRequestedMip > level)
                        completed->Data->LowestRequestedMip = (signed char)level;
                }
                if (level < (unsigned long)completed->Data->SmallestResidentMip) {
                    completed->Data->BeginLoading(level, 0);
                    frame = completed->Data->Frame;
                    return true;
                }
                frame = completed->Data->Frame;
                return true;
            }
            completed->Data->BeginLoading(level, 0);
            return false;
        }
        completed->Data = new CFrameData(completed);
        completed->Data->BeginLoading(level, 0);
        return false;
    }
    if (!cache->GraphicFrame.FrameData)
        return false;
    if (!IsNearlyIdentical(morph, info.Cache->CachedMorph, ReuseTolerance))
        return false;
    if (level <= info.Cache->MipmapLevel)
        info.Cache->Touch();
    if (info.Cache->MipmapLevel > level)
        return false;
    frame = info.Cache->GraphicFrame;
    return true;
}
