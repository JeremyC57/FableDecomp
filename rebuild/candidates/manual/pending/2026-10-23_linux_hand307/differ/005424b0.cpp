// Grade: DIFFER(623v627) -- three residues: (1) the font-name temporary is pushed straight from GetString's eax before GFGetSystemManager, where retail calls the manager first and pushes lea of the temp; (2) the def release lacks retail's extra 'mov edx,edi' copy; (3) the zero register and the handle address are swapped (edi/ebp)
// NUISystem::CIconText::~CIconText() -- clear the text and drop-shadow primitive handles and every icon handle, drop the font pointer, release the def's font from the font manager, then the members and the changing-state component base
// Leaf: ??1CIconText@NUISystem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005424B0, length 627 bytes (`ret`). Label correction: the manifest's "CText::~CText"
//   is this class's destructor (vtables 0x012470CC / 0x012470A8 / 0x012470A0 = RTTI NUISystem::CIconText at +0 / +4 / +0x18;
//   called from the deleting destructor 0x00543C70).
// Callees: std::for_each<CEnginePrimitiveHandle*, mem_fun_ref_t> @ 0x0043F360 (/Gr) with CEnginePrimitiveHandle::Clear @ 0x0041BE70,
//   CComponent::GetDef (slot 108, called directly in the destructor) @ 0x00530EC0, CDefStringTable::GetString @ 0x009D49B0 on the
//   table @ 0x013CA828, GFGetSystemManager @ 0x009A4EC0 (font manager +0x84), CFontManager::ReleaseFont @ 0x009E2B40,
//   ~CCharString @ 0x0099EAE0, operator delete @ 0x00BFE9BC, free @ 0x00BFEA14, CBaseClass::~CBaseClass @ 0x0099A2E0,
//   ~CWideString @ 0x0099B510, CChangingStateComponent::~CChangingStateComponent @ 0x0052CCF0.
// Layout (retail, as the landed ctor 0x00542330): Text +0x15c, DisplayText +0x160, PFont +0x164 (Data) / +0x168 (Info),
//   TextHandle +0x170, DropShadowHandle +0x178, IconHandles +0x180, Icons +0x18c; the def's font name id +0x50.
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
void operator delete(void* p);                       // 0x00BFE9BC
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
struct CCharString {
    void* m_pData;
    ~CCharString();                                  // 0x0099EAE0
};
struct CWideString {
    void* m_pData;
    ~CWideString();                                  // 0x0099B510
};
struct CBaseClass {
    virtual ~CBaseClass();                           // 0x0099A2E0
};
class CEnginePrimitiveHandle;
struct CEngineInternalPrimitiveBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1 (+0x04)
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0x0c)
    long m_handleCount;                              // +0x04
};
struct CIVCountedPointer {
    CEngineInternalPrimitiveBase* Object;
    long GetRefCount() const { return Object ? Object->m_handleCount : 0; }
    __forceinline void Release()
    {
        CEngineInternalPrimitiveBase* object = Object;
        if (object) {
            if (--object->m_handleCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->m_handleCount;
        return *this;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};
class CEnginePrimitiveHandle : public CBaseClass {   // vtable 0x0122F598
public:
    CIVCountedPointer Data;                          // +0x04
    void Clear();                                    // 0x0041BE70 (out of line)
    __forceinline void ClearInline()
    {
        if (Data.GetRefCount() == 1)
            Data.Object->OnLastHandleReleased(this);
        Data = 0;
    }
    virtual __forceinline ~CEnginePrimitiveHandle() { ClearInline(); }
};
struct CMemFunRefClear {                             // std::mem_fun_ref_t<void, CEnginePrimitiveHandle>
    void (CEnginePrimitiveHandle::*m_pmf)();
    explicit CMemFunRefClear(void (CEnginePrimitiveHandle::*pmf)()) : m_pmf(pmf) { }
};
CMemFunRefClear __fastcall for_each(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CMemFunRefClear f);   // 0x0043F360
struct CDeleter { };
struct CCPPointerInfo {
    long Count;                                      // +0x00
    void (CDeleter::*Delete)();                      // +0x04
    CDeleter* Object;                                // +0x08
};
class CFontBank;
struct CFontPointer {                                // CCountedPointer<CFontBank>
    CFontBank* Data;
    CCPPointerInfo* Info;
    __forceinline void Release()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                (Info->Object->*Info->Delete)();
                operator delete(Info);
            }
        }
        Info = 0;
        Data = 0;
    }
    __forceinline ~CFontPointer() { Release(); }
};
template <class T> struct CNativeVector {
    T* First;
    T* Last;
    T* End;
};
struct CHandleVector : CNativeVector<CEnginePrimitiveHandle> {
    ~CHandleVector()
    {
        for (CEnginePrimitiveHandle* p = First; p != Last; ++p)
            p->~CEnginePrimitiveHandle();
        if (First)
            free(First);
    }
};
struct CEmbeddedIcon { long Data[4]; };
struct CIconVector : CNativeVector<CEmbeddedIcon> {
    ~CIconVector()
    {
        if (First)
            free(First);
    }
};
struct CRefCounted {
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CComponentDef {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    CRefCounted Ref;                                 // +0x04
    unsigned char _pad08[0x50 - 0x08];
    long FontName;                                   // +0x50
    void ReleaseTemporary()
    {
        Ref.DecRef();
        if (Ref.GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};
struct CDefStringTable {
    CCharString GetString(long id);                  // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;             // 0x013CA828
struct CFontManager {
    void ReleaseFont(const CCharString& name);       // 0x009E2B40
};
struct CSystemManager {
    unsigned char _pad00[0x84];
    CFontManager* FontManager;                       // +0x84
};
CSystemManager* GFGetSystemManager();                // 0x009A4EC0

namespace NUISystem {
struct CUIInterface { virtual ~CUIInterface(); };
struct CObserver {
    virtual void v00();
    unsigned char _pad04[0x14 - 0x04];
};
struct CActionParam { virtual void v00(); };
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    V16(a) V16(b) V16(c) V16(d) V16(e) V16(f) V4(g0) V4(g1) virtual void h0(); virtual void h1(); virtual void h2();
    virtual CDefPointer<CComponentDef> GetDef();     // slot 108 (+0x1b0) 0x00530EC0
    unsigned char _pad1c[0x134 - 0x1c];
};
class CChangingStateComponent : public CComponent {
public:
    unsigned char _pad134[0x15c - 0x134];
    ~CChangingStateComponent();                      // 0x0052CCF0
};
class CIconText : public CChangingStateComponent {
public:
    CWideString            Text;                     // +0x15c
    CWideString            DisplayText;              // +0x160
    CFontPointer           PFont;                    // +0x164
    bool                   Unavailable;              // +0x16c
    CEnginePrimitiveHandle TextHandle;               // +0x170
    CEnginePrimitiveHandle DropShadowHandle;         // +0x178
    CHandleVector          IconHandles;              // +0x180
    CIconVector            Icons;                    // +0x18c
    virtual ~CIconText();
};

CIconText::~CIconText()
{
    TextHandle.ClearInline();
    DropShadowHandle.ClearInline();
    for_each(IconHandles.First, IconHandles.Last, CMemFunRefClear(&CEnginePrimitiveHandle::Clear));
    PFont.Release();
    GFGetSystemManager()->FontManager->ReleaseFont(g_DefStringTable.GetString(GetDef()->FontName));
}
}
