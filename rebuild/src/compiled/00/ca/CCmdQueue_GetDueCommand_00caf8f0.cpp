// CCmdQueue::GetDueCommand(CDeferredCommand**, long) -- DirectShow BaseClasses ctlutil.cpp: loop: under the lock pick the earliest command (presentation list head, or the stream list head offset by m_StreamTimeOffset while running), AddRef and return it when CheckTime says it is due, else wait on m_evDue (E_ABORT on timeout)
// Leaf: ?GetDueCommand@CCmdQueue@@UAEJPAPAVCDeferredCommand@@J@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAF8F0, 276 bytes (+ int3 padding), thiscall `ret 8`; /EHsc frame (the CAutoLock is EH state
//   0); CBaseList::GetI 0x00CB13A0 fetches each list head's object, CCmdQueue::CheckTime 0x00CAE170 is called (also from
//   GetCommandDueFor 0x00CAED13), the AddRef is the IDeferredCommand sub-object's (+0x0c, slot 1).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 449 "0xCAF8F0 file 'PeekNextConnectedTrackNode' = CCmdQueue::GetDueCommand"):
//   the landed NativeDeferredCommandQueue row already reproduces ctlutil.cpp's body; this names it. CDeferredCommand:
//   m_time +0x18, m_bStream +0x30 (after CUnknown, the IDeferredCommand vptr +0x0c, m_pQueue +0x10, m_pUnk +0x14).
//   ctlutil.h's CCmdQueue layout: the vfptr slot is padded to the class's 8-byte alignment (CRefTime members), so m_Lock is
//   +0x08 (InitializeCriticalSection), m_listPresentation +0x20 and m_listStream +0x38 (CBaseList ctor 0x00CB12E0, NAME()
//   = NULL in release), m_evDue +0x50 (CAMEvent(TRUE) 0x00CB0190, manual reset), m_dwAdvise +0x54, m_tCurrentAdvise +0x58,
//   m_pClock +0x60, m_bRunning +0x64, m_StreamTimeOffset +0x68 (0x70 bytes). Neither the constructor nor the landed
//   destructor 0x00CAF580 stores a vptr and nothing references these three functions (strmbase objects linked whole), so the
//   model is novtable, as in the landed CCmdQueue::~CCmdQueue row.
// Label: the landed name is a propagated manifest label / by-address placeholder; the real name is the strmbase member.
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
struct IReferenceClock;
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
    int CheckTime(CRefTime time, int bStream);   // 0x00CAE170 (ctlutil.h defines it in the class; retail calls one copy)
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

long CCmdQueue::GetDueCommand(CDeferredCommand** ppCmd, long msTimeout)
{
    // loop until we timeout or find a due command
    for (;;) {

        {
            CAutoLock lock(&m_Lock);


            // find the earliest command

            // check the presentation time and the
            // stream time list to find the earliest

            CDeferredCommand* pCmd = 0;

            if (m_listPresentation.GetCount() > 0) {
                POSITION pos = m_listPresentation.GetHeadPosition();
                pCmd = m_listPresentation.Get(pos);
            }

            if (m_bRunning && (m_listStream.GetCount() > 0)) {
                POSITION pos = m_listStream.GetHeadPosition();
                CDeferredCommand* pStrm = m_listStream.Get(pos);

                CRefTime t = pStrm->GetTime() + m_StreamTimeOffset;
                if (!pCmd || (t < pCmd->GetTime())) {
                    pCmd = pStrm;
                }
            }

            //  if we have found one, is it due?
            if (pCmd) {
                if (CheckTime(pCmd->GetTime(), pCmd->IsStreamTime())) {

                    // yes it's due - addref it
                    pCmd->AddRef();
                    *ppCmd = pCmd;
                    return S_OK;
                }
            }
        }

        // block until the advise is signalled
        if (WaitForSingleObject(m_evDue, msTimeout) != WAIT_OBJECT_0) {
            return E_ABORT;
        }
    }
}
