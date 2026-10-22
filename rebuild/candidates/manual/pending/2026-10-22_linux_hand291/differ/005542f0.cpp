// Grade: DIFFER(728v727) -- RST2 n2 form (by-value GetGraphicBank, bank-member GetFirstFrame, CRetailVector accessors) fixes the frame slots, map-in-edi and sbb residues; left: def held in esi (retail ecx), the GraphicIndex load after GetInstance (retail loads Entries start before it), scratch rotation in the inlined GetFrame Touch
// NUISystem::CMorphingSprite::GetDataFromDef() -- probe
// Leaf: ?GetDataFromDef@CMorphingSprite
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
    CGraphicFrame() : FrameData(0) { }
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
struct CGraphicInfo;
struct CGraphicInfoVector {                            // vector<CGraphicInfo> (0x2c-byte elements)
    CGraphicInfo* _M_start;
    CGraphicInfo* _M_finish;
    CGraphicInfo* _M_end_of_storage;
    CGraphicInfo* begin() { return _M_start; }
    CGraphicInfo* end() { return _M_finish; }
    unsigned long size();
    CGraphicInfo& operator[](unsigned long n);
};
struct CGraphicDataBank {
    unsigned char _pad00[0x194];
    unsigned long CurrentFrame;                        // +0x194
    unsigned char _pad198[0x1e0 - 0x198];
    CGraphicInfoVector Infos;                          // +0x1e0
    CGraphicInfo* GetGraphicInfo(unsigned long index);
    CGraphicInfoFrame* GetFirstFrame(unsigned long index);
};
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
    bool InternalGetFrameForcedLoading(unsigned long flags);   // 0x009FF6B0
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FE620
    __forceinline bool GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags);
};

__forceinline bool CGraphicInfoFrame::GetFrame(CGraphicFrame& out, unsigned long mipmapLevel, unsigned long flags)
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
inline unsigned long CGraphicInfoVector::size() { return end() - begin(); }
inline CGraphicInfo& CGraphicInfoVector::operator[](unsigned long n) { return *(begin() + n); }
__forceinline CGraphicInfo* CGraphicDataBank::GetGraphicInfo(unsigned long index)
{
    return index < Infos.size() ? &Infos[index] : 0;
}
__forceinline CGraphicInfoFrame* CGraphicDataBank::GetFirstFrame(unsigned long index) { return GetGraphicInfo(index) ? GetGraphicInfo(index)->Frames : 0; }
struct CGraphicBankPointer {                           // CCountedPointer<CGraphicDataBank>
    CGraphicDataBank* Data;
    CCPInfo*          Info;
    CGraphicBankPointer(const CGraphicBankPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CGraphicBankPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CGraphicDataBank* operator->() const { return Data; }
};
struct CFrontEndManager {
    unsigned char       _pad00[0x10];
    CGraphicBankPointer GraphicBank;                   // +0x10
    CGraphicBankPointer GetGraphicBank() const { return GraphicBank; }
    static CFrontEndManager* __fastcall GetInstance(); // 0x0041E5F2
};
static __forceinline CGraphicInfoFrame* GetFirstFrame(const unsigned long& index)
{
    CGraphicBankPointer bank = CFrontEndManager::GetInstance()->GraphicBank;
    return bank->GetGraphicInfo(index) ? bank->GetGraphicInfo(index)->Frames : 0;
}

struct CRefCounted {                                   // second base of CDefBase: the count at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                            // slot 1
};
struct SMorphEntry {                                   // 0x7c bytes
    unsigned char _pad00[0x3c];
    unsigned long GraphicIndex;                        // +0x3c
    unsigned char _pad40[0x7c - 0x40];
};
struct CMorphEntryVector {
    SMorphEntry* _M_start;
    SMorphEntry* _M_finish;
    SMorphEntry* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const SMorphEntry& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CMorphingSpriteDef : CDefVirtual, CRefCounted {
    unsigned char     _pad08[0x40 - 0x08];
    CMorphEntryVector Entries;                         // +0x40
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointer_CMorphingSpriteDef {
    CMorphingSpriteDef* Data;
    ~CDefPointer_CMorphingSpriteDef() { if (Data) Data->ReleaseTemporary(); }
    CMorphingSpriteDef* operator->() const { return Data; }
};

struct SMorphNode {
    long        Color;
    SMorphNode* Parent;
    SMorphNode* Left;
    SMorphNode* Right;
    long        Key;
    long        Value;
};
struct CRbTreeBaseIterator {
    SMorphNode* _M_node;
};
struct CMorphMapIterator : CRbTreeBaseIterator {
    CMorphMapIterator() { }
    CMorphMapIterator(SMorphNode* node) { _M_node = node; }
    CMorphMapIterator(const CMorphMapIterator& it) { _M_node = it._M_node; }
};
struct CMorphPair {
    const long first;
    long       second;
    CMorphPair(long a, long b) : first(a), second(b) { }
};
struct CMorphMap {                                     // map<long, long>
    SMorphNode*   _M_header;
    unsigned long _M_node_count;
    CMorphMapIterator begin() const { return CMorphMapIterator(_M_header->Left); }
    CMorphMapIterator insert(CMorphMapIterator position, const CMorphPair& value);   // 0x005537F0
};

#define VF(n) virtual void v##n();
namespace NUISystem {
struct CMorphingSprite {
    VF(000) VF(001) VF(002) VF(003) VF(004) VF(005) VF(006) VF(007) VF(008) VF(009)
    VF(010) VF(011) VF(012) VF(013) VF(014) VF(015) VF(016) VF(017) VF(018) VF(019)
    VF(020) VF(021) VF(022) VF(023) VF(024) VF(025) VF(026) VF(027) VF(028) VF(029)
    VF(030) VF(031) VF(032) VF(033) VF(034) VF(035) VF(036) VF(037) VF(038) VF(039)
    VF(040) VF(041) VF(042) VF(043) VF(044) VF(045) VF(046) VF(047) VF(048) VF(049)
    VF(050) VF(051) VF(052) VF(053) VF(054) VF(055) VF(056) VF(057) VF(058) VF(059)
    VF(060) VF(061) VF(062) VF(063) VF(064) VF(065) VF(066) VF(067) VF(068) VF(069)
    VF(070) VF(071) VF(072) VF(073) VF(074) VF(075) VF(076) VF(077) VF(078) VF(079)
    VF(080) VF(081) VF(082) VF(083) VF(084) VF(085) VF(086) VF(087) VF(088) VF(089)
    VF(090) VF(091) VF(092) VF(093) VF(094) VF(095) VF(096) VF(097) VF(098) VF(099)
    VF(100) VF(101) VF(102) VF(103) VF(104) VF(105) VF(106) VF(107)
    virtual CDefPointer_CMorphingSpriteDef LoadAndReturnDef() const;   // slot 108 (+0x1b0)
    unsigned char _pad04[0x18c - 0x04];
    CMorphMap     StateGraphics;                       // +0x18c
    void GetDataFromDef();
};

void CMorphingSprite::GetDataFromDef()
{
    CDefPointer_CMorphingSpriteDef def = LoadAndReturnDef();
    for (unsigned long i = 0; i < def->Entries.size(); ++i) {
        StateGraphics.insert(StateGraphics.begin(), CMorphPair(i, def->Entries[i].GraphicIndex));
        CGraphicInfoFrame* frame = CFrontEndManager::GetInstance()->GetGraphicBank()->GetFirstFrame(def->Entries[i].GraphicIndex);
        frame->BeginLoading(0, 0x10);
        CGraphicFrame graphic;
        frame->GetFrame(graphic, 0, 1);
    }
}
}
