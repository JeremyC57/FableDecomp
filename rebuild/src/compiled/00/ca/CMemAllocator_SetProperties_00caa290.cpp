// CMemAllocator::SetProperties(ALLOCATOR_PROPERTIES* pRequest, ALLOCATOR_PROPERTIES* pActual) -- DirectShow BaseClasses (amfilter.cpp): under the allocator lock, zero pActual, check the requested alignment against the system allocation granularity, refuse while committed or with buffers outstanding, then round the buffer size up to the alignment (prefix included) and record and report the agreed count, size, alignment and prefix
// Leaf: ?SetProperties@CMemAllocator@@UAGJPAUALLOCATOR_PROPERTIES@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAA290, 281 bytes, STDMETHODIMP: `this` on the stack is the IMemAllocatorCallbackTemp
//   subobject (+0x0C), `ret 0Ch`; the CCritSec base at +0x10 (`lea ebx,[esi+4]` behind the null test of the full object).
// Identity: slot 3 (SetProperties) of the IMemAllocatorCallbackTemp vtable 0x012BF810 that CMemAllocator::CreateInstance
//   0x00CAC9D0 stores (its other slots are the landed CBaseAllocator::GetProperties 0x00CA9EC0, Commit, Decommit, GetBuffer,
//   ReleaseBuffer, SetNotify, GetFreeCount). The body is CMemAllocator's override, not CBaseAllocator's: GetSystemInfo and
//   the `cbAlign == 0 || (dwAllocationGranularity & (cbAlign - 1))` check, only pActual null-checked (strmbase has no RTTI or
//   PDB; the names are the BaseClasses source's). The manifest label "AllocateAndValidateMemoryRange" is descriptive.
// Callees: EnterCriticalSection [0x0143FE0C], GetSystemInfo [0x0143FE4C], LeaveCriticalSection [0x0143FE10] (CAutoLock).
// Results: E_POINTER 0x80004003, VFW_E_BADALIGN 0x8004020E, VFW_E_ALREADY_COMMITTED 0x8004020F, VFW_E_BUFFERS_OUTSTANDING
//   0x80040210, NOERROR.
// Shape: the DX9-era source verbatim (the DbgLog / ASSERT / ValidateReadWritePtr lines are empty in release);
//   ZeroMemory -> memset intrinsic (four zero dwords through a copy of pActual), SYSTEM_INFO in the 0x24-byte frame,
//   the chained `pActual->x = m_x = ...` stores member first.
// Model: the landed strmbase model of CBaseAllocator::GetProperties 0x00CA9EC0 (object offsets m_lFree.m_nOnList +0x2C,
//   m_lCount +0x38, m_lAllocated +0x3C, m_lSize +0x40, m_lAlignment +0x44, m_lPrefix +0x48, m_bChanged +0x4C,
//   m_bCommitted +0x50), plus CMemAllocator's m_pBuffer +0x60. engine/CBaseObject.h is Fable's own class, not strmbase's.
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
// ---- strmbase amfilter.h model (only what CMemAllocator::SetProperties and its bases need) ----
typedef struct _AMMediaType AM_MEDIA_TYPE;
struct IMediaSample;
struct ALLOCATOR_PROPERTIES { long cBuffers; long cbBuffer; long cbAlign; long cbPrefix; };
struct __declspec(novtable) IMemAllocator : IUnknown {
    virtual HRESULT __stdcall SetProperties(ALLOCATOR_PROPERTIES* pRequest, ALLOCATOR_PROPERTIES* pActual) = 0;   // 3
    virtual HRESULT __stdcall GetProperties(ALLOCATOR_PROPERTIES* pProps) = 0;                                    // 4
    virtual HRESULT __stdcall Commit() = 0;                                                                       // 5
    virtual HRESULT __stdcall Decommit() = 0;                                                                     // 6
    virtual HRESULT __stdcall GetBuffer(IMediaSample** ppBuffer, REFERENCE_TIME* pStartTime, REFERENCE_TIME* pEndTime, DWORD dwFlags) = 0;   // 7
    virtual HRESULT __stdcall ReleaseBuffer(IMediaSample* pBuffer) = 0;                                           // 8
};
struct IMemAllocatorNotifyCallbackTemp;
struct __declspec(novtable) IMemAllocatorCallbackTemp : IMemAllocator {
    virtual HRESULT __stdcall SetNotify(IMemAllocatorNotifyCallbackTemp* pNotify) = 0;   // 9
    virtual HRESULT __stdcall GetFreeCount(LONG* plBuffersFree) = 0;                     // 10
};
struct __declspec(novtable) IMediaSample : IUnknown {
    virtual HRESULT __stdcall GetPointer(BYTE** ppBuffer) = 0;                                   // 3
    virtual long __stdcall GetSize() = 0;                                                        // 4
    virtual HRESULT __stdcall GetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd) = 0; // 5
    virtual HRESULT __stdcall SetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd) = 0; // 6
    virtual HRESULT __stdcall IsSyncPoint() = 0;                                                 // 7
    virtual HRESULT __stdcall SetSyncPoint(BOOL bIsSyncPoint) = 0;                               // 8
    virtual HRESULT __stdcall IsPreroll() = 0;                                                   // 9
    virtual HRESULT __stdcall SetPreroll(BOOL bIsPreroll) = 0;                                   // 10
    virtual long __stdcall GetActualDataLength() = 0;                                            // 11
    virtual HRESULT __stdcall SetActualDataLength(long lLen) = 0;                                // 12
    virtual HRESULT __stdcall GetMediaType(AM_MEDIA_TYPE** ppMediaType) = 0;                     // 13
    virtual HRESULT __stdcall SetMediaType(AM_MEDIA_TYPE* pMediaType) = 0;                       // 14 (+0x38)
    virtual HRESULT __stdcall IsDiscontinuity() = 0;                                             // 15
    virtual HRESULT __stdcall SetDiscontinuity(BOOL bDiscontinuity) = 0;                         // 16
    virtual HRESULT __stdcall GetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd) = 0;        // 17
    virtual HRESULT __stdcall SetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd) = 0;        // 18
};
struct __declspec(novtable) IMediaSample2 : IMediaSample {
    virtual HRESULT __stdcall GetProperties(DWORD cbProperties, BYTE* pbProperties) = 0;         // 19
    virtual HRESULT __stdcall SetProperties(DWORD cbProperties, const BYTE* pbProperties) = 0;   // 20
};
class CBaseAllocator;
class CMediaSample : public IMediaSample2 {      // amfilter.h
protected:
    friend class CBaseAllocator;
    enum { Sample_SyncPoint = 0x01, Sample_Preroll = 0x02, Sample_Discontinuity = 0x04, Sample_TypeChanged = 0x08,
           Sample_TimeValid = 0x10, Sample_MediaTimeValid = 0x20, Sample_TimeDiscontinuity = 0x40, Sample_StopValid = 0x100,
           Sample_ValidFlags = 0x1FF };
    DWORD m_dwFlags;                              // +0x04
    DWORD m_dwTypeSpecificFlags;                  // +0x08
    BYTE* m_pBuffer;                              // +0x0C
    LONG m_lActual;                               // +0x10
    LONG m_cbBuffer;                              // +0x14
    CBaseAllocator* m_pAllocator;                 // +0x18
    CMediaSample* m_pNext;                        // +0x1C
    REFERENCE_TIME m_Start;                       // +0x20
    REFERENCE_TIME m_End;                         // +0x28
    LONGLONG m_MediaStart;                        // +0x30
    LONG m_MediaEnd;                              // +0x38
    AM_MEDIA_TYPE* m_pMediaType;                  // +0x3C
    DWORD m_dwStreamId;                           // +0x40
public:
    LONG m_cRef;                                  // +0x44
    virtual ~CMediaSample();
    HRESULT __stdcall QueryInterface(const IID& riid, void** ppv);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();
    HRESULT SetPointer(BYTE* ptr, LONG cBytes);
    HRESULT __stdcall GetPointer(BYTE** ppBuffer);
    long __stdcall GetSize();
    HRESULT __stdcall GetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd);
    HRESULT __stdcall SetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd);
    HRESULT __stdcall IsSyncPoint();
    HRESULT __stdcall SetSyncPoint(BOOL bIsSyncPoint);
    HRESULT __stdcall IsPreroll();
    HRESULT __stdcall SetPreroll(BOOL bIsPreroll);
    long __stdcall GetActualDataLength();
    HRESULT __stdcall SetActualDataLength(long lActual);
    HRESULT __stdcall GetMediaType(AM_MEDIA_TYPE** ppMediaType);
    HRESULT __stdcall SetMediaType(AM_MEDIA_TYPE* pMediaType);
    HRESULT __stdcall IsDiscontinuity();
    HRESULT __stdcall SetDiscontinuity(BOOL bDiscontinuity);
    HRESULT __stdcall GetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd);
    HRESULT __stdcall SetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd);
    HRESULT __stdcall GetProperties(DWORD cbProperties, BYTE* pbProperties);
    HRESULT __stdcall SetProperties(DWORD cbProperties, const BYTE* pbProperties);
};
class __declspec(novtable) CBaseAllocator : public CUnknown, public IMemAllocatorCallbackTemp, public CCritSec {
    // IMemAllocatorCallbackTemp +0x0C, CCritSec +0x10
    class CSampleList;
    friend class CSampleList;
    static CMediaSample*& NextSample(CMediaSample* pSample) { return pSample->m_pNext; }
    class CSampleList {
    public:
        CSampleList() : m_List(0), m_nOnList(0) {}
        CMediaSample* Head() const { return m_List; }
        CMediaSample* Next(CMediaSample* pSample) const { return CBaseAllocator::NextSample(pSample); }
        int GetCount() const { return m_nOnList; }
        void Add(CMediaSample* pSample) { CBaseAllocator::NextSample(pSample) = m_List; m_List = pSample; m_nOnList++; }
        CMediaSample* RemoveHead() {
            CMediaSample* pSample = m_List;
            if (pSample != 0) { m_List = CBaseAllocator::NextSample(m_List); m_nOnList--; }
            return pSample;
        }
        void Remove(CMediaSample* pSample);
    public:
        CMediaSample* m_List;
        int m_nOnList;
    };
protected:
    CSampleList m_lFree;                          // +0x28
    HANDLE m_hSem;                                // +0x30
    long m_lWaiting;                              // +0x34
    long m_lCount;                                // +0x38
    long m_lAllocated;                            // +0x3C
    long m_lSize;                                 // +0x40
    long m_lAlignment;                            // +0x44
    long m_lPrefix;                               // +0x48
    BOOL m_bChanged;                              // +0x4C
    BOOL m_bCommitted;                            // +0x50
    BOOL m_bDecommitInProgress;                   // +0x54
    IMemAllocatorNotifyCallbackTemp* m_pNotify;   // +0x58
    BOOL m_fEnableReleaseCallback;                // +0x5C
    virtual void Free() = 0;                      // primary slot 4
    virtual HRESULT Alloc();                      // primary slot 5
public:
    virtual ~CBaseAllocator();
    DECLARE_IUNKNOWN
    HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv);
    HRESULT __stdcall SetProperties(ALLOCATOR_PROPERTIES* pRequest, ALLOCATOR_PROPERTIES* pActual);
    HRESULT __stdcall GetProperties(ALLOCATOR_PROPERTIES* pProps);
    HRESULT __stdcall Commit();
    HRESULT __stdcall Decommit();
    HRESULT __stdcall GetBuffer(IMediaSample** ppBuffer, REFERENCE_TIME* pStartTime, REFERENCE_TIME* pEndTime, DWORD dwFlags);
    HRESULT __stdcall ReleaseBuffer(IMediaSample* pBuffer);
    HRESULT __stdcall SetNotify(IMemAllocatorNotifyCallbackTemp* pNotify);
    HRESULT __stdcall GetFreeCount(LONG* plBuffersFree);
    void NotifySample();
    void SetWaiting() { m_lWaiting++; }
};
struct SYSTEM_INFO {                              // winbase.h, 0x24 bytes
    unsigned short wProcessorArchitecture;
    unsigned short wReserved;
    DWORD dwPageSize;
    void* lpMinimumApplicationAddress;
    void* lpMaximumApplicationAddress;
    DWORD dwActiveProcessorMask;
    DWORD dwNumberOfProcessors;
    DWORD dwProcessorType;
    DWORD dwAllocationGranularity;                // +0x1C
    unsigned short wProcessorLevel;
    unsigned short wProcessorRevision;
};
extern "C" __declspec(dllimport) void __stdcall GetSystemInfo(SYSTEM_INFO* lpSystemInfo);   // IAT 0x0143FE4C
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define ZeroMemory(Destination, Length) memset((Destination), 0, (Length))
#define TRUE 1
#define NOERROR 0
#define E_POINTER                 ((HRESULT)0x80004003L)
#define VFW_E_BADALIGN            ((HRESULT)0x8004020EL)
#define VFW_E_ALREADY_COMMITTED   ((HRESULT)0x8004020FL)
#define VFW_E_BUFFERS_OUTSTANDING ((HRESULT)0x80040210L)
#define CheckPointer(p, ret) { if ((p) == 0) return (ret); }

class CMemAllocator : public CBaseAllocator {    // amfilter.h
protected:
    BYTE* m_pBuffer;                              // +0x60 combined memory for all buffers
    void Free();
    void ReallyFree();
    HRESULT Alloc();
public:
    static CUnknown* CreateInstance(IUnknown*, HRESULT*);   // 0x00CAC9D0
    HRESULT __stdcall SetProperties(ALLOCATOR_PROPERTIES* pRequest, ALLOCATOR_PROPERTIES* pActual);
    CMemAllocator(WCHAR*, IUnknown*, HRESULT*);
    ~CMemAllocator();
};

HRESULT __stdcall CMemAllocator::SetProperties(ALLOCATOR_PROPERTIES* pRequest, ALLOCATOR_PROPERTIES* pActual)
{
    CheckPointer(pActual, E_POINTER);
    CAutoLock cObjectLock(this);

    ZeroMemory(pActual, sizeof(ALLOCATOR_PROPERTIES));

    SYSTEM_INFO SysInfo;
    GetSystemInfo(&SysInfo);

    /*  Check the alignment requested */
    if (pRequest->cbAlign == 0 ||
        (SysInfo.dwAllocationGranularity & (pRequest->cbAlign - 1)) != 0) {
        return VFW_E_BADALIGN;
    }

    /* Can't do this if already committed */
    if (m_bCommitted == TRUE) {
        return VFW_E_ALREADY_COMMITTED;
    }

    /* Must be no outstanding buffers */
    if (m_lFree.GetCount() < m_lAllocated) {
        return VFW_E_BUFFERS_OUTSTANDING;
    }

    // round length up to alignment - remember that prefix is included in the alignment
    LONG lSize = pRequest->cbBuffer + pRequest->cbPrefix;
    LONG lRemainder = lSize % pRequest->cbAlign;
    if (lRemainder != 0) {
        lSize = lSize - lRemainder + pRequest->cbAlign;
    }
    pActual->cbBuffer = m_lSize = (lSize - pRequest->cbPrefix);

    pActual->cBuffers = m_lCount = pRequest->cBuffers;
    pActual->cbAlign = m_lAlignment = pRequest->cbAlign;
    pActual->cbPrefix = m_lPrefix = pRequest->cbPrefix;

    m_bChanged = TRUE;
    return NOERROR;
}