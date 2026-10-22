// Grade: DIFFER(682v682) -- NUISystem::CIconText::AddIcon; all but two instructions match: retail loads FrameWidth (+6) into edx and FrameHeight (+8) into eax back to back before storing the width to the dead graphic-arg slot for fild; ours loads the height later (casts) or first (named locals, SetSize helper, C2DCoordI)
// Leaf: AddIcon
void __cdecl operator delete(void* block);             // 0x00BFE9BC

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
    __forceinline CGraphicFrame() : FrameData(0) { }
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
struct CGraphicInfoFrame;
struct CGraphicInfo {                                  // Ego_r.pdb, 0x2c bytes
    unsigned short     Width;                          // +0x00 CGraphicHeader
    unsigned short     Height;                         // +0x02
    unsigned short     Depth;                          // +0x04
    unsigned short     FrameWidth;                     // +0x06
    unsigned short     FrameHeight;                    // +0x08
    unsigned char      _pad0a[0x20 - 0x0a];
    struct CGraphicDataBank* Bank;                     // +0x20
    long               Index;                          // +0x24
    CGraphicInfoFrame* Frames;                         // +0x28
};
struct CGraphicInfoArray {                             // CArray<CGraphicInfo> (12 bytes retail)
    CGraphicInfo* _M_start;
    CGraphicInfo* _M_finish;
    CGraphicInfo* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
// Layout: retail CGraphicDataBank has its current frame at +0x194 and GraphicList at +0x1e0 (PDB 0x254; the retail
// containers before it are smaller).
struct CGraphicDataBank {
    unsigned char _pad00[0x194];
    unsigned long CurrentFrame;                        // +0x194
    unsigned char _pad198[0x1e0 - 0x198];
    CGraphicInfoArray GraphicList;                     // +0x1e0
    __forceinline CGraphicInfo* GetGraphicInfo(long index) { return (unsigned long)index < GraphicList.size() ? &GraphicList._M_start[index] : 0; }
    __forceinline CGraphicInfoFrame* GetGraphicInfoFrame(long index) { return GetGraphicInfo(index) ? GetGraphicInfo(index)->Frames : 0; }
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
    bool InternalGetFrameForcedLoading(unsigned long flags);   // 0x009FF6B0
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FE620
    __forceinline bool GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags);
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
struct CCPInfo;
struct CGraphicDataBankPointer {                       // CCountedPointer<CGraphicDataBank>
    CGraphicDataBank* Data;
    CCPInfo* Info;
    __forceinline CGraphicDataBankPointer(const CGraphicDataBankPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    __forceinline ~CGraphicDataBankPointer()
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
    CGraphicDataBank* operator->() const { return Data; }
};
class CFrontEndManager {
public:
    unsigned char _pad00[0x10];
    CGraphicDataBankPointer GraphicBank;               // +0x10 (descriptive)
    static CFrontEndManager* __fastcall GetInstance(); // 0x0041E5F2
};

struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
enum EIconFlags { ICON_FLAGS_NONE = 0 };

namespace NUISystem {
class CIconText {
public:
    struct CEmbeddedIcon {                             // Ego_r.pdb, 28 bytes
        long       Graphic;                            // +0x00
        long       CharIndex;                          // +0x04
        EIconFlags Flags;                              // +0x08
        C2DVector  Size;                               // +0x0c
        C2DVector  Offset;                             // +0x14
        CEmbeddedIcon(long graphic, long charIndex, EIconFlags flags) : Graphic(graphic), CharIndex(charIndex), Flags(flags)
        {
            Offset = C2DVector(0.0f, 0.0f);
            Size = C2DVector(40.0f, 40.0f);
        }
    };
    struct CIconArray {                                // vector<CEmbeddedIcon> (12 bytes retail)
        CEmbeddedIcon* _M_start;
        CEmbeddedIcon* _M_finish;
        CEmbeddedIcon* _M_end_of_storage;
        void push_back(const CEmbeddedIcon& icon);     // 0x00543C90
    };
    unsigned char _pad00[0x18c];
    CIconArray Icons;                                  // +0x18c (retail; PDB +0x1c0)
    void AddIcon(long graphic, long charIndex, EIconFlags flags);
};

void CIconText::AddIcon(long graphic, long charIndex, EIconFlags flags)
{
    CEmbeddedIcon icon(graphic, charIndex, flags);
    CGraphicDataBankPointer bank = CFrontEndManager::GetInstance()->GraphicBank;
    if (graphic > 0) {
        CGraphicInfoFrame* frame = bank->GetGraphicInfoFrame(graphic);
        CGraphicInfo* info = frame->GraphicInfo;
        icon.Size.X = (float)info->FrameWidth;
        icon.Size.Y = (float)info->FrameHeight;
        frame->BeginLoading(0, 0x10);
        CGraphicFrame graphicFrame;
        frame->GetFrame(graphicFrame, 0, 0x11);
    } else {
        icon.Size.X = 0.0f;
        icon.Size.Y = 0.0f;
    }
    Icons.push_back(icon);
}
}
