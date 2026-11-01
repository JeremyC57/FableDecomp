// CDeferredCommand::CDeferredCommand(CCmdQueue* pQ, IUnknown* pUnk, HRESULT* phr, IUnknown* pUnkExecutor, REFTIME time, GUID* iid, long dispidMethod, short wFlags, long nArgs, VARIANT* pDispParams, VARIANT* pvarResult, short* puArgErr, BOOL bStream) -- DirectShow ctlutil.cpp constructor
// Leaf: ??0CDeferredCommand@@QAE@PAVCCmdQueue@@PAUIUnknown@@PAJ1NPAU_GUID@@JFJPAUtagVARIANT@@3PAFH@Z
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
    CUnknown(const char* pName, IUnknown* pUnk);   // 0x00CACC80
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
class CBaseList {                                 // wxlist.h (release build: no CBaseObject base), 0x18 bytes
public:
    class CNode {
        CNode* m_pPrev;
        CNode* m_pNext;
        void* m_pObject;
    public:
        CNode* Prev() const { return m_pPrev; }
        CNode* Next() const { return m_pNext; }
        void* GetData() const { return m_pObject; }
    };
    class CNodeCache {
        int m_iCacheSize;
        int m_iUsed;
        CNode* m_pHead;
    };
    typedef CNode* POSITION;
protected:
    CNode* m_pFirst;                              // +0x00
    CNode* m_pLast;                               // +0x04
    LONG m_Count;                                 // +0x08
private:
    CNodeCache m_Cache;                           // +0x0C
public:
    ~CBaseList();
    POSITION GetHeadPositionI() const;
    void* GetI(POSITION p) const;
    int GetCountI() const { return m_Count; }
};
template <class OBJECT> class CGenericList : public CBaseList {
public:
    ~CGenericList() {}
    POSITION GetHeadPosition() const { return (POSITION)m_pFirst; }
    int GetCount() const { return m_Count; }
    OBJECT* Get(POSITION pos) const { return (OBJECT*)GetI(pos); }
};
// ---- oaidl.h / ctlutil.h model: automation types, CBaseDispatch, CDispParams ----
typedef unsigned int UINT;
typedef unsigned short WORD;
typedef DWORD LCID;
typedef WCHAR OLECHAR;
typedef OLECHAR* BSTR;
typedef OLECHAR* LPOLESTR;
typedef long DISPID;
typedef double REFTIME;
struct IDispatch;
enum VARENUM { VT_EMPTY = 0, VT_I4 = 3, VT_BSTR = 8, VT_DISPATCH = 9, VT_UNKNOWN = 13 };
struct tagVARIANT {                               // oaidl.h, 16 bytes (typedef tagVARIANT VARIANT)
    unsigned short vt;                            // +0x00
    WORD wReserved1, wReserved2, wReserved3;
    union {                                       // +0x08
        LONG lVal;
        double dblVal;
        BSTR bstrVal;
        IUnknown* punkVal;
        IDispatch* pdispVal;
    };
};
typedef tagVARIANT VARIANT;
struct tagDISPPARAMS {                            // oaidl.h (typedef tagDISPPARAMS DISPPARAMS)
    VARIANT* rgvarg;                              // +0x00
    DISPID* rgdispidNamedArgs;                    // +0x04
    UINT cArgs;                                   // +0x08
    UINT cNamedArgs;                              // +0x0C
};
typedef tagDISPPARAMS DISPPARAMS;
struct EXCEPINFO;
struct __declspec(novtable) ITypeInfo : IUnknown {
    virtual HRESULT __stdcall GetTypeAttr(void** ppTypeAttr) = 0;                               // 3
};
struct __declspec(novtable) ITypeLib : IUnknown {
    virtual UINT __stdcall GetTypeInfoCount() = 0;                                              // 3
    virtual HRESULT __stdcall GetTypeInfo(UINT index, ITypeInfo** ppTInfo) = 0;                 // 4
    virtual HRESULT __stdcall GetTypeInfoType(UINT index, int* pTKind) = 0;                     // 5
    virtual HRESULT __stdcall GetTypeInfoOfGuid(const GUID& guid, ITypeInfo** ppTinfo) = 0;     // 6 (+0x18)
};
struct __declspec(novtable) IDispatch : IUnknown {
    virtual HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo) = 0;                              // 3
    virtual HRESULT __stdcall GetTypeInfo(UINT itinfo, LCID lcid, ITypeInfo** pptinfo) = 0;    // 4
    virtual HRESULT __stdcall GetIDsOfNames(const IID& riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid) = 0;   // 5
    virtual HRESULT __stdcall Invoke(DISPID dispidMember, const IID& riid, LCID lcid, WORD wFlags, DISPPARAMS* pdispparams,
                                     VARIANT* pvarResult, EXCEPINFO* pexcepinfo, UINT* puArgErr) = 0;   // 6
};
class CBaseDispatch {                             // ctlutil.h
    ITypeInfo* m_pti;                             // +0x00
public:
    CBaseDispatch() : m_pti(0) {}
    ~CBaseDispatch();
    HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo);
    HRESULT __stdcall GetTypeInfo(const IID& riid, UINT itinfo, LCID lcid, ITypeInfo** pptinfo);
    HRESULT __stdcall GetIDsOfNames(const IID& riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid);
};
class CDispParams : public DISPPARAMS {           // ctlutil.h
public:
    CDispParams(UINT nArgs, VARIANT* pArgs, HRESULT* phr = 0);
    ~CDispParams();
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
// ---- ctlutil.h model: CDeferredCommand, CCmdQueue (DX9 SDK) ----
const LONGLONG TimeZero = 0;
struct __declspec(novtable) IDeferredCommand : IUnknown {   // control.h
    virtual HRESULT __stdcall Cancel() = 0;                                                     // 3
    virtual HRESULT __stdcall Confidence(LONG* pConfidence) = 0;                                // 4
    virtual HRESULT __stdcall Postpone(REFTIME newtime) = 0;                                    // 5
    virtual HRESULT __stdcall GetHResult(HRESULT* phrResult) = 0;                               // 6
};
class CCmdQueue;
class CDeferredCommand : public CUnknown, public IDeferredCommand {   // IDeferredCommand +0x0C
public:
    CDeferredCommand(CCmdQueue* pQ, IUnknown* pUnk, HRESULT* phr, IUnknown* pUnkExecutor, REFTIME time, GUID* iid,
                     long dispidMethod, short wFlags, long cArgs, VARIANT* pDispParams, VARIANT* pvarResult,
                     short* puArgErr, BOOL bStream);
    DECLARE_IUNKNOWN
    HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv);
    HRESULT __stdcall Cancel();
    HRESULT __stdcall Confidence(LONG* pConfidence);
    HRESULT __stdcall Postpone(REFTIME newtime);
    HRESULT __stdcall GetHResult(HRESULT* phrResult);
    HRESULT Invoke();
    BOOL IsStreamTime() { return m_bStream; }
    const IID& GetIID() { return *m_iid; }
    CRefTime GetTime() { return m_time; }
protected:
    CCmdQueue* m_pQueue;                          // +0x10
    IUnknown* m_pUnk;                             // +0x14
    REFERENCE_TIME m_time;                        // +0x18
    GUID* m_iid;                                  // +0x20
    long m_dispidMethod;                          // +0x24
    short m_wFlags;                               // +0x28
    VARIANT* m_pvarResult;                        // +0x2C
    BOOL m_bStream;                               // +0x30
    CDispParams m_DispParams;                     // +0x34
    DISPID m_DispId;                              // +0x44
    CBaseDispatch m_Dispatch;                     // +0x48
    HRESULT m_hrResult;                           // +0x4C
};
class __declspec(novtable) CCmdQueue {            // ctlutil.h; the vfptr is padded to 8 (the class holds LONGLONGs)
public:
    CCmdQueue();                                  // 0x00CAF500
    virtual ~CCmdQueue();                         // 0x00CAF580
    virtual HRESULT New(CDeferredCommand** ppCmd, IUnknown* pUnk, REFTIME time, GUID* iid, long dispidMethod, short wFlags,
                        long cArgs, VARIANT* pDispParams, VARIANT* pvarResult, short* puArgErr, BOOL bStream);
    virtual HRESULT Insert(CDeferredCommand* pCmd);
    virtual HRESULT Remove(CDeferredCommand* pCmd);
    virtual HRESULT SetSyncSource(IReferenceClock* pClock);
    virtual HRESULT Run(REFERENCE_TIME tStreamTimeOffset);
    virtual HRESULT EndRun();
    virtual HRESULT GetDueCommand(CDeferredCommand** ppCmd, long msTimeout);
    HANDLE GetDueHandle() { return HANDLE(m_evDue); }
    virtual HRESULT GetCommandDueFor(REFERENCE_TIME tStream, CDeferredCommand** ppCmd);
    BOOL CheckTime(CRefTime time, BOOL bStream);  // inline in ctlutil.h; retail keeps one out-of-line copy (0x00CAE170)
protected:
    CCritSec m_Lock;                              // +0x08
    CGenericList<CDeferredCommand> m_listPresentation;   // +0x20
    CGenericList<CDeferredCommand> m_listStream;  // +0x38
    CAMEvent m_evDue;                             // +0x50
    void SetTimeAdvise(void);                     // 0x00CAF750
    DWORD_PTR m_dwAdvise;                         // +0x54
    CRefTime m_tCurrentAdvise;                    // +0x58
    IReferenceClock* m_pClock;                    // +0x60
    BOOL m_bRunning;                              // +0x64
    CRefTime m_StreamTimeOffset;                  // +0x68
};

#define NOERROR 0L
#define S_OK 0L
#define S_FALSE 1L
#define E_POINTER ((HRESULT)0x80004003L)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define CheckPointer(p, ret) { if ((p) == 0) return (ret); }
#define ValidateReadPtr(p, cb) 0
#define ValidateReadWritePtr(p, cb) 0
#define ASSERT(x)
#define UNREFERENCED_PARAMETER(P) (P)
#define UNITS 10000000
class COARefTime : public CRefTime {             // ctlutil.h (this SDK drop: no +0.5 rounding)
public:
    COARefTime() {}
    COARefTime(REFTIME t) : CRefTime((LONGLONG)(t * UNITS)) {}
};

struct HINSTANCE__ { int unused; };
typedef HINSTANCE__* HINSTANCE;
typedef int (__stdcall* FARPROC)();
HINSTANCE LoadOLEAut32();                                                                  // 0x00CACB20 (combase.cpp)
extern "C" __declspec(dllimport) DWORD __stdcall GetLastError();                           // IAT 0x0143FD58
extern "C" __declspec(dllimport) FARPROC __stdcall GetProcAddress(HINSTANCE hModule, const char* lpProcName);   // IAT 0x0143FE44
extern "C" const GUID LIBID_QuartzTypeLib;                                                 // 0x012AA0B4

typedef HRESULT (__stdcall *LPLOADTYPELIB)(
                            const OLECHAR *szFile,
                            ITypeLib ** pptlib);

typedef HRESULT (__stdcall *LPLOADREGTYPELIB)(const GUID& rguid,
                            WORD wVerMajor,
                            WORD wVerMinor,
                            LCID lcid,
                            ITypeLib ** pptlib);

// attempt to find our type library

HRESULT __stdcall
CBaseDispatch::GetTypeInfo(
  const IID& riid,
  UINT itinfo,
  LCID lcid,
  ITypeInfo ** pptinfo)
{
    if (pptinfo == 0) return (HRESULT)0x80004003L;   // CheckPointer(pptinfo, E_POINTER)
    HRESULT hr;

    *pptinfo = 0;

    // we only support one type element
    if (0 != itinfo) {
        return (HRESULT)0x8002802BL;   // TYPE_E_ELEMENTNOTFOUND
    }

    if (0 == pptinfo) {
        return (HRESULT)0x80004003L;   // E_POINTER
    }

    // always look for neutral
    if (0 == m_pti) {

        LPLOADTYPELIB       lpfnLoadTypeLib;
        LPLOADREGTYPELIB    lpfnLoadRegTypeLib;
        ITypeLib            *ptlib;
        HINSTANCE           hInst;

        static const char  szTypeLib[]    = "LoadTypeLib";
        static const char  szRegTypeLib[] = "LoadRegTypeLib";
        static const WCHAR szControl[]    = L"control.tlb";

        //
        // Try to get the Ole32Aut.dll module handle.
        //

        hInst = LoadOLEAut32();
        if (hInst == 0) {
            DWORD dwError = GetLastError();
            return (HRESULT)(0x80070000UL | dwError);   // AmHresultFromWin32
        }
        lpfnLoadRegTypeLib = (LPLOADREGTYPELIB)GetProcAddress(hInst,
                                                              szRegTypeLib);
        if (lpfnLoadRegTypeLib == 0) {
            DWORD dwError = GetLastError();
            return (HRESULT)(0x80070000UL | dwError);   // AmHresultFromWin32
        }

        hr = (*lpfnLoadRegTypeLib)(LIBID_QuartzTypeLib, 1, 0, // version 1.0
                                   lcid, &ptlib);

        if (hr < 0) {

            // attempt to load directly - this will fill the
            // registry in if it finds it

            lpfnLoadTypeLib = (LPLOADTYPELIB)GetProcAddress(hInst, szTypeLib);
            if (lpfnLoadTypeLib == 0) {
                DWORD dwError = GetLastError();
                return (HRESULT)(0x80070000UL | dwError);   // AmHresultFromWin32
            }

            hr = (*lpfnLoadTypeLib)(szControl, &ptlib);
            if (hr < 0) {
                return hr;
            }
        }

        hr = ptlib->GetTypeInfoOfGuid(
                    riid,
                    &m_pti);

        ptlib->Release();

        if (hr < 0) {
            return hr;
        }
    }

    *pptinfo = m_pti;
    m_pti->AddRef();
    return 0;
}
// the CDeferredCommand constructor (ctlutil.cpp)
CDeferredCommand::CDeferredCommand(
    CCmdQueue * pQ,
    IUnknown*   pUnk,
    HRESULT *   phr,
    IUnknown*   pUnkExecutor,
    REFTIME     time,
    GUID*       iid,
    long        dispidMethod,
    short       wFlags,
    long        nArgs,
    VARIANT*    pDispParams,
    VARIANT*    pvarResult,
    short*      puArgErr,
    BOOL        bStream
    ) :
        CUnknown((const char*)0, pUnk),             // NAME("DeferredCommand") is NULL in a release build
        m_pQueue(pQ),
        m_pUnk(pUnkExecutor),
        m_iid(iid),
        m_dispidMethod(dispidMethod),
        m_wFlags(wFlags),
        m_DispParams(nArgs, pDispParams, phr),
        m_pvarResult(pvarResult),
        m_bStream(bStream),
        m_hrResult((HRESULT)0x80004004L)            // E_ABORT

{
    // convert REFTIME to REFERENCE_TIME
    COARefTime convertor(time);
    m_time = convertor;

    // no check of time validity - it's ok to queue a command that's
    // already late

    // check iid is supportable on pUnk by QueryInterface for it
    IUnknown * pInterface;
    HRESULT hr = m_pUnk->QueryInterface(GetIID(), (void**) &pInterface);
    if (FAILED(hr)) {
        *phr = hr;
        return;
    }
    pInterface->Release();


    // !!! check dispidMethod and param/return types using typelib
    ITypeInfo *pti;
    hr = m_Dispatch.GetTypeInfo(*iid, 0, 0, &pti);
    if (FAILED(hr)) {
        *phr = hr;
        return;
    }
    // !!! some sort of ITypeInfo validity check here
    pti->Release();


    // Fix up the dispid for put and get
    if (wFlags == 4) {                              // DISPATCH_PROPERTYPUT
        m_DispParams.cNamedArgs = 1;
        m_DispId = -3;                              // DISPID_PROPERTYPUT
        m_DispParams.rgdispidNamedArgs = &m_DispId;
    }

    // all checks ok - add to queue
    hr = pQ->Insert(this);
    if (FAILED(hr)) {
        *phr = hr;
    }
}
