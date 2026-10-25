// CAMMsgEvent::WaitMsg(DWORD) -- DirectShow BaseClasses (wxutil.cpp): waits on the event while letting sent messages through (MsgWaitForMultipleObjects + PeekMessage), shrinking the timeout as time passes; TRUE when the event fired
// Leaf: ?WaitMsg@CAMMsgEvent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB01C0, 118 bytes, thiscall, `ret 4`.
// Callees: WINMM timeGetTime (IAT 0x014404B4), USER32 MsgWaitForMultipleObjects (IAT 0x014403B4, held in ebp),
// USER32 PeekMessageW (IAT 0x01440370; UNICODE build, PM_NOREMOVE). QS_SENDMESSAGE 0x40; m_hEvent is the object's first member.
// Shape: dwStartTime is homed in the dead dwTimeout argument slot. Manifest label "WaitForObjectWithMessagePump" is descriptive.
// ---- DirectShow BaseClasses model (strmbase, DX9 SDK era, UNICODE build; combase.h / wxutil.h) ----
typedef long HRESULT;
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef int BOOL;
typedef long LONG;
typedef void* HANDLE;
typedef unsigned short WCHAR;
typedef unsigned char BYTE;
typedef __int64 LONGLONG;
typedef LONGLONG REFERENCE_TIME;
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;   // guiddef.h
typedef GUID IID;
typedef GUID CLSID;
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int n);
#pragma intrinsic(memcmp)
inline int operator==(const GUID& a, const GUID& b) { return !memcmp(&a, &b, sizeof(GUID)); }   // guiddef.h IsEqualGUID
inline int operator!=(const GUID& a, const GUID& b) { return !(a == b); }
struct __declspec(novtable) IUnknown {
    virtual HRESULT __stdcall QueryInterface(const IID& riid, void** ppv) = 0;   // 0
    virtual ULONG __stdcall AddRef() = 0;                                        // 1
    virtual ULONG __stdcall Release() = 0;                                       // 2
};
struct __declspec(novtable) INonDelegatingUnknown {
    virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv) = 0;
    virtual ULONG __stdcall NonDelegatingAddRef() = 0;
    virtual ULONG __stdcall NonDelegatingRelease() = 0;
};
class CBaseObject {                               // release build: only the static live-object count
    CBaseObject(const CBaseObject&);
    void operator=(const CBaseObject&);
    static LONG m_cObjects;
public:
    CBaseObject(const WCHAR* pName);
    CBaseObject(const char* pName);
    ~CBaseObject();
};
class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CBaseObject {
    IUnknown* const m_pUnknown;                   // +0x04
protected:
    LONG m_cRef;                                  // +0x08
public:
    CUnknown(const WCHAR* pName, IUnknown* pUnk);
    virtual ~CUnknown() {}                        // slot 3
    IUnknown* GetOwner() const { return m_pUnknown; }
    HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv);
    ULONG __stdcall NonDelegatingAddRef();
    ULONG __stdcall NonDelegatingRelease();
};
#define DECLARE_IUNKNOWN \
    HRESULT __stdcall QueryInterface(const IID& riid, void** ppv) { return GetOwner()->QueryInterface(riid, ppv); } \
    ULONG __stdcall AddRef() { return GetOwner()->AddRef(); } \
    ULONG __stdcall Release() { return GetOwner()->Release(); }
extern "C" HRESULT __stdcall GetInterface(IUnknown* pUnk, void** ppv);   // 0x00CACBE0 (STDAPI)
struct CRITICAL_SECTION { void* DebugInfo; LONG LockCount; LONG RecursionCount; HANDLE OwningThread; HANDLE LockSemaphore; ULONG SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
class CCritSec {                                  // wxutil.h, release build
    CCritSec(const CCritSec&);
    CCritSec& operator=(const CCritSec&);
    CRITICAL_SECTION m_CritSec;
public:
    CCritSec();
    ~CCritSec();
    void Lock() { EnterCriticalSection(&m_CritSec); }
    void Unlock() { LeaveCriticalSection(&m_CritSec); }
};
class CAutoLock {                                 // wxutil.h
    CAutoLock(const CAutoLock&);
    CAutoLock& operator=(const CAutoLock&);
protected:
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
class CRefTime {                                  // reftime.h
public:
    REFERENCE_TIME m_time;
    CRefTime() { m_time = 0; }
    CRefTime(REFERENCE_TIME rt) { m_time = rt; }
    operator REFERENCE_TIME() const { return m_time; }
    CRefTime& operator=(const CRefTime& rt) { m_time = rt.m_time; return *this; }
    CRefTime& operator=(const LONGLONG ll) { m_time = ll; return *this; }
    CRefTime& operator+=(const CRefTime& rt) { return (*this = *this + rt); }
};
// ---- wxutil.h / strmif.h model: CAMEvent, CAMMsgEvent, IReferenceClock ----
typedef unsigned long DWORD_PTR;
typedef DWORD_PTR HEVENT;
typedef DWORD_PTR HSEMAPHORE;
extern "C" __declspec(dllimport) BOOL __stdcall ResetEvent(HANDLE hEvent);   // IAT 0x0143FED4
class CAMEvent {                                  // wxutil.h
    CAMEvent(const CAMEvent& refEvent);
    CAMEvent& operator=(const CAMEvent& refEvent);
protected:
    HANDLE m_hEvent;                              // +0x00
public:
    CAMEvent(BOOL fManualReset = 0);              // 0x00CB0190
    ~CAMEvent();                                  // 0x00CB01B0
    operator HANDLE() const { return m_hEvent; }
    void Reset() { ResetEvent(m_hEvent); }
};
class CAMMsgEvent : public CAMEvent {             // wxutil.h
public:
    BOOL WaitMsg(DWORD dwTimeout = 0xFFFFFFFF);   // INFINITE
};
struct __declspec(novtable) IReferenceClock : IUnknown {   // strmif.h
    virtual HRESULT __stdcall GetTime(REFERENCE_TIME* pTime) = 0;                               // 3 (+0x0C)
    virtual HRESULT __stdcall AdviseTime(REFERENCE_TIME baseTime, REFERENCE_TIME streamTime, HEVENT hEvent, DWORD_PTR* pdwAdviseCookie) = 0;   // 4 (+0x10)
    virtual HRESULT __stdcall AdvisePeriodic(REFERENCE_TIME startTime, REFERENCE_TIME periodTime, HSEMAPHORE hSemaphore, DWORD_PTR* pdwAdviseCookie) = 0;   // 5
    virtual HRESULT __stdcall Unadvise(DWORD_PTR dwAdviseCookie) = 0;                           // 6 (+0x18)
};
struct HWND__ { int unused; };
typedef HWND__* HWND;
typedef unsigned int UINT;
typedef unsigned int WPARAM;
typedef long LPARAM;
struct POINT { LONG x; LONG y; };
struct MSG { HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam; DWORD time; POINT pt; };
extern "C" __declspec(dllimport) DWORD __stdcall timeGetTime();                                       // IAT 0x014404B4
extern "C" __declspec(dllimport) DWORD __stdcall MsgWaitForMultipleObjects(DWORD nCount, const HANDLE* pHandles,
                                                                            BOOL fWaitAll, DWORD dwMilliseconds, DWORD dwWakeMask);   // IAT 0x014403B4
extern "C" __declspec(dllimport) BOOL __stdcall PeekMessageW(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax,
                                                             UINT wRemoveMsg);                        // IAT 0x01440370

BOOL CAMMsgEvent::WaitMsg(DWORD dwTimeout)
{
    // wait for the event to be signalled, or for the
    // timeout (in MS) to expire.  allow SENT messages
    // to be processed while we wait
    DWORD dwWait;
    DWORD dwStartTime;

    // set the waiting period.
    DWORD dwWaitTime = dwTimeout;

    // the timeout will eventually run down as we iterate
    // processing messages.  grab the start time so that
    // we can calculate elapsed times.
    if (dwWaitTime != 0xFFFFFFFF) {   // INFINITE
        dwStartTime = timeGetTime();
    }

    do {
        dwWait = MsgWaitForMultipleObjects(1, &m_hEvent, 0, dwWaitTime, 0x40);   // QS_SENDMESSAGE
        if (dwWait == 0 + 1) {   // WAIT_OBJECT_0 + 1
            MSG Message;
            PeekMessageW(&Message, 0, 0, 0, 0);   // PM_NOREMOVE

            // If we have an explicit length of time to wait calculate
            // the next wake up point - which might be now.
            // If dwTimeout is INFINITE, it stays INFINITE
            if (dwWaitTime != 0xFFFFFFFF) {

                DWORD dwElapsed = timeGetTime() - dwStartTime;

                dwWaitTime =
                    (dwElapsed >= dwTimeout)
                        ? 0  // wake up with WAIT_TIMEOUT
                        : dwTimeout - dwElapsed;
            }
        }
    } while (dwWait == 0 + 1);

    // return TRUE if we woke on the event handle,
    //        FALSE if we timed out.
    return (dwWait == 0);   // WAIT_OBJECT_0
}