// CCmdQueue::~CCmdQueue() -- releases queued DirectShow commands, clears both lists, and tears down the clock/event/critical section
// Retail: 00CAF580, 251 bytes. Correct owner is Microsoft's DirectShow CCmdQueue, not the propagated CEngineLandscapePatch label.
// Provenance: Microsoft DirectShow base classes ctlutil.cpp SHA256 8bb02b6f6940abbb29581992f5fe9c5fe7613eb5973fd6a39c2e8ff3da548b29, lines 2143-2171;
// ctlutil.h SHA256 2f0188b624aeb02f839f54344252ab6a9cff521490335fdb1f63d3e5c15d125c, CCmdQueue declaration lines 768-902;
// wxlist.h SHA256 5d6305a25f44a238b6d358ba2c0c40f469fec08b250c087be2b4ae837f33e634, CBaseList/CGenericList; wxutil.h SHA256 06bcb8348fe9cb5f1f29e59f4a2b7208f42ee569273fa9e5e8f4b9785489b4fa.
// Native projection: adjacent CAF500 ctor proves the critical section begins +8, list objects +20/+38 and event +50; this is an 8-byte retail prefix view.
// Keep its first 8 bytes opaque: no identity is assigned to +4. Clock is +60, running +64, and the two REFERENCE_TIME values occupy +58 and +68.
// CB1380 is the CBaseList GetNextI(position&) helper; CB1340 is RemoveAll; CB1890 is CBaseList::~CBaseList (RemoveAll plus node-cache cleanup);
// CB01B0 is CAMEvent::~CAMEvent; 143FDE4 is DeleteCriticalSection. The body follows the public CCmdQueue destructor order and retains native COM slots.
// Leaf: ??1CCmdQueue
// Flags: /EHsc
#include <stddef.h>
#include "rebuild_abi.h"

#ifdef _MSC_VER
typedef __int64 REFERENCE_TIME;
#define AM_NOVTABLE __declspec(novtable)
#define DS_STDCALL __stdcall
#else
typedef long long REFERENCE_TIME;
#define AM_NOVTABLE
#define DS_STDCALL
#endif

typedef long HRESULT;
typedef unsigned long ULONG;
typedef unsigned long DWORD_PTR;
typedef int BOOL;
typedef void* HANDLE;
typedef const char* LPCTSTR;
struct IUnknown;
typedef IUnknown* LPUNKNOWN;
typedef double REFTIME;
struct GUID;
struct VARIANT;

struct NativeCriticalSection {
    void* DebugInfo;
    long LockCount;
    long RecursionCount;
    void* OwningThread;
    void* LockSemaphore;
    unsigned long SpinCount;
};

#ifndef NATIVE_IMPORT
#ifdef _MSC_VER
#define NATIVE_IMPORT __declspec(dllimport)
#else
#define NATIVE_IMPORT
#endif
#endif
extern "C" {
NATIVE_IMPORT void DS_STDCALL InitializeCriticalSection(NativeCriticalSection*);  // 0x0143FEE4
NATIVE_IMPORT void DS_STDCALL DeleteCriticalSection(NativeCriticalSection*);      // 0x0143FDE4
}

class CCritSec {
    NativeCriticalSection m_CritSec;
    CCritSec(const CCritSec&);
    CCritSec& operator=(const CCritSec&);
public:
    CCritSec() { InitializeCriticalSection(&m_CritSec); }
    ~CCritSec() { DeleteCriticalSection(&m_CritSec); }
};

struct IUnknown {
    virtual HRESULT DS_STDCALL QueryInterface(const GUID&, void**);
    virtual ULONG DS_STDCALL AddRef();
    virtual ULONG DS_STDCALL Release();
};

struct IDeferredCommand : IUnknown {
    virtual HRESULT DS_STDCALL Cancel();
    virtual HRESULT DS_STDCALL Confidence(long*);
    virtual HRESULT DS_STDCALL Postpone(REFTIME);
    virtual HRESULT DS_STDCALL GetHResult(HRESULT*);
};

// Native ABI view only: retail reads the IDeferredCommand subobject at +0x0c and calls inherited IUnknown::Release slot 2.
// The fixture supplies an interface-subobject double; it does not claim to construct a native CDeferredCommand.
struct CDeferredCommand {
    unsigned char NativePrimaryObjectPrefix[12];
    IDeferredCommand Interface;
};

struct IReferenceClock : IUnknown {
    virtual HRESULT DS_STDCALL GetTime(REFERENCE_TIME*);
    virtual HRESULT DS_STDCALL AdviseTime(REFERENCE_TIME, REFERENCE_TIME, HANDLE, DWORD_PTR*);
    virtual HRESULT DS_STDCALL AdvisePeriodic(REFERENCE_TIME, REFERENCE_TIME, HANDLE, DWORD_PTR*);
    virtual HRESULT DS_STDCALL Unadvise(DWORD_PTR);
};

struct CBaseList {
    struct CNode {
        CNode* m_pPrev;
        CNode* m_pNext;
        void* m_pObject;
        CNode* Prev() const { return m_pPrev; }
        CNode* Next() const { return m_pNext; }
        void* GetData() const { return m_pObject; }
        void SetPrev(CNode* node) { m_pPrev = node; }
        void SetNext(CNode* node) { m_pNext = node; }
        void SetData(void* object) { m_pObject = object; }
    };
    struct CNodeCache {
        int m_iCacheSize;
        int m_iUsed;
        CNode* m_pHead;
    };
    // Access is widened in this ABI-only projection so VC layout assertions and the fixture can inspect the exact public-header fields.
    CNode* m_pFirst;
    CNode* m_pLast;
    long m_Count;
    CNodeCache m_Cache;
    explicit CBaseList(LPCTSTR);
    ~CBaseList();
    void RemoveAll();                       // 0x00CB1340
    void* GetNextI(CNode*& position) const; // 0x00CB1380
    CNode* GetHeadPositionI() const { return m_pFirst; }
};

template<class OBJECT> class CGenericList : public CBaseList {
public:
    explicit CGenericList(LPCTSTR name) : CBaseList(name) {}
    CNode* GetHeadPosition() const { return GetHeadPositionI(); }
    OBJECT* GetNext(CNode*& position) const { return (OBJECT*)GetNextI(position); }
};

class CAMEvent {
public: // Access is widened for the native view and lifecycle fixture; public DirectShow declares the field protected.
    HANDLE m_hEvent;
    CAMEvent(BOOL fManualReset = 0, HRESULT* phr = 0);
    CAMEvent(HRESULT* phr);
    ~CAMEvent();                            // 0x00CB01B0
};

class AM_NOVTABLE CCmdQueue {
public:
    CCmdQueue(HRESULT* = 0);
    virtual ~CCmdQueue();
    virtual HRESULT New(CDeferredCommand**, LPUNKNOWN, REFTIME, GUID*, long, short, long, VARIANT*, VARIANT*, short*, BOOL);
    virtual HRESULT Insert(CDeferredCommand*);
    virtual HRESULT Remove(CDeferredCommand*);
    virtual HRESULT SetSyncSource(IReferenceClock*);
    virtual HRESULT Run(REFERENCE_TIME);
    virtual HRESULT EndRun();
    virtual HRESULT GetDueCommand(CDeferredCommand**, long);
    virtual HRESULT GetCommandDueFor(REFERENCE_TIME, CDeferredCommand**);
    // The retail ABI overlay records a vptr at +0 and an opaque word at +4; no meaning is assigned to that prefix.
    // VC7.1's novtable/virtual-class layout supplies this prefix before the first declared member.
    // Public DirectShow declares the remaining fields protected; access is widened here for fixture and layout checks.
    CCritSec m_Lock;                        // +8
    CGenericList<CDeferredCommand> m_listPresentation; // +0x20
    CGenericList<CDeferredCommand> m_listStream;       // +0x38
    CAMEvent m_evDue;                       // +0x50
    DWORD_PTR m_dwAdvise;                   // +0x54
    REFERENCE_TIME m_tCurrentAdvise;        // +0x58 (CRefTime's public one-field representation)
    IReferenceClock* m_pClock;              // +0x60
    BOOL m_bRunning;                        // +0x64
    REFERENCE_TIME m_StreamTimeOffset;      // +0x68 (CRefTime's public one-field representation)
};

// Explicit data-vptr overlay for native layout checks; it declares no virtual slots or methods.
struct CCmdQueueNativeLayout {
    void* NativeVptr;                       // actual vptr implied by the public virtual declaration
    unsigned long NativePrefix04;           // identity not recovered
    CCritSec m_Lock;
    CGenericList<CDeferredCommand> m_listPresentation;
    CGenericList<CDeferredCommand> m_listStream;
    CAMEvent m_evDue;
    DWORD_PTR m_dwAdvise;
    REFERENCE_TIME m_tCurrentAdvise;
    IReferenceClock* m_pClock;
    BOOL m_bRunning;
    REFERENCE_TIME m_StreamTimeOffset;
};

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(NativeCriticalSection) == 0x18);
FABLE_STATIC_ASSERT(sizeof(CBaseList::CNode) == 0x0c);
FABLE_STATIC_ASSERT(sizeof(CBaseList::CNodeCache) == 0x0c);
FABLE_STATIC_ASSERT(sizeof(CBaseList) == 0x18);
FABLE_STATIC_ASSERT(sizeof(REFERENCE_TIME) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CBaseList, m_pFirst) == 0x00);
FABLE_STATIC_ASSERT(offsetof(CBaseList, m_pLast) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CBaseList, m_Count) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CBaseList, m_Cache) == 0x0c);
FABLE_STATIC_ASSERT(sizeof(CCmdQueueNativeLayout) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CCmdQueueNativeLayout, NativeVptr) == 0x00);
FABLE_STATIC_ASSERT(offsetof(CCmdQueueNativeLayout, NativePrefix04) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CCmdQueueNativeLayout, m_Lock) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CCmdQueueNativeLayout, m_listPresentation) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CCmdQueueNativeLayout, m_listStream) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CCmdQueueNativeLayout, m_evDue) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CCmdQueueNativeLayout, m_dwAdvise) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CCmdQueueNativeLayout, m_tCurrentAdvise) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CCmdQueueNativeLayout, m_pClock) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CCmdQueueNativeLayout, m_bRunning) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CCmdQueueNativeLayout, m_StreamTimeOffset) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CDeferredCommand, Interface) == 0x0c);
#endif

CCmdQueue::~CCmdQueue()
{
    CBaseList::CNode* position = m_listPresentation.GetHeadPosition();
    while (position) {
        CDeferredCommand* command = m_listPresentation.GetNext(position);
        command->Interface.Release();
    }
    m_listPresentation.RemoveAll();

    position = m_listStream.GetHeadPosition();
    while (position) {
        CDeferredCommand* command = m_listStream.GetNext(position);
        command->Interface.Release();
    }
    m_listStream.RemoveAll();

    if (m_pClock) {
        if (m_dwAdvise) {
            m_pClock->Unadvise(m_dwAdvise);
            m_dwAdvise = 0;
        }
        m_pClock->Release();
    }
}