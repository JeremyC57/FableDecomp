#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
typedef long long __int64;
#endif
// behaviour test: NUISystem::CSprite::GetDataFromDef: def fields copied (angle, truncated width/height, 2D flag), first graphic index read through a second counted def, frame size from the bank, frame streaming started (BeginLoading 0/0x10) and the frame fetched at mip 0 with forced loading (new frame data path, cached frame path with touch), no-graphic path, every count restored


#include <stddef.h>
#include <new>
static int g_listRemove, g_listInsert, g_release, g_uninit, g_fdBegin, g_gifBegin, g_forced, g_destroy, g_ctor;
static unsigned long g_beginMip, g_beginFlags, g_forcedFlags;
static bool g_forcedResult;

class CResourceList;
class CResource { public:                                     // Ego_r.pdb, 0x1C bytes
    virtual void v00() {}
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
class CResourceList { public:                                 // Ego_r.pdb, 0x3C bytes
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
    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    __forceinline void DecRef()
    {
        if (--ReferenceCount == 0) {
            this->~CGraphicFrameData();
            operator delete(this);
        }
    }
};
class CGraphicFrame { public:                                 // intrusive pointer to CGraphicFrameData
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
class CManagedTexture { public:                               // Ego_r.pdb, 8 bytes
    unsigned long _pad00;
    CResource*    Data;                                // +0x04 CIVCountedPointer<CManagedTextureData>
    __forceinline void Touch() { if (Data) Data->Touch(); }
};
class CGraphicInfoFrame;
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
    CFrameData(CGraphicInfoFrame* frame)
    {
        ++g_ctor;
        RefCount = 1; ResourceList = 0; PrevResource = 0; NextResource = 0; ResourceSize = 0; LastUsedFrame = 0;
        ManagedTexture.Data = 0; LoadingManagedTexture.Data = 0; FileData[0] = 0; FileData[1] = 0;
        CurrentMipmapLevel = 3; LoadingMipmapLevel = 3; TouchedMipmapLevel = 3; TouchedFrame = 0;
        GraphicInfoFrame = frame;
    }
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FDE80
    __forceinline void DecRef() { if (--RefCount == 0) Release(); }
};
class CGraphicDataBank;
class CGraphicInfo { public:
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
class CGraphicInfoFrame { public:                             // Ego_r.pdb, 16 bytes
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
class CGraphicDataBank { public:
    virtual void v00() {}
    virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {}
    virtual void v17() {} virtual void v18() {} virtual void v19() {} virtual void v20() {}
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
struct CFrontEndManagerGraphicsView {
    unsigned char Base[0x10];
    CSpriteGraphicsPointer PGraphicsBank;
    static CFrontEndManagerGraphicsView* __fastcall GetInstance();   // 0x0041E5F2
    CSpriteGraphicsPointer GetGraphicsBank() const { return PGraphicsBank; }
};

namespace NUISystem {
struct CSpriteGraphicDef {                             // 124 bytes (descriptive)
    unsigned char _pad00[0x3c];
    unsigned long GraphicIndex;                        // +0x3c
    unsigned char _pad40[0x7c - 0x40];
};
struct CDefVirtual {
    virtual void v00() {}                                // slot 0 (the deleting dtor)
    virtual void Destroy();                            // slot 1 (+4)
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CSpriteDef : CDefVirtual, CRefCounted {
    unsigned char      _def008[0x40 - 0x08];
    CSpriteGraphicDef* GraphicsStart;                  // +0x40 (STLport vector)
    CSpriteGraphicDef* GraphicsFinish;                 // +0x44
    CSpriteGraphicDef* GraphicsEnd;                    // +0x48
    unsigned char      _def04c[0x58 - 0x4c];
    float              HeightValue;                    // +0x58
    float              WidthValue;                     // +0x5c
    unsigned char      _def060[0xb8 - 0x60];
    float              Angle;                          // +0xb8
    unsigned char      _def0bc[0x224 - 0xbc];
    long               Sprite2DFlag;                   // +0x224
    long GraphicCount() const { return GraphicsFinish - GraphicsStart; }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CSpriteDef* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(const CDefPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CSpriteDef* operator->() const { return m_p; }
};
class CComponent { public:
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {} virtual void v007() {}
    virtual void v008() {} virtual void v009() {} virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {}
    virtual void v016() {} virtual void v017() {} virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {}
    virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {} virtual void v030() {} virtual void v031() {}
    virtual void v032() {} virtual void v033() {} virtual void v034() {} virtual void v035() {} virtual void v036() {} virtual void v037() {} virtual void v038() {} virtual void v039() {}
    virtual void v040() {} virtual void v041() {} virtual void v042() {} virtual void v043() {} virtual void v044() {} virtual void v045() {} virtual void v046() {} virtual void v047() {}
    virtual void v048() {} virtual void v049() {} virtual void v050() {} virtual void v051() {} virtual void v052() {} virtual void v053() {} virtual void v054() {} virtual void v055() {}
    virtual void v056() {} virtual void v057() {} virtual void v058() {} virtual void v059() {} virtual void v060() {} virtual void v061() {} virtual void v062() {} virtual void v063() {}
    virtual void v064() {} virtual void v065() {} virtual void v066() {} virtual void v067() {} virtual void v068() {} virtual void v069() {} virtual void v070() {} virtual void v071() {}
    virtual void v072() {} virtual void v073() {} virtual void v074() {} virtual void v075() {} virtual void v076() {} virtual void v077() {} virtual void v078() {} virtual void v079() {}
    virtual void v080() {} virtual void v081() {} virtual void v082() {} virtual void v083() {} virtual void v084() {} virtual void v085() {} virtual void v086() {} virtual void v087() {}
    virtual void v088() {} virtual void v089() {} virtual void v090() {} virtual void v091() {} virtual void v092() {} virtual void v093() {} virtual void v094() {} virtual void v095() {}
    virtual void v096() {} virtual void v097() {} virtual void v098() {} virtual void v099() {} virtual void v100() {} virtual void v101() {} virtual void v102() {} virtual void v103() {}
    virtual void v104() {} virtual void v105() {} virtual void v106() {} virtual void v107() {}
    virtual CDefPointer GetDef();                       // slot 108 (+0x1B0)
    unsigned char _pad004[0xcc - 0x04];
    float SizeX, SizeY;                                 // +0xcc
};
class CSprite : public CComponent { public:
    unsigned char _pad0d4[0x164 - 0xd4];
    float         Angle;                                // +0x164
    unsigned long Width;                                // +0x168
    unsigned long Height;                               // +0x16c
    unsigned char NoRenderNextFrame;                    // +0x170
    long          Sprite2DFlag;                         // +0x174
    unsigned long GraphicIndex;                         // +0x178
    unsigned long GetFirstGraphicIndex() { return GetDef()->GraphicsStart[0].GraphicIndex; }
    void GetDataFromDef();
};

void CSprite::GetDataFromDef()
{
    CDefPointer def = GetDef();
    Angle = def->Angle;
    Width = (unsigned long)def->WidthValue;
    Height = (unsigned long)def->HeightValue;
    Sprite2DFlag = def->Sprite2DFlag;
    if (def->GraphicCount())
        GraphicIndex = GetFirstGraphicIndex();
    else
        GraphicIndex = 0;
    if (GraphicIndex > 0) {
        SizeX = (float)CFrontEndManagerGraphicsView::GetInstance()->GetGraphicsBank()->GetGraphicFrameWidth(GraphicIndex, 0);
        SizeY = (float)CFrontEndManagerGraphicsView::GetInstance()->GetGraphicsBank()->GetGraphicFrameHeight(GraphicIndex, 0);
        CGraphicInfoFrame* frames = CFrontEndManagerGraphicsView::GetInstance()->GetGraphicsBank()->GetGraphicFrames(GraphicIndex);
        frames->BeginLoading(0, 0x10);
        CGraphicFrame frame;
        frames->GetFrame(frame, 0, 1);
    }
}
}

void CResource::Release() { ++g_release; }
void CResource::ListRemove() { ++g_listRemove; }
void CResource::ListInsert(CResource*, CResource*) { ++g_listInsert; }
void CGraphicFrameData::Uninitialise() { ++g_uninit; }
void CFrameData::BeginLoading(unsigned long, unsigned long) { ++g_fdBegin; }
void CGraphicInfoFrame::BeginLoading(unsigned long m, unsigned long f) { ++g_gifBegin; g_beginMip = m; g_beginFlags = f; }
bool CGraphicInfoFrame::InternalGetFrameForcedLoading(unsigned long f) { ++g_forced; g_forcedFlags = f; return g_forcedResult; }
unsigned long CGraphicDataBank::GetGraphicFrameWidth(unsigned long i, unsigned long f) const { return 10 * i + 3 + f; }
unsigned long CGraphicDataBank::GetGraphicFrameHeight(unsigned long i, unsigned long f) const { return 20 * i + 5 + f; }
static void* g_viewStore[16];
CFrontEndManagerGraphicsView* __fastcall CFrontEndManagerGraphicsView::GetInstance() { return (CFrontEndManagerGraphicsView*)g_viewStore; }
static NUISystem::CSpriteDef* g_def;
void NUISystem::CDefVirtual::Destroy() { ++g_destroy; }
NUISystem::CDefPointer NUISystem::CComponent::GetDef() { CDefPointer p; p.m_p = g_def; if (g_def) ++g_def->m_count; return p; }

static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static void reset() { g_listRemove = g_listInsert = g_release = g_uninit = g_fdBegin = g_gifBegin = g_forced = g_destroy = g_ctor = 0; g_beginMip = g_beginFlags = g_forcedFlags = 99; }

int main()
{
    static CGraphicDataBank bank;
    static CGraphicInfo infos[3];
    static CGraphicInfoFrame gif;
    memset(infos, 0, sizeof(infos));
    memset(&gif, 0, sizeof(gif));
    bank.CurrentFrame = 77;
    bank.GraphicList._M_start = infos; bank.GraphicList._M_finish = infos + 3; bank.GraphicList._M_end_of_storage = infos + 3;
    infos[2].Bank = &bank; infos[2].Index = 2; infos[2].Frames = &gif;
    gif.GraphicInfo = &infos[2];
    static CSpriteGraphicsInfo info;
    info.RefCount = 1; info.DeleteFunc = 0; info.Data = 0;
    CFrontEndManagerGraphicsView* view = CFrontEndManagerGraphicsView::GetInstance();
    view->PGraphicsBank.Data = &bank; view->PGraphicsBank.Info = &info;

    static NUISystem::CSpriteGraphicDef gdefs[2];
    memset(gdefs, 0, sizeof(gdefs));
    gdefs[0].GraphicIndex = 2; gdefs[1].GraphicIndex = 1;
    NUISystem::CSpriteDef def;
    def.m_count = 1;
    def.GraphicsStart = gdefs; def.GraphicsFinish = gdefs + 2; def.GraphicsEnd = gdefs + 2;
    def.HeightValue = 16.7f; def.WidthValue = 32.9f; def.Angle = 1.5f; def.Sprite2DFlag = 7;
    g_def = &def;

    // 1: no frame data yet -> new CFrameData, BeginLoading, forced load fails, nothing fetched
    NUISystem::CSprite s;
    s.SizeX = -1.0f; s.SizeY = -1.0f; s.GraphicIndex = 99;
    reset(); g_forcedResult = false;
    s.GetDataFromDef();
    CHECK(s.Angle == 1.5f); CHECK(s.Width == 32); CHECK(s.Height == 16); CHECK(s.Sprite2DFlag == 7);
    CHECK(s.GraphicIndex == 2);
    CHECK(s.SizeX == 23.0f); CHECK(s.SizeY == 45.0f);
    CHECK(g_gifBegin == 1 && g_beginMip == 0 && g_beginFlags == 0x10);
    CHECK(g_ctor == 1); CHECK(g_fdBegin == 1); CHECK(g_forced == 1 && g_forcedFlags == 1);
    CHECK(gif.Data.P != 0 && gif.Data.P->GraphicInfoFrame == &gif);
    CHECK(def.m_count == 1); CHECK(g_destroy == 0); CHECK(info.RefCount == 1);

    // 2: cached frame at mip 0 -> touch (bank frame, resource list), frame copied and dropped
    static CResourceList list;
    list.CurrentFrame = 5;
    static CGraphicFrameData gfd;
    memset(&gfd, 0, sizeof(gfd));
    gfd.ReferenceCount = 1;
    CFrameData* fd = gif.Data.P;
    fd->Frame.FrameData = &gfd;
    fd->CurrentMipmapLevel = 0; fd->TouchedMipmapLevel = 2; fd->TouchedFrame = 1;
    fd->ResourceList = &list; fd->LastUsedFrame = 3;
    reset();
    s.GetDataFromDef();
    CHECK(g_gifBegin == 1); CHECK(g_ctor == 0); CHECK(g_fdBegin == 0); CHECK(g_forced == 0);
    CHECK(fd->TouchedFrame == 77); CHECK(fd->TouchedMipmapLevel == 0);
    CHECK(g_listRemove == 1 && g_listInsert == 1); CHECK(fd->LastUsedFrame == 5);
    CHECK(gfd.ReferenceCount == 1); CHECK(g_uninit == 0);
    CHECK(gif.Data.P == fd);
    CHECK(def.m_count == 1); CHECK(info.RefCount == 1);

    // 3: no graphics -> index 0, size untouched, no streaming
    def.GraphicsFinish = gdefs;
    s.SizeX = -2.0f; s.SizeY = -3.0f;
    reset();
    s.GetDataFromDef();
    CHECK(s.GraphicIndex == 0); CHECK(s.SizeX == -2.0f && s.SizeY == -3.0f);
    CHECK(g_gifBegin == 0); CHECK(def.m_count == 1); CHECK(g_destroy == 0);

    if (fails == 0) printf("0041ac20_TEST PASS\n");
    return fails;
}