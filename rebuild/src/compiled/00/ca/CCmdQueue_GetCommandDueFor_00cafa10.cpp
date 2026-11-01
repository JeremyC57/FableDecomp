// CCmdQueue::GetCommandDueFor(REFERENCE_TIME, CDeferredCommand**) -- DirectShow BaseClasses ctlutil.cpp: under the lock return the first presentation-time command already due (AddRef'd), else the first stream-time command due by rtStream (AddRef'd), else while running a presentation command due by rtStream + m_StreamTimeOffset; VFW_E_NOT_FOUND otherwise
// Leaf: ?GetCommandDueFor@CCmdQueue@@UAEJ_JPAPAVCDeferredCommand@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAFA10, 395 bytes, thiscall `ret 0Ch`, /EHsc frame (the CAutoLock is EH state 0).
// Identity: DirectShow BaseClasses ctlutil.cpp CCmdQueue::GetCommandDueFor: the CCmdQueue layout of the landed rows (m_Lock +0x08,
//   m_listPresentation +0x20, m_listStream +0x38, m_pClock +0x60, m_bRunning +0x64, m_StreamTimeOffset +0x68), the inlined CheckTime
//   (IReferenceClock::GetTime, slot 3) and VFW_E_NOT_FOUND 0x80040216 are the SDK text, including the SDK's `pPresent->AddRef()` in
//   the stream branch (retail 0x00CAFB03 AddRefs edi = the presentation head) and no AddRef in the running branch. It sits between
//   CCmdQueue::GetDueCommand 0x00CAF8F0 and CPosPassThru::QueryInterface 0x00CAFC40; nothing references it (novtable strmbase
//   class, objects linked whole). The catalog's "CEditControlCentre::FindTrackNodeInTimeRange" is wrong.
// Callees: Enter/LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10), CBaseList::GetI 0x00CB13A0.
// ---- strmbase model (DirectShow BaseClasses, DX9 SDK era, UNICODE, release: combase.h / wxutil.h / wxlist.h / reftime.h / ctlutil.h) ----
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;
typedef __int64 REFERENCE_TIME;
typedef __int64 LONGLONG;
typedef double REFTIME;
typedef void* HANDLE;
typedef unsigned long DWORD_PTR;
typedef struct __POSITION { int unused; }* POSITION;
struct VARIANT;
#define NOERROR 0L
#define S_OK 0L
#define E_ABORT ((long)0x80004004L)
#define INFINITE 0xFFFFFFFF
#define WAIT_OBJECT_0 0
#define VFW_E_NOT_FOUND ((long)0x80040216L)
#define NAME(_x_) ((const unsigned short*)0)
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
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FEE4
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(CRITICAL_SECTION* section);       // IAT 0x0143FDE4
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);        // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);        // IAT 0x0143FE10
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(HANDLE hHandle, unsigned long dwMilliseconds);   // IAT 0x0143FEAC
class CCritSec {                                 // wxutil.h
    CCritSec(const CCritSec& refCritSec);
    CCritSec& operator=(const CCritSec& refCritSec);
    CRITICAL_SECTION m_CritSec;
public:
    CCritSec() { InitializeCriticalSection(&m_CritSec); }
    ~CCritSec() { DeleteCriticalSection(&m_CritSec); }
    void Lock() { EnterCriticalSection(&m_CritSec); }
    void Unlock() { LeaveCriticalSection(&m_CritSec); }
};
class CAutoLock {                                // wxutil.h
    CAutoLock(const CAutoLock& refAutoLock);
    CAutoLock& operator=(const CAutoLock& refAutoLock);
protected:
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
class CAMEvent {                                 // wxutil.h (DX9 era: no phr argument)
    CAMEvent(const CAMEvent& refEvent);
    CAMEvent& operator=(const CAMEvent& refEvent);
protected:
    HANDLE m_hEvent;
public:
    CAMEvent(int fManualReset = 0);              // 0x00CB0190
    ~CAMEvent();                                 // 0x00CB01B0
    operator HANDLE() const { return m_hEvent; }
};
class CRefTime {                                 // reftime.h: exactly one REFERENCE_TIME, no virtuals
public:
    REFERENCE_TIME m_time;
    inline CRefTime() { m_time = 0; }
    inline CRefTime(REFERENCE_TIME rt) { m_time = rt; }
    inline operator REFERENCE_TIME() const { return m_time; }
    inline CRefTime& operator=(const CRefTime& rt) { m_time = rt.m_time; return *this; }
    inline CRefTime& operator=(const LONGLONG ll) { m_time = ll; return *this; }
    inline CRefTime& operator+=(const CRefTime& rt) { return (*this = *this + rt); }
};
class CBaseList {                                // wxlist.h
public:
    class CNode {
        CNode* m_pPrev;
        CNode* m_pNext;
        void* m_pObject;
    };
    class CNodeCache {
        int m_iCacheSize;
        int m_iUsed;
        CNode* m_pHead;
    };
protected:
    CNode* m_pFirst;                             // +0x00
    CNode* m_pLast;                              // +0x04
    long m_Count;                                // +0x08
private:
    CNodeCache m_Cache;                          // +0x0c
public:
    CBaseList(const unsigned short* pName);      // 0x00CB12E0
    ~CBaseList();                                // 0x00CB1890
    void* GetNextI(POSITION& rp) const;          // 0x00CB1380
    void* GetI(POSITION p) const;                // 0x00CB13A0
    void RemoveAll();                            // 0x00CB1340
};
template <class OBJECT> class CGenericList : public CBaseList {   // wxlist.h
public:
    CGenericList(const unsigned short* pName) : CBaseList(pName) { }
    POSITION GetHeadPosition() const { return (POSITION)m_pFirst; }
    int GetCount() const { return m_Count; }
    OBJECT* GetNext(POSITION& rp) const { return (OBJECT*)GetNextI(rp); }
    OBJECT* Get(POSITION pos) const { return (OBJECT*)GetI(pos); }
};
// combase.h CBaseObject (no data in release; counts live objects), in the namespace the landed strmbase rows use so
// its destructor's symbol stays apart from the game's own CBaseObject (0x0099A380 / 0x0099A430) in the relocation bindings.
namespace N_00cacaf0 { class CBaseObject {
public:
    CBaseObject(const unsigned short* pName);
    ~CBaseObject();                              // 0x00CACAF0
}; } using N_00cacaf0::CBaseObject;
class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CBaseObject {   // combase.h (AM_NOVTABLE)
    IUnknown* const m_pUnknown;                  // +0x04
protected:
    long m_cRef;                                 // +0x08
public:
    CUnknown(const unsigned short* pName, IUnknown* pUnk);
    virtual ~CUnknown() { }
};
struct IReferenceClock : IUnknown {             // strmif.h
    virtual long __stdcall GetTime(REFERENCE_TIME* pTime) = 0;                  // 3 (+0x0c)
    virtual long __stdcall AdviseTime(REFERENCE_TIME baseTime, REFERENCE_TIME streamTime, HANDLE hEvent, DWORD_PTR* pdwAdviseCookie) = 0;
    virtual long __stdcall AdvisePeriodic(REFERENCE_TIME startTime, REFERENCE_TIME periodTime, HANDLE hSemaphore, DWORD_PTR* pdwAdviseCookie) = 0;
    virtual long __stdcall Unadvise(DWORD_PTR dwAdviseCookie) = 0;
};
struct IDeferredCommand : IUnknown {             // control.h
    virtual long __stdcall Cancel() = 0;                         // 3
    virtual long __stdcall Confidence(long* pConfidence) = 0;    // 4
    virtual long __stdcall Postpone(REFTIME newtime) = 0;        // 5
    virtual long __stdcall GetHResult(long* phrResult) = 0;      // 6
};
class CCmdQueue;
class CDeferredCommand : public CUnknown, public IDeferredCommand {   // ctlutil.h: IDeferredCommand +0x0c
public:
    // DECLARE_IUNKNOWN
    long __stdcall QueryInterface(const GUID& riid, void** ppv);
    unsigned long __stdcall AddRef();
    unsigned long __stdcall Release();
    int IsStreamTime() { return m_bStream; }
    CRefTime GetTime() { return m_time; }
protected:
    CCmdQueue* m_pQueue;                         // +0x10
    IUnknown* m_pUnk;                            // +0x14
    REFERENCE_TIME m_time;                       // +0x18
    GUID* m_iid;                                 // +0x20
    long m_dispidMethod;                         // +0x24
    short m_wFlags;                              // +0x28
    VARIANT* m_pvarResult;                       // +0x2c
    int m_bStream;                               // +0x30
};
class __declspec(novtable) CCmdQueue {           // ctlutil.h
public:
    CCmdQueue();
    virtual ~CCmdQueue();
    virtual long New(CDeferredCommand** ppCmd, IUnknown* pUnk, REFTIME time, GUID* iid, long dispidMethod, short wFlags,
                     long cArgs, VARIANT* pDispParams, VARIANT* pvarResult, short* puArgErr, int bStream);
    virtual long Insert(CDeferredCommand* pCmd);
    virtual long Remove(CDeferredCommand* pCmd);
    virtual long GetDueCommand(CDeferredCommand** ppCmd, long msTimeout);
    virtual long GetCommandDueFor(REFERENCE_TIME tStream, CDeferredCommand** ppCmd);
    int CheckTime(CRefTime time, int bStream) {  // ctlutil.h: defined in the class (out-of-line copy 0x00CAE170; inlined here)
        // if no clock, nothing is due!
        if (!m_pClock) {
            return 0;
        }
        // stream time
        if (bStream) {
            // not valid if not running
            if (!m_bRunning) {
                return 0;
            }
            // add on known stream time offset to get presentation time
            time += m_StreamTimeOffset;
        }
        CRefTime Now;
        m_pClock->GetTime((REFERENCE_TIME*)&Now);
        return (time <= Now);
    }
    virtual long SetSyncSource(IReferenceClock* pClock);
    virtual long Run(REFERENCE_TIME tStreamTimeOffset);
    virtual long EndRun();
protected:
    CCritSec m_Lock;                             // +0x08
    CGenericList<CDeferredCommand> m_listPresentation;   // +0x20
    CGenericList<CDeferredCommand> m_listStream;         // +0x38
    CAMEvent m_evDue;                            // +0x50
    void SetTimeAdvise(void);
    DWORD_PTR m_dwAdvise;                        // +0x54
    CRefTime m_tCurrentAdvise;                   // +0x58
    IReferenceClock* m_pClock;                   // +0x60
    int m_bRunning;                              // +0x64
    CRefTime m_StreamTimeOffset;                 // +0x68
};


long CCmdQueue::GetCommandDueFor(REFERENCE_TIME rtStream, CDeferredCommand** ppCmd)
{
    CAutoLock lock(&m_Lock);

    CRefTime tStream(rtStream);

    // find earliest stream and presentation time commands
    CDeferredCommand* pStream = 0;
    if (m_listStream.GetCount() > 0) {
        POSITION pos = m_listStream.GetHeadPosition();
        pStream = m_listStream.Get(pos);
    }
    CDeferredCommand* pPresent = 0;
    if (m_listPresentation.GetCount() > 0) {
        POSITION pos = m_listPresentation.GetHeadPosition();
        pPresent = m_listPresentation.Get(pos);
    }

    // is there a presentation time that has passed already
    if (pPresent && CheckTime(pPresent->GetTime(), 0)) {
        pPresent->AddRef();
        *ppCmd = pPresent;
        return S_OK;
    }

    // is there a stream time command due before this stream time
    if (pStream && (pStream->GetTime() <= tStream)) {
        pPresent->AddRef();                      // sic: ctlutil.cpp AddRefs the presentation command here (retail 0x00CAFB03)
        *ppCmd = pStream;
        return S_OK;
    }

    // if we are running, we can map presentation times to
    // stream time. In this case, is there a presentation time command
    // that will be due before this stream time is presented?
    if (m_bRunning && pPresent) {

        // this stream time will appear at...
        tStream += m_StreamTimeOffset;

        // due before that?
        if (pPresent->GetTime() <= tStream) {
            *ppCmd = pPresent;
            return S_OK;
        }
    }

    // no commands due yet
    return VFW_E_NOT_FOUND;
}