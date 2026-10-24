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

int main()
{
    CHandleBasedMemoryPool pool;
    {
        CScopedMemoryHandle h(&pool, 1234);
        h.Clear();
        Check(pool.FreeCount == 1 && pool.LastHandle == 1234);
        Check(h.Pool == 0 && h.Handle == -1);
        h.Clear();
        Check(pool.FreeCount == 1);
        CScopedMemoryHandle none(0, 55);
        none.Clear();
        Check(none.Handle == 55 && pool.FreeCount == 1);
    }
    Check(pool.FreeCount == 1);

    if (g_fail) {
        std::printf("00a8d190_TEST FAIL\n");
        return 1;
    }
    std::printf("00a8d190_TEST PASS\n");
    return 0;
}