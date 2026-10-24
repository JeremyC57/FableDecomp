#include <cstdio>
#include <new>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

static int g_fail = 0;
static void Check(bool ok) { if (!ok) g_fail = 1; }

class CHandleBasedMemoryPool {                          // records the handles it is asked to free
public:
    int  FreeCount;
    long LastHandle;
    CHandleBasedMemoryPool() : FreeCount(0), LastHandle(0) { }
    void Free(long handle) { ++FreeCount; LastHandle = handle; }
};
class CScopedMemoryHandle {
public:
    CHandleBasedMemoryPool* Pool;
    long                    Handle;
    CScopedMemoryHandle(CHandleBasedMemoryPool* pool, long handle) : Pool(pool), Handle(handle) { }
    // 0x00A8D190: give the handle back to its pool once
    void Clear()
    {
        if (Pool) {
            Pool->Free(Handle);
            Pool = 0;
            Handle = -1;
        }
    }
    ~CScopedMemoryHandle() { Clear(); }
};

// the row: CCountedPointer<CScopedMemoryHandle>::DeleteData -- the count block's delete hook
static void __fastcall DeleteData(void* data)
{
    delete (CScopedMemoryHandle*)data;
}

int main()
{
    CHandleBasedMemoryPool pool;
    void (__fastcall* hook)(void*) = &DeleteData;
    hook(new CScopedMemoryHandle(&pool, 99));
    Check(pool.FreeCount == 1 && pool.LastHandle == 99);
    hook(new CScopedMemoryHandle(0, 7));
    Check(pool.FreeCount == 1);
    CScopedMemoryHandle* cleared = new CScopedMemoryHandle(&pool, 3);
    cleared->Clear();
    hook(cleared);
    Check(pool.FreeCount == 2 && pool.LastHandle == 3);
    DeleteData(0);
    Check(pool.FreeCount == 2);

    if (g_fail) {
        std::printf("00a8d7c0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a8d7c0_TEST PASS\n");
    return 0;
}