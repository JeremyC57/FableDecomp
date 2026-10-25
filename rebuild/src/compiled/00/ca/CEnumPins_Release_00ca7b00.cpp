// CEnumPins::Release() -- DirectShow BaseClasses amfilter.cpp: drop one reference on the pin enumerator and destroy it (virtual deleting destructor) when the count reaches zero; returns the new count
// Leaf: ?Release@CEnumPins@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA7B00, 42 bytes (+6 int3). __stdcall (STDMETHODIMP_(ULONG), `ret 4`), IEnumPins
//   vtable 0x012BF69C slot 2 (QueryInterface 0x00CA7A80, AddRef 0x00CA7AE0, Next 0x00CAAFC0, Skip 0x00CAB0E0, Reset 0x00CA7B30,
//   Clone 0x00CABED0, scalar deleting destructor 0x00CAAF90 at slot 7).
// Callees: InterlockedDecrement (KERNEL32 IAT 0x0143FD38; declared with a plain long* addend, same _InterlockedDecrement@4 import);
//   the virtual scalar deleting destructor (slot 7, `push 1 ; call [edx+1Ch]`) behind the `delete this` null test.
// Layout (amfilter.h, release): vptr +0x00, m_Position +0x04, m_PinCount +0x08, m_pFilter +0x0C, m_Version +0x10,
//   m_cRef +0x14, m_PinCache (CGenericList<CBasePin>) +0x18.
// Shape: amfilter.cpp verbatim.
// Label: manifest "InterlockedDecrement_WithCleanup" is a descriptive placeholder; the real name is the strmbase member.
// ---- strmbase model (DirectShow BaseClasses, DX9 SDK era: combase.h / wxutil.h; UNICODE, release build) ----
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int n);
#pragma intrinsic(memcmp)
inline int IsEqualGUID(const GUID& a, const GUID& b) { return !memcmp(&a, &b, sizeof(GUID)); }
inline int operator==(const GUID& a, const GUID& b) { return IsEqualGUID(a, b); }
typedef __int64 REFERENCE_TIME;
typedef __int64 LONGLONG;
#define NOERROR 0L
#define S_OK 0L
#define S_FALSE 1L
#define E_POINTER ((long)0x80004003L)
#define E_OUTOFMEMORY ((long)0x8007000EL)
struct IUnknown {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;   // 0
    virtual unsigned long __stdcall AddRef() = 0;                              // 1 (+0x04)
    virtual unsigned long __stdcall Release() = 0;                             // 2 (+0x08)
};
struct INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
struct CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                              // IAT 0x0143FDE0
extern "C" __declspec(dllimport) int __stdcall ResetEvent(void* event);                            // IAT 0x0143FED4
class CCritSec {                                 // wxutil.h
    CRITICAL_SECTION m_CritSec;
public:
    void Lock() { EnterCriticalSection(&m_CritSec); }
    void Unlock() { LeaveCriticalSection(&m_CritSec); }
};
class CAutoLock {                                // wxutil.h
protected:
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
class CAMEvent {                                 // wxutil.h (holds only the HANDLE)
    void* m_hEvent;
public:
    operator void*() const { return m_hEvent; }
    void Set() { SetEvent(m_hEvent); }
    void Reset() { ResetEvent(m_hEvent); }
};
#define CheckPointer(p, ret) { if ((p) == 0) return (ret); }
class CUnknown : public INonDelegatingUnknown {  // combase.h (CBaseObject is empty in release)
public:
    IUnknown* m_pUnknown;                        // +0x04 (const LPUNKNOWN)
    long m_cRef;                                 // +0x08
    long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv);   // 0x00CACCB0
    virtual void CUnknown_v03();                 // primary slot 3 (the virtual destructor)
};

extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement(long* lpAddend);   // IAT 0x0143FD38
class CBaseFilter;
struct IPin;
struct IEnumPins : IUnknown {
    virtual long __stdcall Next(unsigned long cPins, IPin** ppPins, unsigned long* pcFetched) = 0;   // 3
    virtual long __stdcall Skip(unsigned long cPins) = 0;                                            // 4
    virtual long __stdcall Reset() = 0;                                                              // 5
    virtual long __stdcall Clone(IEnumPins** ppEnum) = 0;                                            // 6
};
class CEnumPins : public IEnumPins {             // amfilter.h; release layout
    int m_Position;                              // +0x04 current ordinal position
    int m_PinCount;                              // +0x08 number of pins available
    CBaseFilter* m_pFilter;                      // +0x0C the filter who owns us
    long m_Version;                              // +0x10 pin version information
    long m_cRef;                                 // +0x14
public:
    virtual ~CEnumPins();                        // 7 (+0x1C) scalar deleting destructor 0x00CAAF90
    long __stdcall QueryInterface(const GUID& riid, void** ppv);                                // 0x00CA7A80
    unsigned long __stdcall AddRef();                                                           // 0x00CA7AE0
    unsigned long __stdcall Release();                                                          // 0x00CA7B00
    long __stdcall Next(unsigned long cPins, IPin** ppPins, unsigned long* pcFetched);         // 0x00CAAFC0
    long __stdcall Skip(unsigned long cPins);                                                   // 0x00CAB0E0
    long __stdcall Reset();                                                                     // 0x00CA7B30
    long __stdcall Clone(IEnumPins** ppEnum);                                                   // 0x00CABED0
};

unsigned long __stdcall CEnumPins::Release()
{
    unsigned long cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0) {
        delete this;
    }
    return cRef;
}