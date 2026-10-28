// NUISystem::CIconText::~CIconText() -- clear the text and drop-shadow primitive handles and every icon handle, drop the font, release the def's font from the font manager, then the members and the changing-state component base
// Leaf: ??1CIconText@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005424B0, 627 bytes, `ret`. FableWin.pdb ??1CIconText@NUISystem@@UAE@XZ; vtables
//   0x012470CC / 0x012470A8 / 0x012470A0 (RTTI NUISystem::CIconText at +0 / +4 / +0x18: CUIInterface, CObserver, CActionParam).
// Callees: CEnginePersistentPrimitiveBase::NotifyReleased (virtual slot 3) and CIVCountedPointeeBase::Delete (slot 1) through
//   the inline CEnginePrimitiveHandle::Clear (0x0041BE70 out of line); std::for_each<CEnginePrimitiveHandle*,
//   mem_fun_ref_t<void, CEnginePrimitiveHandle> > 0x0043F360 (__fastcall, /Gr); operator delete 0x00BFE9BC (font pointer info);
//   CComponent::LoadAndReturnDef 0x00530EC0 (virtual, called directly in the destructor); CDefStringTable::GetString
//   0x009D49B0 on g_DefStringTable 0x013CA828 (inline CDefString -> CCharString conversion, built in the return slot, so the
//   ReleaseFont argument is `lea`d after the GFGetSystemManager call); __fastcall GFGetSystemManager 0x009A4EC0;
//   CFontManager::ReleaseFont 0x009E2B40; ~CCharString 0x0099EAE0; free 0x00BFEA14 (vector storage); CBaseClass::~CBaseClass
//   0x0099A2E0; ~CWideString 0x0099B510; CChangingStateComponent::~CChangingStateComponent 0x0052CCF0.
// Layout (retail, as the landed ctor 0x00542330; FableWin.pdb names): Text +0x15c, DisplayText +0x160, PFont +0x164,
//   Unavailable +0x16c, TextHandle +0x170, DropShadowHandle +0x178, IconHandles +0x180, Icons +0x18c; CUIDef::Font +0x50;
//   CSystemManager::PFontManager +0x84. Vtable orders from retail RTTI: CIVCountedPointeeBase {dtor, Delete},
//   CEnginePersistentPrimitiveBase 0x012A3D60 {dtor, Delete, CollectStats (pure), NotifyReleased, WasRendered}.
// Shape: CCountedPointer<CFontBank>::Reset(0) = ReleaseData (Data = 0, Info = 0) then Data = 0 (retail's Info-then-Data
//   stores); the CIVCountedPointer release dec-tests the pointee's IVRefCount through a local copy (`dec [ecx+4]`), while the
//   CDefPointer release calls the pointee's Release() (long from the unsigned count: the `mov edx,edi` copy) on a copy taken
//   inside the null test.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
class CBaseClass {
public:
    virtual ~CBaseClass();                              // 0x0099A2E0
};

namespace std {
template <class _Tp> struct less;
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
template <class _Key, class _Tp, class _Compare, class _Alloc> class map;
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start) free(_M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
template <class _Ret, class _Tp> class mem_fun_ref_t {
public:
    explicit mem_fun_ref_t(_Ret (_Tp::*__pf)()) : _M_f(__pf) { }
    _Ret operator()(_Tp& __r) const { return (__r.*_M_f)(); }
private:
    _Ret (_Tp::*_M_f)();
};
template <class _Ret, class _Tp> inline mem_fun_ref_t<_Ret, _Tp> mem_fun_ref(_Ret (_Tp::*__f)()) { return mem_fun_ref_t<_Ret, _Tp>(__f); }
template <class _InputIter, class _Function> _Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f);   // 0x0043F360
} // namespace std

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                        // slot 1
    long GetRefCount() const { return IVRefCount; }
    void AddRef() const { ++IVRefCount; }
    long Release() const { return --IVRefCount; }
    mutable unsigned long IVRefCount;                   // +0x04
};
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    ~CIVCountedPointer() { Release(); }
    T* operator->() const { return (T*)Object; }
    unsigned long GetRefCount() const { return Object ? Object->GetRefCount() : 0; }
    void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    void Reset(T* object)
    {
        Release();
        Object = object;
        if (object)
            object->AddRef();
    }
};
class CEnginePrimitiveHandle;
enum EEnginePrimitiveStatIndex { };
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
    friend class CEnginePrimitiveHandle;
public:
    virtual void CollectStats(std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > >& stats,
                              EEnginePrimitiveStatIndex index, unsigned long flags) const = 0;   // slot 2
protected:
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3
    virtual bool WasRendered() const;                   // slot 4
};
class CEnginePrimitiveHandle : public CBaseClass {      // vtable 0x0122F598
public:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
    virtual __forceinline ~CEnginePrimitiveHandle() { Clear(); }
    void Clear()                                        // 0x0041BE70
    {
        if (Data.GetRefCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
};

class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
    CCPPointerInfo(unsigned long refCount, void (__fastcall* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer() { ReleaseData(); }
    void ReleaseData()
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
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, Data);
    }
    static void __fastcall DeleteData(void* data);
};
class CFontBank;

class CDefPointeeBase : public CIVCountedPointeeBase { };
class CDefString {
public:
    long TablePos;
    CCharString GetString() const;
    operator CCharString() const;
};
class CDefStringTable {
public:
    CCharString GetString(long tablePos) const;         // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
inline CCharString CDefString::GetString() const { return g_DefStringTable.GetString(TablePos); }
inline CDefString::operator CCharString() const { return GetString(); }
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    ~CDefPointer() { Release(); }
    T* operator->() const { return (T*)Object; }
    void Release()
    {
        if (Object) {
            const CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
};

class CFontManager {
public:
    void ReleaseFont(const CCharString& name);          // 0x009E2B40
};
class CSystemManager {
public:
    unsigned char _pad00[0x84];
    CCountedPointer<CFontManager> PFontManager;         // +0x84
    CFontManager* GetFontManager() { return PFontManager.Data; }
};
CSystemManager* __fastcall GFGetSystemManager();        // 0x009A4EC0

class C2DVector { public: float X; float Y; };

namespace NUISystem {
enum EIconFlags { };
class CUIDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x50 - 0x08];
    CDefString    Font;                                 // +0x50
};
class CUIInterface { public: virtual ~CUIInterface(); };
class CObserver {
public:
    virtual ~CObserver();
    unsigned char _pad04[0x14 - 0x04];
};
class CActionParam { public: virtual ~CActionParam(); };
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() const;   // 0x00530EC0
    unsigned char _pad1c[0x134 - 0x1c];
};
class CChangingStateComponent : public CComponent {
public:
    unsigned char _pad134[0x15c - 0x134];
    virtual ~CChangingStateComponent();                 // 0x0052CCF0
};
class CIconText : public CChangingStateComponent {
public:
    struct CEmbeddedIcon { long Graphic; long CharIndex; EIconFlags Flags; C2DVector Size; C2DVector Offset; };
    CWideString                         Text;           // +0x15c
    CWideString                         DisplayText;    // +0x160
    CCountedPointer<CFontBank>          PFont;          // +0x164
    bool                                Unavailable;    // +0x16c
    CEnginePrimitiveHandle              TextHandle;     // +0x170
    CEnginePrimitiveHandle              DropShadowHandle;   // +0x178
    std::vector<CEnginePrimitiveHandle> IconHandles;    // +0x180
    std::vector<CEmbeddedIcon>          Icons;          // +0x18c
    virtual ~CIconText();
};

CIconText::~CIconText()
{
    TextHandle.Clear();
    DropShadowHandle.Clear();
    std::for_each(IconHandles.begin(), IconHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    PFont.Reset(0);
    GFGetSystemManager()->GetFontManager()->ReleaseFont(LoadAndReturnDef()->Font);
}
} // namespace NUISystem