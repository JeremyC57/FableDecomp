// Behaviour test for 00a02b30: final.cpp copied verbatim, the imports and the two pool helpers stubbed below so the
// lock calls, the free-list calls and every store are observable. CHandleBasedMemoryPool::Free
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __stdcall
#define __cdecl
#define _snprintf snprintf
#endif
#define __declspec(x)
#define EnterCriticalSection Test_EnterCriticalSection
#define LeaveCriticalSection Test_LeaveCriticalSection
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[2048];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CHandleBasedMemoryPool::Free(long handle) -- under the pool lock: mark the handle's area free, put it on its free list, account the allocation change, merge it with free physical neighbours (next, then previous) and push the handle slot onto the free-handle chain
// Leaf: Free
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A02B30, 201 bytes, `ret 4`.
// Callees: EnterCriticalSection / LeaveCriticalSection through their imports (0x0143FE0C / 0x0143FE10) when MultiThreaded,
//   CHandleBasedMemoryPool::InsertFreeArea @ 0x00A01DA0, CHandleBasedMemoryPool::CombineAreas(CHeader* first, CHeader*
//   second) @ 0x00A01E40 (both landed).
// Layout: Ego_r.pdb CHandleBasedMemoryPool names; engine/CHandleBasedMemoryPool.h gives Handles a 16-byte CArray, retail's
//   is 12 bytes (landed ValidateResources 0x00A02AD0 / InternalAllocateHandle 0x00A03040), so everything after +0x438
//   sits 4 bytes lower in retail (FirstUnusedHandle +0x444, CriticalSection +0x44c) and `this` is modelled locally.
//   CHeader is the Ego_r.pdb 32-byte record; CHandleInfo the 4-byte union {Header, NextFree}.
// Shape: the handle slot's byte offset kept in ebp (`lea ebp,[ebx*4]`); the sizes are read back after InsertFreeArea
//   (which folded Padding into Size); NumAllocations is bumped on free as on allocation (retail behaviour); the slot is
//   cleared, then re-addressed as a CHandleInfo reference that is cleared again and given the old chain head.
//   The final MultiThreaded test reads `this` through a reference (`CHandleBasedMemoryPool* const& self = this`): the
//   FirstUnusedHandle store then stays before the MultiThreaded reload (a direct member read lets VC sink the store past
//   the load and the pops, since both are provably disjoint `this` offsets; through the reference they may alias).
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // 0x0143FE10

struct CHeader {                                   // CHandleBasedMemoryPool::CHeader, 32 bytes in front of every area
    void*     Resource;                            // +0x00
    long      Handle;                              // +0x04
    long      Size;                                // +0x08
    short     Padding;                             // +0x0c
    unsigned char Status;                          // +0x0e ('N' free, 'T' taken)
    unsigned char Locked;                          // +0x0f
    CHeader** RefAvailable;                        // +0x10 (the link that points at this free area)
    CHeader*  NextAvailable;                       // +0x14
    CHeader*  PrevContiguous;                      // +0x18
    CHeader*  NextContiguous;                      // +0x1c
    void RemoveFromFreeList()
    {
        if (RefAvailable && Status == 'N') {
            Status = 0;
            *RefAvailable = NextAvailable;
            if (NextAvailable)
                NextAvailable->RefAvailable = RefAvailable;
            RefAvailable = 0;
            NextAvailable = 0;
        }
    }
};

union CHandleInfo {                                // CHandleBasedMemoryPool::CHandleInfo (4 bytes): the area, or the next free slot
    CHeader* Header;
    long     NextFree;
};

struct SCriticalSection {                          // RTL_CRITICAL_SECTION (24 bytes)
    void* DebugInfo;
    long  LockCount;
    long  RecursionCount;
    void* OwningThread;
    void* LockSemaphore;
    unsigned long SpinCount;
};

class CHandleBasedMemoryPool { public:
    virtual void v00();
    CHeader*      Buffer;                          // +0x04
    void*         UnalignedBuffer;                 // +0x08
    long          PoolSize;                        // +0x0c
    long          AllocatedSize;                   // +0x10
    long          NumAllocations;                  // +0x14
    long          LockCount;                       // +0x18
    long          LargestFreeArea;                 // +0x1c
    unsigned long MemoryFlags;                     // +0x20
    bool          MultiThreaded;                   // +0x24
    bool          ShutdownFlag;                    // +0x25
    bool          EnableDefrag;                    // +0x26
    bool          EnableFullDefragIfPartialFails;  // +0x27
    unsigned long MaxFreeSizeBeforeDefrag;         // +0x28
    unsigned long MaxFreeSizeBeforeFailure;        // +0x2c
    long          MaxBlocksToTryOnPartialDefrag;   // +0x30
    void*         FailureHandler;                  // +0x34
    CHeader*      FreeLists[256];                  // +0x38
    CHandleInfo*  Handles;                         // +0x438 CArray<CHandleInfo> (12 bytes in retail: first, last, end)
    CHandleInfo*  HandlesLast;                     // +0x43c
    CHandleInfo*  HandlesEnd;                      // +0x440
    long          FirstUnusedHandle;               // +0x444
    void*         Name;                            // +0x448 CCharString
    SCriticalSection CriticalSection;              // +0x44c
    void InsertFreeArea(CHeader* area);            // 0x00A01DA0
    void CombineAreas(CHeader* first, CHeader* second);   // 0x00A01E40
    void Free(long handle);
};

void CHandleBasedMemoryPool::Free(long handle)
{
    if (MultiThreaded)
        EnterCriticalSection(&CriticalSection);
    CHeader* area = Handles[handle].Header;
    area->Status = 'N';
    area->Locked = 0;
    InsertFreeArea(area);
    ++NumAllocations;
    AllocatedSize -= area->Padding + area->Size + 0x20;
    if (area->NextContiguous && area->NextContiguous->Status == 'N')
        CombineAreas(area, area->NextContiguous);
    if (area->PrevContiguous && area->PrevContiguous->Status == 'N')
        CombineAreas(area->PrevContiguous, area);
    Handles[handle].Header = 0;
    CHandleInfo& slot = Handles[handle];
    slot.Header = 0;
    slot.NextFree = FirstUnusedHandle;
    FirstUnusedHandle = handle;
    CHandleBasedMemoryPool* const& self = this;
    if (self->MultiThreaded)
        LeaveCriticalSection(&self->CriticalSection);
}
// ---- stubs ----
static CHandleBasedMemoryPool* g_pool;
static CHeader g_areas[3];
static const char* AreaName(const CHeader* a) { return a == &g_areas[0] ? "A" : a == &g_areas[1] ? "B" : a == &g_areas[2] ? "C" : "?"; }
extern "C" void __stdcall Test_EnterCriticalSection(void* section) { note(section == &g_pool->CriticalSection ? "enter" : "enter(?)"); }
extern "C" void __stdcall Test_LeaveCriticalSection(void* section) { note(section == &g_pool->CriticalSection ? "leave" : "leave(?)"); }
void CHandleBasedMemoryPool::v00() { }
void CHandleBasedMemoryPool::InsertFreeArea(CHeader* area)
{
    char b[64]; _snprintf(b, sizeof b, "insert(%s,%c,%d)", AreaName(area), area->Status, (int)area->Locked); note(b);
    area->Size += area->Padding; area->Padding = 0;      // retail folds the padding into the size
}
void CHandleBasedMemoryPool::CombineAreas(CHeader* first, CHeader* second)
{
    char b[64]; _snprintf(b, sizeof b, "combine(%s,%s)", AreaName(first), AreaName(second)); note(b);
}
int main()
{
    CHandleBasedMemoryPool* pool = (CHandleBasedMemoryPool*)calloc(1, sizeof(CHandleBasedMemoryPool)); g_pool = pool;
    CHandleInfo handles[4]; memset(handles, 0, sizeof handles);
    pool->Handles = handles; pool->HandlesLast = handles + 4; pool->HandlesEnd = handles + 4;
    memset(g_areas, 0, sizeof g_areas);
    CHeader& A = g_areas[0]; CHeader& B = g_areas[1]; CHeader& C = g_areas[2];
    A.NextContiguous = &B; B.PrevContiguous = &A; B.NextContiguous = &C; C.PrevContiguous = &B;
    // 1. single-threaded, free B: neighbours taken, so no combines; padding folded into the size for the accounting
    B.Status = 'T'; B.Locked = 1; B.Size = 100; B.Padding = 12; A.Status = 'T'; C.Status = 'T';
    handles[2].Header = &B; pool->FirstUnusedHandle = 7; pool->NumAllocations = 5; pool->AllocatedSize = 1000; pool->MultiThreaded = false;
    g_log[0] = 0; pool->Free(2);
    CHECKLOG("insert(B,N,0);");
    CHECK(B.Status == 'N' && B.Locked == 0 && pool->NumAllocations == 6 && pool->AllocatedSize == 1000 - (112 + 0x20));
    CHECK(handles[2].NextFree == 7 && pool->FirstUnusedHandle == 2);
    // 2. multi-threaded, free A: next (B) is free -> combine(A,B); no previous
    A.Status = 'T'; A.Locked = 1; A.Size = 50; A.Padding = 0; handles[0].Header = &A; pool->MultiThreaded = true;
    g_log[0] = 0; pool->Free(0);
    CHECKLOG("enter;insert(A,N,0);combine(A,B);leave;");
    CHECK(pool->NumAllocations == 7 && pool->AllocatedSize == 1000 - 144 - 82 && handles[0].NextFree == 2 && pool->FirstUnusedHandle == 0);
    // 3. free C: previous (B) free -> combine(B,C); next null; a non-free previous would be skipped
    C.Status = 'T'; C.Size = 10; C.Padding = 2; handles[3].Header = &C; pool->MultiThreaded = false;
    g_log[0] = 0; pool->Free(3);
    CHECKLOG("insert(C,N,0);combine(B,C);");
    CHECK(handles[3].NextFree == 0 && pool->FirstUnusedHandle == 3 && pool->AllocatedSize == 1000 - 144 - 82 - 44);
    // 4. free B again with both neighbours taken-and-locked ('T'): nothing combined; and a taken previous is not combined
    A.Status = 'T'; C.Status = 'T'; B.Status = 'T'; B.Size = 5; B.Padding = 0; handles[1].Header = &B;
    g_log[0] = 0; pool->Free(1);
    CHECKLOG("insert(B,N,0);"); CHECK(handles[1].NextFree == 3 && pool->FirstUnusedHandle == 1);
    free(pool);
    if (fails == 0) printf("00a02b30_TEST PASS\n"); else printf("00a02b30_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}