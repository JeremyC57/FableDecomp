// CBaseAllocator::~CBaseAllocator() -- DirectShow strmbase allocator base destructor: close the free-list semaphore, release the notify callback, then destroy the CCritSec and CUnknown bases
// Leaf: ??1CBaseAllocator
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA9D00, length 108 bytes (padded to 112).
// Label correction: the catalog's "CVertexShaderData::~CVertexShaderData" is a propagated label. This is strmbase's
// CBaseAllocator::~CBaseAllocator (amfilter.cpp): `if (m_hSem != NULL) CloseHandle(m_hSem); if (m_pNotify) m_pNotify->Release();`.
// Its only caller is CMemAllocator::~CMemAllocator 0x00CAA410 (vtable 0x012BF83C, CMemAllocator per the retry 10b21 labels).
// The EH unwind map at FuncInfo 0x0137143C proves the base list: state 0 calls 0x00CA7780 (CUnknown::~CUnknown, out of line)
// on `this`; state 1 null-checks `this`, adds 0x10 and calls 0x00CA4870 (CCritSec::~CCritSec) -- the null-checked
// this-adjust is a base-class conversion, so CCritSec is a third BASE (strmbase: `class CBaseAllocator : public CUnknown,
// public IMemAllocatorCallbackTemp, public CCritSec`), not a member; its temporary is retail's second frame slot.
// Callees: CloseHandle (IAT 0x0143FE30), IMemAllocatorNotifyCallbackTemp::Release (slot 2, `__stdcall`),
// DeleteCriticalSection (IAT 0x0143FDE4), CBaseObject::~CBaseObject @ 0x00CACAF0 (strmbase: decrements the object count).
// Layout (retail, strmbase release build): CUnknown {vptr +0, CBaseObject (empty, EBO) +4, m_pUnknown +4, m_cRef +8},
// IMemAllocatorCallbackTemp vptr +0xc, CCritSec {CRITICAL_SECTION} +0x10, m_lFree {m_List, m_nOnList} +0x28, m_hSem +0x30,
// m_lWaiting..m_bDecommitInProgress +0x34..+0x57, m_pNotify +0x58, m_fEnableReleaseCallback +0x5c (size 0x60). strmbase
// declares m_cRef with a cv-qualifier for interlocked access; it is a plain LONG here (the destructor never touches it).
// Shape: no vtable stores (AM_NOVTABLE class and CUnknown, novtable COM interfaces); EH state 1 covers the body and the
// CCritSec base, state 0 the CUnknown base whose inline empty destructor leaves only the CBaseObject call (state -1).
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
struct __declspec(novtable) INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
namespace N_00cacaf0 { class CBaseObject {                                // strmbase object counter (no data in the release build)
public:
    ~CBaseObject();                                // 0x00CACAF0
}; } using N_00cacaf0::CBaseObject;
class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CBaseObject {
    IUnknownRetail* const m_pUnknown;              // +0x04
protected:
    long m_cRef;                                   // +0x08
public:
    virtual ~CUnknown() {}                         // out-of-line copy 0x00CA7780 (unwind)
};
struct __declspec(novtable) IMemAllocatorCallbackTemp : IUnknownRetail {   // +0x0c
};
struct __declspec(novtable) IMemAllocatorNotifyCallbackTemp : IUnknownRetail {
};
class CCritSec {
    _RTL_CRITICAL_SECTION_Retail m_CritSec;        // +0x00
public:
    ~CCritSec() { DeleteCriticalSection(&m_CritSec); }   // out-of-line copy 0x00CA4870 (unwind)
};
class CMediaSample;
class __declspec(novtable) CBaseAllocator : public CUnknown, public IMemAllocatorCallbackTemp, public CCritSec {
protected:
    class CSampleList {
    public:
        CMediaSample* m_List;                      // +0x28
        int           m_nOnList;                   // +0x2c
    };
    CSampleList m_lFree;                           // +0x28
    void*       m_hSem;                            // +0x30
    long        m_lWaiting;                        // +0x34
    long        m_lCount;                          // +0x38
    long        m_lAllocated;                      // +0x3c
    long        m_lSize;                           // +0x40
    long        m_lAlignment;                      // +0x44
    long        m_lPrefix;                         // +0x48
    int         m_bChanged;                        // +0x4c
    int         m_bCommitted;                      // +0x50
    int         m_bDecommitInProgress;             // +0x54
    IMemAllocatorNotifyCallbackTemp* m_pNotify;    // +0x58
    int         m_fEnableReleaseCallback;          // +0x5c
public:
    ~CBaseAllocator();
};

CBaseAllocator::~CBaseAllocator()
{
    if (m_hSem != 0)
        CloseHandle(m_hSem);
    if (m_pNotify)
        m_pNotify->Release();
}