// CMemAllocator::Alloc() -- DirectShow BaseClasses (amfilter.cpp, DX9 SDK era): under the allocator lock, check SetProperties was called (CBaseAllocator::Alloc: VFW_E_SIZENOTSET for a zero count, size or alignment; nothing to do while unchanged), free the old block (ReallyFree), round size + prefix up to the alignment, VirtualAlloc one committed block for all buffers (E_OUTOFMEMORY on failure), then carve it into m_lCount CMediaSample objects pushed on the free list, and clear m_bChanged
// Leaf: ?Alloc@CMemAllocator@@MAEJXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAC640, 414 bytes, thiscall plain `ret` (the protected virtual `HRESULT Alloc(void)`,
//   not an STDMETHOD), EH-framed (the CAutoLock, state 0).
// Identity: the body is strmbase's CMemAllocator::Alloc verbatim -- CBaseAllocator::Alloc inlined (m_lCount +0x38 / m_lSize
//   +0x40 / m_lAlignment +0x44 tests, VFW_E_SIZENOTSET 0x80040212, m_bChanged +0x4C), `if (m_pBuffer) ReallyFree();` (+0x60,
//   the landed CMemAllocator::ReallyFree 0x00CAA3C0), the alignment rounding, VirtualAlloc(NULL, m_lCount * lAlignedSize,
//   MEM_COMMIT, PAGE_READWRITE), and the CMediaSample constructor inlined into the 0x48-byte `new` (vtable 0x012BF6E0;
//   m_pBuffer = pNext + m_lPrefix, m_lActual = m_cbBuffer = m_lSize, m_pAllocator = this) with the inline CSampleList::Add.
//   It is the older DX9 SDK text: no size / prefix / overflow checks, and the CMediaSample constructor has no `length < 0`
//   test. Model: the landed CMemAllocator::SetProperties 0x00CAA290 strmbase model. The manifest label "sub_00cac640".
// Callees: EnterCriticalSection [0x0143FE0C] / LeaveCriticalSection [0x0143FE10] (CAutoLock), CMemAllocator::ReallyFree
//   0x00CAA3C0, VirtualAlloc [0x0143FD44], operator new 0x00BFEA1A.
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
    CMediaSample(const WCHAR* pName, CBaseAllocator* pAllocator, HRESULT* phr, BYTE* pBuffer, LONG length);
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
#define TRUE 1
#define NOERROR 0
#define FALSE 0
#define S_OK                      ((HRESULT)0x00000000L)
#define S_FALSE                   ((HRESULT)0x00000001L)
#define E_OUTOFMEMORY             ((HRESULT)0x8007000EL)
#define VFW_E_SIZENOTSET          ((HRESULT)0x80040212L)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define ASSERT(x)
#define NAME(x) 0
#define AM_STREAM_MEDIA 0
#define MEM_COMMIT     0x1000
#define PAGE_READWRITE 0x04
typedef BYTE* PBYTE;
typedef BYTE* LPBYTE;
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void* lpAddress, unsigned long dwSize, DWORD flAllocationType, DWORD flProtect);   // IAT 0x0143FD44
void* __cdecl operator new(unsigned int size);   // 0x00BFEA1A

class CMemAllocator : public CBaseAllocator {    // amfilter.h
protected:
    BYTE* m_pBuffer;                              // +0x60 combined memory for all buffers
    void Free();
    void ReallyFree();                            // 0x00CAA3C0
    HRESULT Alloc();                              // 0x00CAC640
public:
    static CUnknown* CreateInstance(IUnknown*, HRESULT*);   // 0x00CAC9D0
    HRESULT __stdcall SetProperties(ALLOCATOR_PROPERTIES* pRequest, ALLOCATOR_PROPERTIES* pActual);   // 0x00CAA290
    CMemAllocator(WCHAR*, IUnknown*, HRESULT*);
    ~CMemAllocator();
};


// CBaseAllocator::Alloc, inlined from the same TU (amfilter.cpp)
inline HRESULT CBaseAllocator::Alloc(void)
{
    /* Error if he hasn't set the size yet */
    if (m_lCount <= 0 || m_lSize <= 0 || m_lAlignment <= 0) {
        return VFW_E_SIZENOTSET;
    }

    /* should never get here while buffers outstanding */
    ASSERT(m_lFree.GetCount() == m_lAllocated);

    /* If the requirements haven't changed then don't reallocate */
    if (m_bChanged == FALSE) {
        return S_FALSE;
    }

    return NOERROR;
}

// CMediaSample constructor (DX9 SDK amfilter.cpp), inlined into the `new` below
inline CMediaSample::CMediaSample(const WCHAR* pName, CBaseAllocator* pAllocator, HRESULT* phr, BYTE* pBuffer, LONG length) :
    m_pBuffer(pBuffer),             // Initialise the buffer
    m_cbBuffer(length),             // And it's length
    m_lActual(length),              // By default, actual = length
    m_pMediaType(0),                // No media type change
    m_dwFlags(0),                   // Nothing set
    m_cRef(0),                      // 0 ref count
    m_dwTypeSpecificFlags(0),       // Type specific flags
    m_dwStreamId(AM_STREAM_MEDIA),  // Stream id
    m_pAllocator(pAllocator)        // Allocator
{
    /* We must have an owner and it must also be derived from class
       CBaseAllocator BUT we do not hold a reference count on it */

    ASSERT(pAllocator);
}

HRESULT CMemAllocator::Alloc(void)
{
    CAutoLock lck(this);

    /* Check he has called SetProperties */
    HRESULT hr = CBaseAllocator::Alloc();
    if (FAILED(hr)) {
        return hr;
    }

    /* If the requirements haven't changed then don't reallocate */
    if (hr == S_FALSE) {
        ASSERT(m_pBuffer);
        return NOERROR;
    }
    ASSERT(hr == S_OK); // we use this fact in the loop below

    /* Free the old resources */
    if (m_pBuffer) {
        ReallyFree();
    }

    /* Compute the aligned size */
    LONG lAlignedSize = m_lSize + m_lPrefix;
    if (m_lAlignment > 1) {
        LONG lRemainder = lAlignedSize % m_lAlignment;
        if (lRemainder != 0) {
            lAlignedSize += (m_lAlignment - lRemainder);
        }
    }

    /* Create the contiguous memory block for the samples
       making sure it's properly aligned (64K should be enough!)
    */
    ASSERT(lAlignedSize % m_lAlignment == 0);

    m_pBuffer = (PBYTE)VirtualAlloc(0,
                    m_lCount * lAlignedSize,
                    MEM_COMMIT,
                    PAGE_READWRITE);

    if (m_pBuffer == 0) {
        return E_OUTOFMEMORY;
    }

    LPBYTE pNext = m_pBuffer;
    CMediaSample *pSample;

    ASSERT(m_lAllocated == 0);

    // Create the new samples - we have allocated m_lSize bytes for each sample
    // plus m_lPrefix bytes per sample as a prefix. We set the pointer to
    // the memory after the prefix - so that GetPointer() will return a pointer
    // to m_lSize bytes.
    for (; m_lAllocated < m_lCount; m_lAllocated++, pNext += lAlignedSize) {

        pSample = new CMediaSample(
                            NAME("Default memory media sample"),
                this,
                            &hr,
                            pNext + m_lPrefix,      // GetPointer() value
                            m_lSize);               // not including prefix

            ASSERT(SUCCEEDED(hr));
        if (pSample == 0) {
            return E_OUTOFMEMORY;
        }

        // This CANNOT fail
        m_lFree.Add(pSample);
    }

    m_bChanged = FALSE;
    return NOERROR;
}