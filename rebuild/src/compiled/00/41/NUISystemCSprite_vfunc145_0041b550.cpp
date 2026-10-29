//0041b550 NUISystem::CSprite::SetGraphicIndex (PDB egor903/debug0042cd20).
// Layout: sharedCSprite donor has flattened bases; retailSizecc/d0,GraphicIndex178.
// Graphic resource model reused from matched0041ac20; includes same recovered helpers.
#include <new>
#include "engine/CSprite.h"
#include "engine/CResource.h"
#include "engine/CResourceList.h"
#include "engine/CGraphicFrame.h"
// Layout: CGraphicFrame donor is one FrameData pointer at+0 (size4).
// NUISystem method adapter preserves that exact layout and adds intrusive ownership methods.
// Layout: resource method views preserve donor CResource 0x1c and CResourceList 0x3c,
// restoring virtual methods and typed Head rather than duplicating donor vfptr storage.
namespace NUISystem {
struct CResourceList;
struct CResource {                                     // Ego_r.pdb, 0x1C bytes
    virtual void v00();
    virtual void Release();                            // slot 1: the CIVCountedPointer release on the last reference
    unsigned long  RefCount;                           // +0x04 (CIVCountedPointer count)
    CResourceList* ResourceList;                       // +0x08
    CResource*     PrevResource;                       // +0x0c
    CResource*     NextResource;                       // +0x10
    unsigned long  ResourceSize;                       // +0x14
    unsigned long  LastUsedFrame;                      // +0x18
    void ListRemove();                                 // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next); // 0x009FBF90
    __forceinline void Touch();
};
struct CResourceList {                                 // Ego_r.pdb, 0x3C bytes
    unsigned long _pad00;
    CResource     Head;                                // +0x04
    unsigned long ResourceCount;                       // +0x20
    long          AllocatedMemory;                     // +0x24
    long          MaximumMemory;                       // +0x28
    unsigned long CurrentFrame;                        // +0x2c
};
__forceinline void CResource::Touch()
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
struct CGraphicFrameData {                             // Ego_r.pdb, 27 bytes
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
    __forceinline void DecRef()
    {
        if (--ReferenceCount == 0) {
            this->~CGraphicFrameData();
            operator delete(this);
        }
    }
};
struct CGraphicFrame {                                 // intrusive pointer to CGraphicFrameData
    CGraphicFrameData* FrameData;
    CGraphicFrame() : FrameData(0) { }
    __forceinline ~CGraphicFrame()
    {
        if (FrameData)
            FrameData->DecRef();
    }
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
struct CManagedTexture {                               // Ego_r.pdb, 8 bytes
    unsigned long _pad00;
    CResource*    Data;                                // +0x04 CIVCountedPointer<CManagedTextureData>
    __forceinline void Touch() { if (Data) Data->Touch(); }
};
struct CGraphicInfoFrame;
struct CFrameData : CResource {                        // Ego_r.pdb, 0x44 bytes
    CGraphicFrame   Frame;                             // +0x1c
    CManagedTexture ManagedTexture;                    // +0x20
    CManagedTexture LoadingManagedTexture;             // +0x28
    void*           FileData[2];                       // +0x30 CCountedPointer<CBankFileAsyncData>
    signed char     CurrentMipmapLevel;                // +0x38
    signed char     LoadingMipmapLevel;                // +0x39
    signed char     TouchedMipmapLevel;                // +0x3a
    unsigned long   TouchedFrame;                      // +0x3c
    CGraphicInfoFrame* GraphicInfoFrame;               // +0x40
    CFrameData(CGraphicInfoFrame* frame);              // 0x009FE380
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FDE80
    __forceinline void DecRef() { if (--RefCount == 0) Release(); }
};
struct CGraphicDataBank;
struct CGraphicInfo {
    unsigned char      _pad00[0x20];
    CGraphicDataBank*  Bank;                           // +0x20
    long               Index;                          // +0x24
    CGraphicInfoFrame* Frames;                         // +0x28
};
struct CFrameDataPointer {                             // CIVCountedPointer<CFrameData>
    CFrameData* P;
    __forceinline CFrameDataPointer& operator=(CFrameData* p)
    {
        if (P) {
            P->DecRef();
            P = 0;
        }
        P = p;
        return *this;
    }
    CFrameData* operator->() const { return P; }
    operator bool() const { return P != 0; }
};
struct CGraphicInfoFrame {                             // Ego_r.pdb, 16 bytes
    unsigned long     _pad00;
    unsigned short    FrameNum;                        // +0x04
    signed char       SmallMipmapLevel;                // +0x06
    CFrameDataPointer Data;                            // +0x08
    CGraphicInfo*     GraphicInfo;                     // +0x0c
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FE620
    bool InternalGetFrameForcedLoading(unsigned long flags);   // 0x009FF6B0
    __forceinline bool GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags);
};
struct CGraphicInfoArray {
    CGraphicInfo* _M_start; CGraphicInfo* _M_finish; CGraphicInfo* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CGraphicInfo& operator[](unsigned int n) { return *(_M_start + n); }
};
struct CGraphicDataBank {
    virtual void v00();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual unsigned long GetGraphicFrameWidth(unsigned long index, unsigned long frame) const;    // slot 21 (+0x54)
    virtual unsigned long GetGraphicFrameHeight(unsigned long index, unsigned long frame) const;   // slot 22 (+0x58)
    unsigned char _pad004[0x194 - 0x04];
    unsigned long CurrentFrame;                        // +0x194
    unsigned char _pad198[0x1e0 - 0x198];
    CGraphicInfoArray GraphicList;                     // +0x1e0 (CArray<CGraphicInfo>: STLport vector)
    __forceinline CGraphicInfo* GetGraphicInfo(unsigned long index) { return index < GraphicList.size() ? &GraphicList[index] : 0; }
    __forceinline CGraphicInfoFrame* GetGraphicFrames(unsigned long index) { return GetGraphicInfo(index) ? GetGraphicInfo(index)->Frames : 0; }
};
__forceinline bool CGraphicInfoFrame::GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags)
{
    if (Data) {
        if (Data->Frame.FrameData) {
            if (!(flags & 8)) {
                if (Data->TouchedFrame != GraphicInfo->Bank->CurrentFrame || mipmapLevel <= (unsigned long)Data->TouchedMipmapLevel) {
                    Data->TouchedFrame = GraphicInfo->Bank->CurrentFrame;
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

struct CSpriteGraphicsInfo {
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CSpriteGraphicsPointer {
    CGraphicDataBank* Data; CSpriteGraphicsInfo* Info;
    CSpriteGraphicsPointer(const CSpriteGraphicsPointer& p) : Data(p.Data), Info(p.Info) { if (Info) ++Info->RefCount; }
    ~CSpriteGraphicsPointer() { if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); ::operator delete(Info); } }
    CGraphicDataBank* operator->() const { return Data; }
};
class CManager { public:
    unsigned char Base[0x10];
    CSpriteGraphicsPointer PGraphicsBank;
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
    CSpriteGraphicsPointer GetGraphicsBank() const { return PGraphicsBank; }
};

class CSprite { public:unsigned char ToSize[0xcc];float SizeX,SizeY;unsigned char ToGraphic[0x178-0xd4];unsigned long GraphicIndex;void SetGraphicIndex(unsigned long);};
void CSprite::SetGraphicIndex(unsigned long index){GraphicIndex=index;if(GraphicIndex>0){SizeX=(float)CManager::GetInstance()->GetGraphicsBank()->GetGraphicFrameWidth(GraphicIndex,0);SizeY=(float)CManager::GetInstance()->GetGraphicsBank()->GetGraphicFrameHeight(GraphicIndex,0);CGraphicInfoFrame* frames=CManager::GetInstance()->GetGraphicsBank()->GetGraphicFrames(GraphicIndex);frames->BeginLoading(0,0x10);CGraphicFrame frame;frames->GetFrame(frame,0,1);}}
}