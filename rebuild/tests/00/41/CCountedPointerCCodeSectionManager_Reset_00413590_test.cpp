// Behaviour test for CCountedPointer<CCodeSectionManager>::Reset(T*) (row 0x00413590): engine/CCountedPointer.h with the row's opt-in.
// Portable: g++ -std=c++11 (64-bit) and VC7.1 /Od (linked with the row object; the Reset / DeleteData COMDATs coincide).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifdef _MSC_VER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
#else
// 64-bit host: rebuild_abi.h asserts 32-bit pointers, so mirror the engine/CCountedPointer.h members Reset uses (same bodies).
#define FABLE_FASTCALL
class CCPPointerInfo {
public:
    unsigned long RefCount; void (FABLE_FASTCALL* DeleteFunc)(void*); void* Data;
    CCPPointerInfo(unsigned long r, void (FABLE_FASTCALL* d)(void*), void* p) : RefCount(r), DeleteFunc(d), Data(p) { }
};
template <class T> class CCountedPointer {
public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer() { ReleaseData(); }
    void ReleaseData() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } Data = 0; Info = 0; }
    void Reset(T* data) { ReleaseData(); Data = data; if (data) { void* b = ::operator new(sizeof(CCPPointerInfo)); Info = b ? new (b) CCPPointerInfo(1, DeleteData, (void*)Data) : 0; } }   // VC7.1's new-expression null check
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }
};
#endif
 class CCodeSectionManager { public: int Value; }; 

static unsigned g_newCalls, g_deleteCalls, g_destroyCalls;
static void* g_lastDeleted;
static void* g_lastDestroyed;
static bool g_failAlloc;
static unsigned char g_block[64];

static bool g_armed;
void* operator new(size_t size)
{
    if (!g_armed) return malloc(size);
    ++g_newCalls;
    if (g_failAlloc) return 0;
    memset(g_block, 0, sizeof(g_block));
    return size <= sizeof(g_block) ? (void*)g_block : 0;
}
void operator delete(void* p) { if (!g_armed) { free(p); return; } ++g_deleteCalls; g_lastDeleted = p; }
static void FABLE_FASTCALL TestDestroy(void* object) { ++g_destroyCalls; g_lastDestroyed = object; }
static void Clear() { g_newCalls = g_deleteCalls = g_destroyCalls = 0; g_lastDeleted = g_lastDestroyed = 0; g_failAlloc = false; }

typedef CCodeSectionManager T;
int main()
{
    bool ok = true;
    T oldObj, newObj;
    g_armed = true;
    // shared block: decrement only, then a fresh block for the new object
    Clear();
    CCPPointerInfo shared(2, TestDestroy, &oldObj);
    CCountedPointer<T> p; p.Data = &oldObj; p.Info = &shared;
    p.Reset(&newObj);
    CCPPointerInfo* fresh = (CCPPointerInfo*)g_block;
    ok = ok && shared.RefCount == 1 && g_destroyCalls == 0 && g_deleteCalls == 0 && g_newCalls == 1
            && p.Data == &newObj && p.Info == fresh && fresh->RefCount == 1 && fresh->Data == (void*)&newObj
            && fresh->DeleteFunc == &CCountedPointer<T>::DeleteData;
    // last owner, reset to null: destroy + free the block, nothing new
    Clear();
    CCPPointerInfo last(1, TestDestroy, &oldObj);
    CCountedPointer<T> q; q.Data = &oldObj; q.Info = &last;
    q.Reset(0);
    ok = ok && last.RefCount == 0 && g_destroyCalls == 1 && g_lastDestroyed == (void*)&oldObj && g_deleteCalls == 1
            && g_lastDeleted == (void*)&last && g_newCalls == 0 && q.Data == 0 && q.Info == 0;
    // no block, allocation fails: Data kept, Info 0
    Clear(); g_failAlloc = true;
    CCountedPointer<T> r;
    r.Reset(&newObj);
    ok = ok && g_newCalls == 1 && r.Data == &newObj && r.Info == 0;
    r.Info = 0; r.Data = 0; p.Info = 0; q.Info = 0;   // the blocks above are test-owned: keep the destructors quiet
    g_armed = false;
    printf(ok ? "FABLETLC_PHYSICS_FACES_COUNTED_RESET_413590 PASS\n" : "FABLETLC_PHYSICS_FACES_COUNTED_RESET_413590 FAIL\n");
    return ok ? 0 : 1;
}
