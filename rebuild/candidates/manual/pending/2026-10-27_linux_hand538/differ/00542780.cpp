// NUISystem::CIconText::AddIcon(long graphic, long charIndex, EIconFlags flags) -- build an embedded icon (default 40x40, no offset); for a real graphic take its frame size from the front end's graphics bank and request the frame (loading it if needed), else a zero size; append it to Icons
// Grade: DIFFER(682v682) -- all but two instructions match: retail loads FrameWidth (+6) and FrameHeight (+8) back to back before the first fild/fstp; ours loads the height after storing the width
// Leaf: ?AddIcon@CIconText@NUISystem@@UAEXJJW4EIconFlags@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00542780, 682 bytes, `ret 0xc`. FableWin.pdb ?AddIcon@CIconText@NUISystem@@UAEXJJW4EIconFlags@2@@Z.
// Callees: NUISystem::CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall static), CGraphicInfoFrame::BeginLoading 0x009FE620,
//   CGraphicInfoFrame::CFrameData::BeginLoading 0x009FDE80 / CFrameData(const CGraphicInfoFrame*) 0x009FE380,
//   CGraphicInfoFrame::InternalGetFrameForcedLoading 0x009FF6B0, CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90,
//   CGraphicFrameData::Uninitialise 0x009FCBA0, operator new 0x00BFEA1A / delete 0x00BFE9BC,
//   vector<CIconText::CEmbeddedIcon>::push_back 0x00543C90.
// Layout: Ego_r.pdb CResource / CResourceList / CGraphicFrameData / CGraphicInfo / CGraphicInfoFrame / CFrameData;
//   retail CGraphicDataBank CurrentFrame +0x194, GraphicList +0x1e0; NUISystem::CManager PGraphicsBank at retail +0x10;
//   CIconText::Icons +0x18c.
void __cdecl operator delete(void* block);             // 0x00BFE9BC

class CResourceList;
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                       // slot 1
    mutable unsigned long IVRefCount;                  // +0x04
};
class CResource : public CIVCountedPointeeBase {       // Ego_r.pdb, 0x1C bytes
public:
    CResourceList* ResourceList;                       // +0x08
    CResource*     PrevResource;                       // +0x0c
    CResource*     NextResource;                       // +0x10
    unsigned long  ResourceSize;                       // +0x14
    unsigned long  LastUsedFrame;                      // +0x18
    __forceinline void TouchResource();
private:
    void ListRemove();                                 // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next); // 0x009FBF90
};
class CResourceList {                                  // Ego_r.pdb, 0x3C bytes
public:
    unsigned long _pad00;
    CResource     Head;                                // +0x04
    unsigned long ResourceCount;                       // +0x20
    long          AllocatedMemory;                     // +0x24
    long          MaximumMemory;                       // +0x28
    unsigned long CurrentFrame;                        // +0x2c
};
__forceinline void CResource::TouchResource()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}

class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    T* operator->() const { return Data; }
    __forceinline ~CCountedPointer()
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
class CGraphicFrameData {                             // Ego_r.pdb, 27 bytes
public:
    unsigned long   ReferenceCount;                    // +0x00
    void*           Data;                              // +0x04
    CCountedPointer<class CTexture> PTexture;                          // +0x08
    unsigned short  TLX, TLY, Width, Height, Pitch;    // +0x10
    unsigned char   Alpha : 1;                         // +0x1a
    unsigned char   BooleanAlpha : 1;
    unsigned char   UsesSurface : 1;
    unsigned char   SectionOfTPage : 1;
    unsigned char   IsInitialised : 1;
    unsigned char   ScreenGraphic : 1;
    void Uninitialise();                               // 0x009FCBA0
    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    __forceinline void Release()
    {
        if (--ReferenceCount == 0) {
            this->~CGraphicFrameData();
            operator delete(this);
        }
    }
};
class CGraphicFrame {                                 // intrusive pointer to CGraphicFrameData
public:
    CGraphicFrameData* FrameData;
    __forceinline CGraphicFrame() : FrameData(0) { }
    __forceinline ~CGraphicFrame() { if (FrameData) FrameData->Release(); }
    __forceinline CGraphicFrame& operator=(const CGraphicFrame& other)
    {
        if (FrameData)
            FrameData->Release();
        FrameData = other.FrameData;
        if (FrameData)
            ++FrameData->ReferenceCount;
        return *this;
    }
};
class CManagedTexture {                                // Ego_r.pdb, 8 bytes
public:
    unsigned long _pad00;
    CResource*    Data;                                // +0x04 CIVCountedPointer<CManagedTextureData>
    __forceinline void TouchTexture() { if (Data) Data->TouchResource(); }
};
class CGraphicInfoFrame;
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    __forceinline CIVCountedPointer& operator=(T* object)
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
        Object = object;
        return *this;
    }
    T* operator->() const { return (T*)Object; }
    bool IsNull() const { return Object == 0; }
};
class CGraphicInfo;
class CGraphicInfoFrame {                             // Ego_r.pdb, 16 bytes
public:
    class CFrameData;
    unsigned long     _pad00;
    unsigned short    FrameNum;                        // +0x04
    signed char       SmallMipmapLevel;                // +0x06
    CIVCountedPointer<CFrameData> Data;                // +0x08
    CGraphicInfo*     GraphicInfo;                     // +0x0c
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags) const;   // 0x009FE620
    __forceinline bool GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags) const;
private:
    bool InternalGetFrameForcedLoading(unsigned long flags) const;   // 0x009FF6B0
};
class CGraphicInfoFrame::CFrameData : public CResource {     // Ego_r.pdb, 0x44 bytes
public:
    CGraphicFrame   Frame;                             // +0x1c
    CManagedTexture ManagedTexture;                    // +0x20
    CManagedTexture LoadingManagedTexture;             // +0x28
    void*           FileData[2];                       // +0x30 CCountedPointer<CBankFileAsyncData>
    signed char     CurrentMipmapLevel;                // +0x38
    signed char     LoadingMipmapLevel;                // +0x39
    signed char     TouchedMipmapLevel;                // +0x3a
    unsigned long   TouchedFrame;                      // +0x3c
    const CGraphicInfoFrame* GraphicInfoFrame;         // +0x40
    CFrameData(const CGraphicInfoFrame* frame);        // 0x009FE380
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FDE80
};
class CGraphicInfo {                                  // Ego_r.pdb, 0x2c bytes
public:
    unsigned short     Width;                          // +0x00 CGraphicHeader
    unsigned short     Height;                         // +0x02
    unsigned short     Depth;                          // +0x04
    unsigned short     FrameWidth;                     // +0x06
    unsigned short     FrameHeight;                    // +0x08
    unsigned char      _pad0a[0x20 - 0x0a];
    class CGraphicDataBank* Bank;                     // +0x20
    long               Index;                          // +0x24
    CGraphicInfoFrame* Frames;                         // +0x28
    const CGraphicInfoFrame* PeekInfoFrame(unsigned long frame) const { return &Frames[frame]; }
};
template <class T> class CArray {                      // STLport vector layout (12 bytes retail)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    const T& operator[](unsigned long n) const { return _M_start[n]; }
};
// Layout: retail CGraphicDataBank has its current frame at +0x194 and GraphicList at +0x1e0 (PDB 0x254; the retail
// containers before it are smaller).
class CGraphicDataBank {
public:
    unsigned char _pad00[0x194];
    unsigned long CurrentFrame;                        // +0x194
    unsigned char _pad198[0x1e0 - 0x198];
    CArray<CGraphicInfo> GraphicList;                  // +0x1e0
    __forceinline const CGraphicInfo* PeekGraphic(unsigned long index) const { return index < GraphicList.size() ? &GraphicList[index] : 0; }
    __forceinline const CGraphicInfoFrame* PeekGraphicInfoFrame(unsigned long index, unsigned long frame) const
    {
        return PeekGraphic(index) ? PeekGraphic(index)->PeekInfoFrame(frame) : 0;
    }
};
__forceinline bool CGraphicInfoFrame::GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags) const
{
    if (!Data.IsNull()) {
        if (Data->Frame.FrameData) {
            if (!(flags & 8)) {
                if (Data->TouchedFrame != GraphicInfo->Bank->CurrentFrame || mipmapLevel <= (unsigned long)Data->TouchedMipmapLevel) {
                    Data->TouchedFrame = GraphicInfo->Bank->CurrentFrame;
                    Data->ManagedTexture.TouchTexture();
                    if (mipmapLevel <= (unsigned long)Data->CurrentMipmapLevel)
                        Data->TouchResource();
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
        const_cast<CIVCountedPointer<CFrameData>&>(Data) = new CFrameData(this);
        Data->BeginLoading(mipmapLevel, flags);
    }
    if (!(flags & 1) || !InternalGetFrameForcedLoading(flags))
        return false;
    out = Data->Frame;
    return true;
}
namespace NUISystem {
class CManager {
public:
    unsigned char _pad00[0x10];
    CCountedPointer<CGraphicDataBank> PGraphicsBank;   // +0x10 (Ego_r.pdb +0x20)
    CCountedPointer<CGraphicDataBank> GetGraphicsBank() { return PGraphicsBank; }
};
class CFrontEndManager : public CManager {
public:
    static CFrontEndManager* __fastcall GetInstance(); // 0x0041E5F2
};
}

class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& __x);                    // 0x00543C90 (vector<CIconText::CEmbeddedIcon>)
};
}

namespace NUISystem {
enum EIconFlags { };
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
    virtual void AddIcon(long graphic, long charIndex, EIconFlags flags);
    unsigned char _pad04[0x18c - 0x04];
    std::vector<CEmbeddedIcon> Icons;                  // +0x18c (retail; PDB +0x1c0)
};

void CIconText::AddIcon(long graphic, long charIndex, EIconFlags flags)
{
    CEmbeddedIcon icon(graphic, charIndex, flags);
    CCountedPointer<CGraphicDataBank> bank = CFrontEndManager::GetInstance()->GetGraphicsBank();
    if (graphic > 0) {
        const CGraphicInfoFrame* frame = bank->PeekGraphicInfoFrame(graphic, 0);
        const CGraphicInfo* info = frame->GraphicInfo;
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
