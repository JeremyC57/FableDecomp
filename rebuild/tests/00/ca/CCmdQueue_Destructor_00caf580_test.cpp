#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __int64 long long
#endif
#define NATIVE_IMPORT

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
#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#ifndef _MSC_VER
#define FABLE_STATIC_ASSERT(expression)
#else
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_abi_assert_, __LINE__)[(expression) ? 1 : -1]
#endif


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


#include <cstdio>
#include <cstdlib>
#include <new>

static int g_initCritical = 0;
static int g_deleteCritical = 0;
static NativeCriticalSection* g_lastCritical = 0;
static int g_closeHandles = 0;
static HANDLE g_lastHandle = 0;
static int g_nodeAllocations = 0;
static int g_nodeDeletes = 0;
static IUnknown* g_releaseTargets[3] = {0, 0, 0};
static int g_commandReleaseCounts[3] = {0, 0, 0};
static int g_releaseOrder[3] = {-1, -1, -1};
static int g_releaseOrderCount = 0;
static IReferenceClock* g_testClock = 0;
static CCmdQueue* g_activeQueue = 0;
static int g_unadviseCalls = 0;
static int g_clockReleaseCalls = 0;
static DWORD_PTR g_lastCookie = 0;
static int g_adviseStillPresentDuringUnadvise = 0;
static int g_cookieClearedBeforeClockRelease = 0;

void* operator new(size_t size) throw(std::bad_alloc)
{
    void* result = std::malloc(size);
    if (!result) throw std::bad_alloc();
    ++g_nodeAllocations;
    return result;
}

void operator delete(void* pointer) throw()
{
    if (pointer) {
        ++g_nodeDeletes;
        std::free(pointer);
    }
}

extern "C" void DS_STDCALL InitializeCriticalSection(NativeCriticalSection* section)
{
    ++g_initCritical;
    g_lastCritical = section;
}

extern "C" void DS_STDCALL DeleteCriticalSection(NativeCriticalSection* section)
{
    ++g_deleteCritical;
    if (section != g_lastCritical) g_deleteCritical = -1000;
}

static BOOL FixtureCloseHandle(HANDLE handle)
{
    ++g_closeHandles;
    g_lastHandle = handle;
    return 1;
}

CBaseList::CBaseList(LPCTSTR)
    : m_pFirst(0), m_pLast(0), m_Count(0)
{
    m_Cache.m_iCacheSize = 10;
    m_Cache.m_iUsed = 0;
    m_Cache.m_pHead = 0;
}

void CBaseList::RemoveAll()
{
    CNode* node = m_pFirst;
    while (node) {
        CNode* current = node;
        node = node->Next();
        delete current;
    }
    m_Count = 0;
    m_pFirst = 0;
    m_pLast = 0;
}

void* CBaseList::GetNextI(CNode*& position) const
{
    if (!position) return 0;
    CNode* current = position;
    position = current->Next();
    return current->GetData();
}

CBaseList::~CBaseList()
{
    RemoveAll();
    CNode* node = m_Cache.m_pHead;
    while (node) {
        CNode* current = node;
        node = node->Next();
        delete current;
    }
}

CAMEvent::CAMEvent(BOOL, HRESULT*) : m_hEvent(0) {}
CAMEvent::CAMEvent(HRESULT*) : m_hEvent(0) {}
CAMEvent::~CAMEvent()
{
    if (m_hEvent) FixtureCloseHandle(m_hEvent);
}

CCmdQueue::CCmdQueue(HRESULT*)
    : m_listPresentation("presentation"),
      m_listStream("stream"),
      m_evDue(1, 0),
      m_dwAdvise(0),
      m_tCurrentAdvise(0),
      m_pClock(0),
      m_bRunning(0),
      m_StreamTimeOffset(0)
{
}

HRESULT CCmdQueue::New(CDeferredCommand**, LPUNKNOWN, REFTIME, GUID*, long, short, long,
                       VARIANT*, VARIANT*, short*, BOOL) { return 0; }
HRESULT CCmdQueue::Insert(CDeferredCommand*) { return 0; }
HRESULT CCmdQueue::Remove(CDeferredCommand*) { return 0; }
HRESULT CCmdQueue::SetSyncSource(IReferenceClock*) { return 0; }
HRESULT CCmdQueue::Run(REFERENCE_TIME) { return 0; }
HRESULT CCmdQueue::EndRun() { return 0; }
HRESULT CCmdQueue::GetDueCommand(CDeferredCommand**, long) { return 0; }
HRESULT CCmdQueue::GetCommandDueFor(REFERENCE_TIME, CDeferredCommand**) { return 0; }

HRESULT DS_STDCALL IUnknown::QueryInterface(const GUID&, void**) { return 0; }
ULONG DS_STDCALL IUnknown::AddRef() { return 1; }
ULONG DS_STDCALL IUnknown::Release()
{
    if (this == static_cast<IUnknown*>(g_testClock)) {
        ++g_clockReleaseCalls;
        if (g_activeQueue && g_activeQueue->m_dwAdvise == 0)
            g_cookieClearedBeforeClockRelease = 1;
        return 1;
    }
    for (int i = 0; i != 3; ++i) {
        if (this == g_releaseTargets[i]) {
            ++g_commandReleaseCounts[i];
            if (g_releaseOrderCount < 3) g_releaseOrder[g_releaseOrderCount++] = i;
            return 1;
        }
    }
    return 0;
}

HRESULT DS_STDCALL IDeferredCommand::Cancel() { return 0; }
HRESULT DS_STDCALL IDeferredCommand::Confidence(long*) { return 0; }
HRESULT DS_STDCALL IDeferredCommand::Postpone(REFTIME) { return 0; }
HRESULT DS_STDCALL IDeferredCommand::GetHResult(HRESULT*) { return 0; }

HRESULT DS_STDCALL IReferenceClock::GetTime(REFERENCE_TIME*) { return 0; }
HRESULT DS_STDCALL IReferenceClock::AdviseTime(REFERENCE_TIME, REFERENCE_TIME, HANDLE, DWORD_PTR*) { return 0; }
HRESULT DS_STDCALL IReferenceClock::AdvisePeriodic(REFERENCE_TIME, REFERENCE_TIME, HANDLE, DWORD_PTR*) { return 0; }
HRESULT DS_STDCALL IReferenceClock::Unadvise(DWORD_PTR cookie)
{
    ++g_unadviseCalls;
    g_lastCookie = cookie;
    if (g_activeQueue && g_activeQueue->m_dwAdvise == cookie)
        g_adviseStillPresentDuringUnadvise = 1;
    return 0;
}

static void AppendCommand(CGenericList<CDeferredCommand>& list, CDeferredCommand* command)
{
    CBaseList::CNode* node = new CBaseList::CNode;
    node->m_pPrev = list.m_pLast;
    node->m_pNext = 0;
    node->m_pObject = command;
    if (list.m_pLast) list.m_pLast->m_pNext = node;
    else list.m_pFirst = node;
    list.m_pLast = node;
    ++list.m_Count;
}

static void AppendCachedNode(CBaseList& list)
{
    CBaseList::CNode* node = new CBaseList::CNode;
    node->m_pPrev = 0;
    node->m_pNext = list.m_Cache.m_pHead;
    node->m_pObject = 0;
    list.m_Cache.m_pHead = node;
    ++list.m_Cache.m_iUsed;
}

static bool CheckListsClockAndEvent()
{
    int oldInit = g_initCritical;
    int oldDelete = g_deleteCritical;
    IReferenceClock clock;
    CDeferredCommand first;
    CDeferredCommand second;
    CDeferredCommand third;
    int eventSentinel = 0;
    g_testClock = &clock;
    g_releaseTargets[0] = static_cast<IUnknown*>(&first.Interface);
    g_releaseTargets[1] = static_cast<IUnknown*>(&second.Interface);
    g_releaseTargets[2] = static_cast<IUnknown*>(&third.Interface);
    {
        CCmdQueue queue;
        g_activeQueue = &queue;
        queue.m_evDue.m_hEvent = &eventSentinel;
        queue.m_pClock = &clock;
        queue.m_dwAdvise = 0x12345678UL;
        AppendCommand(queue.m_listPresentation, &first);
        AppendCommand(queue.m_listPresentation, &second);
        AppendCommand(queue.m_listStream, &third);
        AppendCachedNode(queue.m_listPresentation);
        AppendCachedNode(queue.m_listStream);
    }
    g_activeQueue = 0;
    return g_commandReleaseCounts[0] == 1 && g_commandReleaseCounts[1] == 1 &&
           g_commandReleaseCounts[2] == 1 && g_releaseOrderCount == 3 &&
           g_releaseOrder[0] == 0 && g_releaseOrder[1] == 1 && g_releaseOrder[2] == 2 &&
           g_unadviseCalls == 1 && g_lastCookie == 0x12345678UL &&
           g_adviseStillPresentDuringUnadvise && g_clockReleaseCalls == 1 &&
           g_cookieClearedBeforeClockRelease && g_closeHandles == 1 &&
           g_lastHandle == &eventSentinel && g_nodeAllocations == 5 && g_nodeDeletes == 5 &&
           g_initCritical == oldInit + 1 && g_deleteCritical == oldDelete + 1;
}

static bool CheckNullClockAndEmptyLists()
{
    int oldClose = g_closeHandles;
    int oldReleases = g_clockReleaseCalls;
    int oldUnadvise = g_unadviseCalls;
    int oldInit = g_initCritical;
    int oldDelete = g_deleteCritical;
    {
        CCmdQueue queue;
        g_activeQueue = &queue;
        queue.m_pClock = 0;
        queue.m_dwAdvise = 0;
        queue.m_evDue.m_hEvent = 0;
    }
    g_activeQueue = 0;
    return g_closeHandles == oldClose && g_clockReleaseCalls == oldReleases &&
           g_unadviseCalls == oldUnadvise && g_initCritical == oldInit + 1 &&
           g_deleteCritical == oldDelete + 1;
}

static bool CheckClockWithoutAdvise()
{
    IReferenceClock clock;
    g_testClock = &clock;
    int oldReleases = g_clockReleaseCalls;
    int oldUnadvise = g_unadviseCalls;
    {
        CCmdQueue queue;
        g_activeQueue = &queue;
        queue.m_pClock = &clock;
        queue.m_dwAdvise = 0;
    }
    g_activeQueue = 0;
    return g_clockReleaseCalls == oldReleases + 1 && g_unadviseCalls == oldUnadvise;
}

static bool CheckNativeLayout()
{
    CCmdQueue queue;
#ifdef _MSC_VER
    char* base = (char*)&queue;
    if (sizeof(CCmdQueue) != 0x70 ||
        (char*)&queue.m_Lock - base != 0x08 ||
        (char*)&queue.m_listPresentation - base != 0x20 ||
        (char*)&queue.m_listStream - base != 0x38 ||
        (char*)&queue.m_evDue - base != 0x50 ||
        (char*)&queue.m_dwAdvise - base != 0x54 ||
        (char*)&queue.m_tCurrentAdvise - base != 0x58 ||
        (char*)&queue.m_pClock - base != 0x60 ||
        (char*)&queue.m_bRunning - base != 0x64 ||
        (char*)&queue.m_StreamTimeOffset - base != 0x68) {
        std::printf("layout size=%u lock=%ld pres=%ld stream=%ld event=%ld advise=%ld ref=%ld clock=%ld running=%ld origin=%ld\n",
                    (unsigned)sizeof(CCmdQueue), (long)((char*)&queue.m_Lock - base),
                    (long)((char*)&queue.m_listPresentation - base), (long)((char*)&queue.m_listStream - base),
                    (long)((char*)&queue.m_evDue - base), (long)((char*)&queue.m_dwAdvise - base),
                    (long)((char*)&queue.m_tCurrentAdvise - base), (long)((char*)&queue.m_pClock - base),
                    (long)((char*)&queue.m_bRunning - base), (long)((char*)&queue.m_StreamTimeOffset - base));
        return false;
    }
#endif
    return true;
}

int main()
{
    bool layout = CheckNativeLayout();
    bool first = CheckListsClockAndEvent();
    bool second = CheckNullClockAndEmptyLists();
    bool third = CheckClockWithoutAdvise();
    if (!layout || !first || !second || !third) {
        std::printf("diagnostic %d %d %d rel=%d,%d,%d order=%d,%d,%d ua=%d cookie=%lu clock=%d clear=%d close=%d nodes=%d/%d cs=%d/%d\n",
                    first, second, third, g_commandReleaseCounts[0], g_commandReleaseCounts[1],
                    g_commandReleaseCounts[2], g_releaseOrder[0], g_releaseOrder[1], g_releaseOrder[2],
                    g_unadviseCalls, g_lastCookie, g_clockReleaseCalls, g_cookieClearedBeforeClockRelease,
                    g_closeHandles, g_nodeAllocations, g_nodeDeletes, g_initCritical, g_deleteCritical);
        std::printf("00caf580_TEST FAIL\n");
        return 1;
    }
    std::printf("00caf580_TEST PASS\n");
    return 0;
}