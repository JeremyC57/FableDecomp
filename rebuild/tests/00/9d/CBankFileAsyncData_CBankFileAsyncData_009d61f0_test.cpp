// Behaviour test for 009d61f0: final.cpp copied verbatim (the dllimport declaration made plain, Sleep renamed by macro to
// keep clear of kernel32), the declared callees stubbed below so calls and arguments are observable.
// CBankFileAsyncData::~CBankFileAsyncData()
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __stdcall
#define __cdecl
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[2048];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
#define Sleep Test_Sleep
// ---- source (verbatim) ----
// CBankFileAsyncData::~CBankFileAsyncData() -- unless aborted: under the bank's critical section, drop this request from the bank's instance and restart sets, spin (unlocking around Sleep(0)) until any outstanding threaded read has finished, and give the pool handle back; then return an internally allocated buffer to the global allocator (destroying the allocator when it empties and is flagged to die) and run the base destructors
// Leaf: ??1CBankFileAsyncData@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D61F0, length 348 bytes (+ int3 padding), ebp frame aligned to 8, plain ret.
// Callees: CCriticalSectionLock ctor @ 0x00A62920 / Unlock @ 0x00A62960 / Lock @ 0x00A62980 / dtor @ 0x00A629A0,
//          set<CBankFileAsyncData*>::erase(const key&) @ 0x009D7F40 (_Rb_tree, twice), CThreadedFile::HasReadFinished(ulong)
//          @ 0x0098EC90 (a thunk onto the global thread manager that ignores `this`), Sleep (kernel32 import 0x0143FE1C,
//          hoisted into edi), CHandleBasedMemoryPool::Free @ 0x00A62ED0, GFEnterAllocatorLock @ 0x009F2000 /
//          GFLeaveAllocatorLock @ 0x009F2010, CMemoryAllocator::InternalDeallocate @ 0x009F2920 / GetAllocationCount
//          @ 0x009F1F10 / ~CMemoryAllocator @ 0x009F2310 / operator delete @ 0x009F1F90 (the landed
//          GFFreeFromGlobalAllocator body, inlined), CBase::~CBase @ 0x0099A300. Vtables 0x0129B8B4 (primary) /
//          0x0129B8A0 (the threaded-file request base inside CBankFileAsyncData) / 0x0129B860 (that base's own).
// Globals: the loading allocator @ 0x013CAA90 and its destroy-when-empty flag @ 0x013CAA94.
// Layout: Ego_r.pdb CBankFileAsyncData (0x48; two vptrs): Data +8, Buffer +0xc, Length +0x10, SkipBytes +0x14,
//          ThreadedFileUID +0x18, MemoryPool +0x1c, MemoryHandle +0x20, InternalAllocatedBufferSize +0x24, BankFile +0x28,
//          Finished +0x44, BufferLocked +0x45, AllowRestart +0x46, Aborted +0x47; the bank's sets at +0x128 / +0x134, its
//          critical section at +0x14c, its threaded file (counted pointer) at +0x110.
// Shape: the erase key is the implicit `this` temporary (one frame word reused by both erases); the lock local is
//          8-aligned (`and esp,-8`); the wait is a peeled test plus a do-while that unlocks around Sleep(0), the threaded
//          file reached through an inline accessor so the object loads before the UID push (a direct member chain pushes
//          first); the buffer free is the inlined GFFreeFromGlobalAllocator with the buffer read into edi before the lock.
extern "C" void __stdcall Sleep(unsigned long milliseconds);   // import 0x0143FE1C (plain declaration for the test)

struct CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };
struct __declspec(align(8)) CCriticalSectionLock_Retail {   // 8 bytes on the stack
    CRITICAL_SECTION_Fable* m_pSection;
    bool                    m_locked;
    CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock_Retail();                                               // 0x00A629A0
    void Unlock();                                                                // 0x00A62960
    void Lock();                                                                  // 0x00A62980
};

class CMemoryAllocator { public:
    void InternalDeallocate(void* block);          // 0x009F2920
    long GetAllocationCount();                     // 0x009F1F10
    ~CMemoryAllocator();                           // 0x009F2310
    static void __fastcall operator delete(void* block);   // 0x009F1F90
};
void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010
extern CMemoryAllocator* g_pGlobalAllocator_013caa90;          // 0x013CAA90
extern bool              g_bDestroyAllocatorWhenEmpty_013caa94; // 0x013CAA94
__forceinline void GFFreeFromGlobalAllocator(void* block)
{
    GFEnterAllocatorLock();
    if (block && g_pGlobalAllocator_013caa90) {
        g_pGlobalAllocator_013caa90->InternalDeallocate(block);
        if (g_bDestroyAllocatorWhenEmpty_013caa94) {
            if (g_pGlobalAllocator_013caa90->GetAllocationCount() == 0) {
                delete g_pGlobalAllocator_013caa90;
                g_pGlobalAllocator_013caa90 = 0;
            }
        }
    }
    GFLeaveAllocatorLock();
}

class CHandleBasedMemoryPool { public: void Free_00a62ed0(long handle); };   // 0x00A62ED0
class CThreadedFile {
public:
    bool HasReadFinished(unsigned long readUID);   // 0x0098EC90
};
class CBankFileAsyncData;
struct CAsyncDataSet {                             // set<CBankFileAsyncData*> (STLport _Rb_tree)
    void* _M_header;
    unsigned int _M_node_count;
    unsigned char _M_key_compare;
    unsigned int erase(CBankFileAsyncData* const& key);   // 0x009D7F40
};
class CBankFileAsync { public:
    unsigned char _pad000[0x110];
    CThreadedFile* ThreadedFile;                   // +0x110 (counted pointer object)
    unsigned char _pad114[0x14];
    CAsyncDataSet AsyncDataInstances;              // +0x128
    CAsyncDataSet ReadsToRestart;                  // +0x134
    unsigned char _pad140[0xc];
    CRITICAL_SECTION_Fable CriticalSection;        // +0x14c
    CThreadedFile* GetThreadedFile() const { return ThreadedFile; }
};

namespace N_0099a300 { struct CBase {
    virtual ~CBase();                              // 0x0099A300
}; } using N_0099a300::CBase;
struct CThreadedFileRequest {                      // the second base (vtable 0x0129B860): the file thread's callback interface
    virtual ~CThreadedFileRequest() { }
};
class CBankFileAsyncData : public CBase, public CThreadedFileRequest {   // vtables 0x0129B8B4 / 0x0129B8A0
public:
    void*                   Data;                  // +0x08
    void*                   Buffer;                // +0x0c
    unsigned long           Length;                // +0x10
    unsigned long           SkipBytes;             // +0x14
    unsigned long           ThreadedFileUID;       // +0x18
    CHandleBasedMemoryPool* MemoryPool;            // +0x1c
    long                    MemoryHandle;          // +0x20
    unsigned long           InternalAllocatedBufferSize;   // +0x24
    CBankFileAsync*         BankFile;              // +0x28
    unsigned char           LoadDesc[0x18];        // +0x2c
    bool                    Finished;              // +0x44
    bool                    BufferLocked;          // +0x45
    bool                    AllowRestart;          // +0x46
    bool                    Aborted;               // +0x47
    virtual ~CBankFileAsyncData();
};

CBankFileAsyncData::~CBankFileAsyncData()
{
    if (!Aborted) {
        CCriticalSectionLock_Retail lock(&BankFile->CriticalSection, false);
        BankFile->AsyncDataInstances.erase(this);
        BankFile->ReadsToRestart.erase(this);
        if (ThreadedFileUID) {
            if (!BankFile->GetThreadedFile()->HasReadFinished(ThreadedFileUID)) {
                do {
                    lock.Unlock();
                    Sleep(0);
                    lock.Lock();
                } while (!BankFile->GetThreadedFile()->HasReadFinished(ThreadedFileUID));
            }
        }
        if (MemoryPool && MemoryHandle != -1) {
            MemoryPool->Free_00a62ed0(MemoryHandle);
            MemoryHandle = -1;
            Buffer = 0;
            Data = 0;
        }
    }
    if (InternalAllocatedBufferSize > 0)
        GFFreeFromGlobalAllocator(Buffer);
}

// ---- stubs ----
extern "C" void __stdcall Sleep(unsigned long milliseconds) { char b[32]; _snprintf(b, sizeof b, "sleep%lu", milliseconds); note(b); }
static CBankFileAsync g_bank;
CCriticalSectionLock_Retail::CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* section, bool tryOnly) : m_pSection(section), m_locked(true) { CHECK(section == &g_bank.CriticalSection && !tryOnly); note("lock"); }
CCriticalSectionLock_Retail::~CCriticalSectionLock_Retail() { note("~lock"); }
void CCriticalSectionLock_Retail::Unlock() { note("unlock"); m_locked = false; }
void CCriticalSectionLock_Retail::Lock() { note("relock"); m_locked = true; }
static int g_allocatorDeallocs = 0, g_allocatorCount = 1, g_allocatorDtors = 0, g_allocatorDeletes = 0;
void CMemoryAllocator::InternalDeallocate(void* block) { ++g_allocatorDeallocs; char b[32]; _snprintf(b, sizeof b, "dealloc%d", block != 0); note(b); }
long CMemoryAllocator::GetAllocationCount() { note("count"); return g_allocatorCount; }
CMemoryAllocator::~CMemoryAllocator() { ++g_allocatorDtors; note("~allocator"); }
void __fastcall CMemoryAllocator::operator delete(void* block) { ++g_allocatorDeletes; (void)block; note("free-allocator"); }
void __cdecl GFEnterAllocatorLock() { note("enter"); }
void __cdecl GFLeaveAllocatorLock() { note("leave"); }
static CMemoryAllocator g_allocatorObject;
CMemoryAllocator* g_pGlobalAllocator_013caa90 = &g_allocatorObject;
bool g_bDestroyAllocatorWhenEmpty_013caa94 = false;
void CHandleBasedMemoryPool::Free_00a62ed0(long handle) { char b[32]; _snprintf(b, sizeof b, "poolfree%ld", handle); note(b); }
static int g_readsLeft = 0;
bool CThreadedFile::HasReadFinished(unsigned long readUID) { char b[32]; _snprintf(b, sizeof b, "finished?%lu", readUID); note(b); return g_readsLeft-- <= 0; }
static CBankFileAsyncData* g_erased[4]; static int g_erases = 0;
unsigned int CAsyncDataSet::erase(CBankFileAsyncData* const& key) { note(this == &g_bank.AsyncDataInstances ? "erase-instances" : "erase-restart"); if (g_erases < 4) g_erased[g_erases] = key; ++g_erases; return 1; }
CBase::~CBase() { note("~base"); }
static CThreadedFile g_file; static CHandleBasedMemoryPool g_pool;
static unsigned char g_storage[sizeof(CBankFileAsyncData) + 16];
static CBankFileAsyncData* make(bool aborted, unsigned long uid, bool pool, long handle, unsigned long internalSize, void* buffer)
{
    CBankFileAsyncData* d = new (g_storage) CBankFileAsyncData;
    d->Data = (void*)1; d->Buffer = buffer; d->Length = 0; d->SkipBytes = 0; d->ThreadedFileUID = uid; d->MemoryPool = pool ? &g_pool : 0;
    d->MemoryHandle = handle; d->InternalAllocatedBufferSize = internalSize; d->BankFile = &g_bank; d->Finished = false; d->BufferLocked = false; d->AllowRestart = false; d->Aborted = aborted;
    g_log[0] = 0; g_erases = 0; g_allocatorDeallocs = g_allocatorDtors = g_allocatorDeletes = 0; g_pGlobalAllocator_013caa90 = &g_allocatorObject; g_bDestroyAllocatorWhenEmpty_013caa94 = false; g_allocatorCount = 1; g_readsLeft = 0;
    return d;
}
int main()
{
    g_bank.ThreadedFile = &g_file; char buf[4];
    // 1. aborted request without an internal buffer: only the base destructors
    { CBankFileAsyncData* d = make(true, 5, true, 3, 0, buf); d->~CBankFileAsyncData(); CHECKLOG("~base;"); CHECK(d->MemoryHandle == 3); }
    // 2. live request, no outstanding read, pool handle given back
    { CBankFileAsyncData* d = make(false, 0, true, 7, 0, buf); d->~CBankFileAsyncData();
      CHECKLOG("lock;erase-instances;erase-restart;poolfree7;~lock;~base;"); CHECK(g_erases == 2 && g_erased[0] == d && g_erased[1] == d);
      CHECK(d->MemoryHandle == -1 && d->Buffer == 0 && d->Data == 0); }
    // 3. outstanding read that finishes after two sleeps; no pool
    { CBankFileAsyncData* d = make(false, 9, false, 7, 0, buf); g_readsLeft = 2; d->~CBankFileAsyncData();
      CHECKLOG("lock;erase-instances;erase-restart;finished?9;unlock;sleep0;relock;finished?9;unlock;sleep0;relock;finished?9;~lock;~base;"); CHECK(d->MemoryHandle == 7); }
    // 4. read already finished; pool present but no handle
    { CBankFileAsyncData* d = make(false, 4, true, -1, 0, buf); d->~CBankFileAsyncData(); CHECKLOG("lock;erase-instances;erase-restart;finished?4;~lock;~base;"); }
    // 5. aborted, internal buffer returned to the allocator; allocator kept (not flagged)
    { CBankFileAsyncData* d = make(true, 0, false, -1, 64, buf); d->~CBankFileAsyncData(); CHECKLOG("enter;dealloc1;leave;~base;"); CHECK(g_pGlobalAllocator_013caa90 == &g_allocatorObject); }
    // 6. flagged allocator with allocations left: kept; then empty: destroyed and cleared
    { CBankFileAsyncData* d = make(true, 0, false, -1, 64, buf); g_bDestroyAllocatorWhenEmpty_013caa94 = true; g_allocatorCount = 2; d->~CBankFileAsyncData();
      CHECKLOG("enter;dealloc1;count;leave;~base;"); CHECK(g_pGlobalAllocator_013caa90 == &g_allocatorObject); }
    { CBankFileAsyncData* d = make(true, 0, false, -1, 64, buf); g_bDestroyAllocatorWhenEmpty_013caa94 = true; g_allocatorCount = 0; d->~CBankFileAsyncData();
      CHECKLOG("enter;dealloc1;count;~allocator;free-allocator;leave;~base;"); CHECK(g_pGlobalAllocator_013caa90 == 0 && g_allocatorDtors == 1 && g_allocatorDeletes == 1); }
    // 7. internal size set but no buffer / no allocator: lock taken and released only
    { CBankFileAsyncData* d = make(true, 0, false, -1, 64, 0); d->~CBankFileAsyncData(); CHECKLOG("enter;leave;~base;"); }
    { CBankFileAsyncData* d = make(true, 0, false, -1, 64, buf); g_pGlobalAllocator_013caa90 = 0; d->~CBankFileAsyncData(); CHECKLOG("enter;leave;~base;"); }
    if (fails == 0) printf("009d61f0_TEST PASS\n"); else printf("009d61f0_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}