// NUISystem::CMorphingSprite::GetDataFromDef() -- records every state's graphic index in GraphicIndices and starts loading (and forces) each state graphic's first frame
// Grade: DIFFER(727v727) -- residue (23 mnemonic diffs, same length): the pair temporary's first/second stores are scheduled around the insert pushes differently, and scratch-register colouring (eax/edx vs ecx/edx) in the two inlined CResource::Touch copies and the frame-data release; lane gap: register colouring | tried: renamed to the PDB model (CUIDef::States, CUIStateDef::GraphicIndex, CManager::GetGraphicsBank by value, CGraphicDataBank::PeekGraphic/PeekGraphicInfoFrame, const CGraphicInfoFrame::GetFrame, CGraphicInfoFrame::CFrameData) and dropped the stray size pragma (62 -> 48), STLport vector operator[] through begin() (48 -> 31: the States start is loaded before GetInstance as retail), a named head local in Touch (-> 23), pair forms (make_pair, value_type, by-value ctor: worse or inert); 1 RTM grade this pass (17 total) | next: model CResource::Touch's list insert through the real CResourceList helper if one exists in the PDB, else accept as a lane gap
// Leaf: ?GetDataFromDef@CMorphingSprite@NUISystem@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005542F0, 727 bytes, thiscall, default-flags TU (no optimize pragma:
//   magic-multiply divisions), no frame pointer.
// Identity: Ego_r.pdb / FableWin.pdb ?GetDataFromDef@CMorphingSprite@NUISystem@@QAEXXZ.
// Callees: CComponent::LoadAndReturnDef() const (vtable slot 108, +0x1B0; CDefPointer<const CUIDef> by hidden
//   pointer), STLport _Rb_tree<unsigned long, pair<const unsigned long, unsigned long>, ...>::insert_unique(iterator,
//   const value_type&) 0x005537F0 (map::insert(position, value) inline), NUISystem::CManager::GetInstance() 0x0041E5F2,
//   CManager::GetGraphicsBank() (inline, CCountedPointer<CGraphicDataBank> by value), CGraphicDataBank::PeekGraphic /
//   PeekGraphicInfoFrame (inline), CGraphicInfoFrame::BeginLoading(unsigned long, unsigned long) const 0x009FE620, the
//   inline CGraphicInfoFrame::GetFrame(CGraphicFrame&, unsigned long, unsigned long) const with
//   CFrameData::BeginLoading 0x009FDE80, new CFrameData(const CGraphicInfoFrame*) 0x009FE380 (operator new 0x00BFEA1A),
//   CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90, CGraphicInfoFrame::InternalGetFrameForcedLoading
//   0x009FF6B0, CGraphicFrameData::Uninitialise 0x009FCBA0, operator delete 0x00BFE9BC.
// Layout: CMorphingSprite GraphicIndices +0x18C (STLport map<unsigned long, unsigned long>); CUIDef States +0x40
//   (vector<CUIStateDef>, 0x7C-byte states, GraphicIndex +0x3C); CManager PGraphicsBank +0x10; CGraphicDataBank
//   GraphicList +0x1E0 (CArray<CGraphicInfo>, 0x2C-byte infos), the resource list's CurrentFrame +0x194.
//   Local model: engine/CMorphingSprite.h flattens the vptr into a data member, so the slot-108 virtual call needs a
//   polymorphic local model (header debt).

void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* block);             // 0x00BFE9BC

class CResourceList;
class CResource {
public:
    virtual void v00();
    virtual void Release();                            // slot 1: the CIVCountedPointer release on the last reference
    unsigned long  RefCount;                           // +0x04 (CIVCountedPointeeBase count)
    CResourceList* ResourceList;                       // +0x08
    CResource*     PrevResource;                       // +0x0c
    CResource*     NextResource;                       // +0x10
    unsigned long  ResourceSize;                       // +0x14
    unsigned long  LastUsedFrame;                      // +0x18
    __forceinline void Touch();
private:
    void ListRemove();                                 // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next); // 0x009FBF90
};
class CResourceList {
public:
    unsigned long _pad_0x00;
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
        CResource* head = &ResourceList->Head;
        ListInsert(head->PrevResource, head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}

class CCPPointerInfo {
public:
    unsigned long RefCount;                            // +0x00
    void (__fastcall* DeleteFunc)(void* data);         // +0x04
    void*         Data;                                // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    T* operator->() const { return Data; }
};
class CTexture;
template <> class CCountedPointer<CTexture> {
public:
    CTexture*       Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
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
class CGraphicFrameData {
public:
    unsigned long             ReferenceCount;          // +0x00
    void*                     Data;                    // +0x04
    CCountedPointer<CTexture> PTexture;                // +0x08
    unsigned short            TLX, TLY, Width, Height, Pitch;   // +0x10
    unsigned char             Alpha : 1;               // +0x1a
    unsigned char             BooleanAlpha : 1;
    unsigned char             UsesSurface : 1;
    unsigned char             SectionOfTPage : 1;
    unsigned char             IsInitialised : 1;
    unsigned char             ScreenGraphic : 1;
    void Uninitialise();                               // 0x009FCBA0
    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    __forceinline void RemoveReference()
    {
        if (--ReferenceCount == 0) {
            this->~CGraphicFrameData();
            operator delete(this);
        }
    }
};
class CGraphicFrame {
public:
    CGraphicFrameData* FrameData;                      // +0x00
    CGraphicFrame() : FrameData(0) { }
    __forceinline ~CGraphicFrame() { if (FrameData) FrameData->RemoveReference(); }
    __forceinline CGraphicFrame& operator=(const CGraphicFrame& other)
    {
        if (FrameData)
            FrameData->RemoveReference();
        FrameData = other.FrameData;
        if (FrameData)
            ++FrameData->ReferenceCount;
        return *this;
    }
};
template <class T> class CIVCountedPointer {
public:
    T* Object;                                         // +0x00
    __forceinline CIVCountedPointer& operator=(T* object)
    {
        if (Object) {
            Object->DecRef();
            Object = 0;
        }
        Object = object;
        return *this;
    }
    T* operator->() const { return Object; }
    operator bool() const { return Object != 0; }
};
class CManagedTextureData : public CResource { };
class CManagedTexture {
public:
    unsigned long                         _pad_0x00;
    CIVCountedPointer<CManagedTextureData> Data;       // +0x04
    __forceinline void Touch() { if (Data) Data->Touch(); }
};
class CGraphicInfo;
class CGraphicDataBank;
class CGraphicInfoFrame {
public:
    class CFrameData : public CResource {
    public:
        CGraphicFrame      Frame;                      // +0x1c
        CManagedTexture    ManagedTexture;             // +0x20
        CManagedTexture    LoadingManagedTexture;      // +0x28
        void*              FileData[2];                // +0x30 CCountedPointer<CBankFileAsyncData>
        signed char        CurrentMipmapLevel;         // +0x38
        signed char        LoadingMipmapLevel;         // +0x39
        signed char        TouchedMipmapLevel;         // +0x3a
        unsigned long      TouchedFrame;               // +0x3c
        CGraphicInfoFrame* GraphicInfoFrame;           // +0x40
        CFrameData(const CGraphicInfoFrame* frame);    // 0x009FE380
        void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FDE80
        __forceinline void DecRef() { if (--RefCount == 0) Release(); }
    };
    unsigned long                         _pad_0x00;
    unsigned short                        FrameNum;          // +0x04
    signed char                           SmallMipmapLevel;  // +0x06
    mutable CIVCountedPointer<CFrameData> Data;              // +0x08
    CGraphicInfo*                         GraphicInfo;       // +0x0c
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags) const;   // 0x009FE620
    __forceinline bool GetFrame(CGraphicFrame& frame, unsigned long mipmapLevel, unsigned long flags) const;
private:
    bool InternalGetFrameForcedLoading(unsigned long flags) const;   // 0x009FF6B0
};
template <class T> class CArray {                      // Lionhead vector (STLport layout, 12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned long size() const;
    const T& operator[](unsigned long n) const;
};
template <class T> inline unsigned long CArray<T>::size() const { return end() - begin(); }
template <class T> inline const T& CArray<T>::operator[](unsigned long n) const { return *(begin() + n); }
class CGraphicDataBank {
public:
    unsigned char        _pad_0x000[0x194];
    unsigned long        CurrentFrame;                 // +0x194 (the bank's resource list CurrentFrame)
    unsigned char        _pad_0x198[0x1e0 - 0x198];
    CArray<CGraphicInfo> GraphicList;                  // +0x1e0
    const CGraphicInfo* PeekGraphic(unsigned long index) const;
    const CGraphicInfoFrame* PeekGraphicInfoFrame(unsigned long graphicIndex, unsigned long frameIndex) const;
};
class CGraphicInfo {
public:
    unsigned char      Header[0x1c];                   // +0x00 CGraphicHeader
    long               TextureType;                    // +0x1c
    CGraphicDataBank*  Bank;                           // +0x20
    long               Index;                          // +0x24
    CGraphicInfoFrame* Frames;                         // +0x28
};
__forceinline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
__forceinline const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphicIndex, unsigned long frameIndex) const
{
    return PeekGraphic(graphicIndex) ? &PeekGraphic(graphicIndex)->Frames[frameIndex] : 0;
}

__forceinline bool CGraphicInfoFrame::GetFrame(CGraphicFrame& frame, unsigned long mipmapLevel, unsigned long flags) const
{
    if (Data) {
        if (Data->Frame.FrameData) {
            if (!(flags & 8)) {
                unsigned long current = GraphicInfo->Bank->CurrentFrame;
                if (Data->TouchedFrame != current || mipmapLevel <= (unsigned long)Data->TouchedMipmapLevel) {
                    Data->TouchedFrame = current;
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
                    frame = Data->Frame;
                    return true;
                }
            } else {
                frame = Data->Frame;
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
    frame = Data->Frame;
    return true;
}

namespace NUISystem {
class CManager {
public:
    unsigned char                     _pad_0x00[0x10];
    CCountedPointer<CGraphicDataBank> PGraphicsBank;   // +0x10
    CCountedPointer<CGraphicDataBank> GetGraphicsBank() { return PGraphicsBank; }
    static CManager* __fastcall GetInstance();         // 0x0041E5F2
};
}

class CDefVirtual {
public:
    virtual void v00();
    virtual void Destroy();                            // slot 1
};
class CRefCounted {                                    // the count at +4 behind the vptr
public:
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};

namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Pair> struct _Select1st { };
template <class _Tp> struct less { };
template <class _Tp> struct _Nonconst_traits { };
struct _Rb_tree_node_base {
    bool                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0c
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    unsigned char       _M_key_compare;                 // +0x08
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator insert_unique(iterator __position, const _Value& __v);   // 0x005537F0
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const _Tp* begin() const { return _M_start; }
    const _Tp& operator[](unsigned int __n) const { return *(begin() + __n); }
};
}

namespace NUISystem {
class CUIStateDef {
public:
    unsigned char _pad_0x00[0x3c];
    unsigned long GraphicIndex;                        // +0x3c
    unsigned char _pad_0x40[0x7c - 0x40];
};
class CUIDef : public CDefVirtual, public CRefCounted {
public:
    unsigned char            _pad_0x08[0x40 - 0x08];
    std::vector<CUIStateDef> States;                   // +0x40
    void ReleaseTemporary() const { const_cast<CUIDef*>(this)->DecRef(); if (GetRefCount() == 0) const_cast<CUIDef*>(this)->Destroy(); }
};
}
template <class T> class CDefPointer {
public:
    T* Object;
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};

#define VF(n) virtual void v##n();
namespace NUISystem {
class CMorphingSprite {
public:
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
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() const;   // slot 108 (+0x1b0)
    unsigned char _pad_0x04[0x18c - 0x04];
    std::map<unsigned long, unsigned long> GraphicIndices;        // +0x18c
    void GetDataFromDef();
};

void CMorphingSprite::GetDataFromDef()
{
    CDefPointer<const CUIDef> def = LoadAndReturnDef();
    for (unsigned long i = 0; i < def->States.size(); ++i) {
        GraphicIndices.insert(GraphicIndices.begin(), std::pair<const unsigned long, unsigned long>(i, def->States[i].GraphicIndex));
        const CGraphicInfoFrame* frame = CManager::GetInstance()->GetGraphicsBank()->PeekGraphicInfoFrame(def->States[i].GraphicIndex, 0);
        frame->BeginLoading(0, 0x10);
        CGraphicFrame graphic;
        frame->GetFrame(graphic, 0, 1);
    }
}
}
