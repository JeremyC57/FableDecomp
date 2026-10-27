// CBaseAllocator::CBaseAllocator(CHAR* pName, IUnknown* pUnk, HRESULT* phr, BOOL bEvent, BOOL fEnableReleaseCallback) -- DirectShow BaseClasses (amfilter.cpp) allocator constructor, #ifdef UNICODE CHAR overload: CUnknown base, the CCritSec lock initialised, empty free list, every counter and flag 0, the release-callback flag stored; with bEvent a semaphore (0, 0x7FFFFFFF) is created and a failure reports E_OUTOFMEMORY
// Leaf: ??0CBaseAllocator@@QAE@PADPAUIUnknown@@PAJHH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAC570, 130 bytes, thiscall `ret 0x14`, returns this; no vtable store (AM_NOVTABLE).
// Identity (proven): the body is amfilter.cpp's CBaseAllocator constructor member for member (the CSampleList pair at
//   +0x28, m_hSem +0x30 ... m_pNotify +0x58, m_fEnableReleaseCallback +0x5C, CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL),
//   *phr = E_OUTOFMEMORY); the CUnknown it calls, 0x00CACC80, is bound as ??0CUnknown@@QAE@PBDPAUIUnknown@@@Z (the CHAR
//   overload) in reloc_bindings, so this is the CHAR overload; its twin 0x00CABAE0 calls the WCHAR CUnknown 0x00CACB80.
//   Source order (TCHAR overload first, then the #ifdef UNICODE CHAR overload) matches the address order.
// Label: the manifest's "sub_00cac570" is a gap-scan placeholder.
// Callees: CUnknown::CUnknown(const char*, IUnknown*) 0x00CACC80; InitializeCriticalSection (IAT 0x0143FEE4);
//   CreateSemaphoreW (IAT 0x0143FE54).
// Layout: CUnknown {novtable INonDelegatingUnknown vptr, m_pUnknown +4, m_cRef +8}, IMemAllocatorCallbackTemp vptr +0x0C,
//   CCritSec +0x10 (CRITICAL_SECTION, 0x18 bytes), then the allocator members from +0x28 (no shared engine header).
typedef long HRESULT;
typedef int BOOL;
struct GUID;
struct IUnknown {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
struct CRITICAL_SECTION_ {
    void* DebugInfo;
    long LockCount;
    long RecursionCount;
    void* OwningThread;
    void* LockSemaphore;
    unsigned long SpinCount;
};
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(CRITICAL_SECTION_* section);   // IAT 0x0143FEE4
extern "C" __declspec(dllimport) void* __stdcall CreateSemaphoreW(void* attributes, long initial, long maximum, const unsigned short* name);   // IAT 0x0143FE54

struct __declspec(novtable) INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
class CBaseObject {
public:
    CBaseObject(const char* pName);
};
class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CBaseObject {
    IUnknown* const m_pUnknown;                      // +0x04
protected:
    long m_cRef;                                     // +0x08
public:
    CUnknown(const char* pName, IUnknown* pUnk);   // 0x00CACC80
};
struct __declspec(novtable) IMemAllocatorCallbackTemp {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
class CCritSec {
    CRITICAL_SECTION_ m_CritSec;
public:
    CCritSec() { InitializeCriticalSection(&m_CritSec); }
};
class CMediaSample;
class IMemAllocatorNotifyCallbackTemp;

class __declspec(novtable) CBaseAllocator : public CUnknown, public IMemAllocatorCallbackTemp, public CCritSec {
protected:
    class CSampleList {
    public:
        CSampleList() : m_List(0), m_nOnList(0) { }
        CMediaSample* m_List;                         // +0x28
        int m_nOnList;                                // +0x2C
    };
    CSampleList m_lFree;
    void* m_hSem;                                     // +0x30
    long m_lWaiting;                                  // +0x34
    long m_lCount;                                    // +0x38
    long m_lAllocated;                                // +0x3C
    long m_lSize;                                     // +0x40
    long m_lAlignment;                                // +0x44
    long m_lPrefix;                                   // +0x48
    BOOL m_bChanged;                                  // +0x4C
    BOOL m_bCommitted;                                // +0x50
    BOOL m_bDecommitInProgress;                       // +0x54
    IMemAllocatorNotifyCallbackTemp* m_pNotify;       // +0x58
    BOOL m_fEnableReleaseCallback;                    // +0x5C
public:
    CBaseAllocator(char* pName, IUnknown* pUnk, HRESULT* phr, BOOL bEvent = 1, BOOL fEnableReleaseCallback = 0);
};

CBaseAllocator::CBaseAllocator(char* pName, IUnknown* pUnk, HRESULT* phr, BOOL bEvent, BOOL fEnableReleaseCallback)
    : CUnknown(pName, pUnk),
      m_lAllocated(0),
      m_bChanged(0),
      m_bCommitted(0),
      m_bDecommitInProgress(0),
      m_lSize(0),
      m_lCount(0),
      m_lAlignment(0),
      m_lPrefix(0),
      m_hSem(0),
      m_lWaiting(0),
      m_fEnableReleaseCallback(fEnableReleaseCallback),
      m_pNotify(0)
{
    if (bEvent) {
        m_hSem = CreateSemaphoreW(0, 0, 0x7FFFFFFF, 0);
        if (m_hSem == 0) {
            *phr = (HRESULT)0x8007000EL;
            return;
        }
    }
}