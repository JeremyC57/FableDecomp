// CBase_00ca9d00::~CBase_00ca9d00() -- EH-framed destructor of the strmbase-style COM object under CVtable_012bf83c: close the +0x30 handle, release the +0x58 COM pointer, delete the +0x10 critical section, then the CBaseObject subobject's out-of-line destructor
// Grade: DIFFER(106v108) -- EH frame: retail reserves an 8-byte frame (`sub esp,8`, this saved at [esp+8], one slot unused) where ours has `push ecx` (this at [esp+4]); the DeleteCriticalSection argument goes through eax in retail, edx in ours; all members, offsets and EH states match; tried CBaseObject empty/4-byte, inline virtual ~CUnknown | retry 6 (Fable), 3 tries: CBaseObject as a direct base of the derived class gives retail's `sub esp,8` frame but MSVC then orders the polymorphic IStreamCallback base before it (CBaseObject lands at +8, m_cRef at +4); dropping the user `virtual ~CUnknown()` and a local-pointer Release change nothing; next: CBaseObject direct base placed after a CUnknown that holds only INonDelegatingUnknown, with m_cRef kept in CUnknown, so the direct base sits at +4
// Leaf: ??1CBase_00ca9d00
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA9D00, length 108 bytes (padded to 112).
// Label correction: the catalog's "CVertexShaderData::~CVertexShaderData" is a propagated label (CVertexShaderData is the 20-byte
// class with vtable 0x0129DA50). The only caller is the landed derived destructor 0x00CAA410 (CVtable_012bf83c, primary vtable
// 0x012BF83C, interface vtable 0x012BF810 at +0xc), whose model names this base CBase_00ca9d00.
// Callees: CloseHandle (IAT 0x0143FE30), the +0x58 COM pointer's Release (slot 2, `__stdcall`), DeleteCriticalSection
// (IAT 0x0143FDE4), CBaseObject::~CBaseObject @ 0x00CACAF0 (strmbase: decrements the active-object count; manifest
// "Refcount_DecrementUnloadDLL").
// Layout (retail): CUnknown {vptr +0, CBaseObject (4-byte name word) +4, m_cRef +8} (AM_NOVTABLE), interface vptr +0xc, CCritSec
// {CRITICAL_SECTION} +0x10..+0x27, handle +0x30, COM pointer +0x58.
// Shape: no vtable stores (the class and CUnknown are `__declspec(novtable)`, the interface a novtable COM interface); EH state 1
// covers the body and the CCritSec member, state -1 the CBaseObject base destructor.
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);                 // IAT 0x0143FE30
struct _RTL_CRITICAL_SECTION_Retail {
    void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount;
};
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);   // IAT 0x0143FDE4
struct __declspec(novtable) IUnknownRetail {
    virtual long __stdcall QueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
struct __declspec(novtable) INonDelegatingUnknownRetail {
    virtual long __stdcall NonDelegatingQueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
class CBaseObject {                                // strmbase object counter base (4 bytes in this build)
public:
    const char* m_pName;                           // +0x04 in CUnknown
    ~CBaseObject();                                // 0x00CACAF0
};
class __declspec(novtable) CUnknown : public INonDelegatingUnknownRetail, public CBaseObject {
public:
    long m_cRef;                                   // +0x08
    virtual ~CUnknown() {}                         // strmbase: `virtual ~CUnknown() {};`
};
struct __declspec(novtable) IStreamCallback_012bf810 : IUnknownRetail {   // the second interface (vtable 0x012BF810 in the derived class)
};
class CCritSec {
    _RTL_CRITICAL_SECTION_Retail m_CritSec;        // +0x00
public:
    ~CCritSec() { DeleteCriticalSection(&m_CritSec); }
};
class __declspec(novtable) CBase_00ca9d00 : public CUnknown, public IStreamCallback_012bf810 {
public:
    CCritSec        m_Lock;                        // +0x10
    unsigned char   _pad28[0x08];                  // +0x28
    void*           m_hHandle;                     // +0x30
    unsigned char   _pad34[0x24];                  // +0x34
    IUnknownRetail* m_pObject;                     // +0x58
    unsigned char   _pad5c[0x04];                  // +0x5c
    ~CBase_00ca9d00();
};
CBase_00ca9d00::~CBase_00ca9d00()
{
    if (m_hHandle)
        CloseHandle(m_hHandle);
    if (m_pObject)
        m_pObject->Release();
}
