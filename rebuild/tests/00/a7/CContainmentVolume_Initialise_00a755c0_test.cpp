// Behaviour test for 00a755c0: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CContainmentVolume::Initialise(const CPlane*, unsigned long, CMovableResourceMemoryPool*)
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __int64 long long
#else
#define snprintf _snprintf
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CContainmentVolume::Initialise(const CPlane* planes, unsigned long count, CMovableResourceMemoryPool* pool) -- size the plane array (movable pool block when a pool is given, else new CPlane[count]) and copy the planes in
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A755C0, 174 bytes, `ret 0xc`.
// Callees: CMovableResourceMemoryPool::AllocateResource(long*, CMovableResource*, ulong, ulong) 0x00A62E40,
//   CHandleBasedMemoryPool::GetPointerFromHandle(long) 0x00A026B0, operator new[] 0x00BFEB22,
//   `vector constructor iterator' 0x004038C0 with the out-of-line empty CPlane::CPlane() 0x0063EC80,
//   the LTCG std::copy<const CPlane*, CPlane*> 0x00A759E0 (first in ecx, last in edx, dest on the stack, `ret 4`
//   = exactly __fastcall).
// Layout: engine/CContainmentVolume.h (ContainmentPlanes +4, Size +8, MemoryPool +0xc); CPlane = { C3DVector Normal; float Distance }.
// Shape: Size and MemoryPool are stored before their tests; the pool block size goes through the same
//   `__int64` 0..0x7fffffff guard as the stream reads; the handle lives in the dead `count` slot. The size guard and
//   the AllocateResource call sit in ONE __forceinline AllocateChecked wrapper tested by a single `if`: VC7.1 then lays
//   the shared `return false` block as the fall-through of the AllocateResource test (`jne success`) with the size guard
//   jumping forward to it; two separate `if (...) return false;` statements keep the first copy and jump back to it.
#include "engine/CContainmentVolume.h"

void* __cdecl operator new[](unsigned int size);   // MSVCR71 import thunk 0x00BFEB22 (the lane <new> lacks it)

struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct CPlane {
    C3DVector Normal;
    float     Distance;
    CPlane();                                                      // 0x0063EC80 (empty, out of line)
};

CPlane* __fastcall CopyPlanes(const CPlane* first, const CPlane* last, CPlane* dest);   // 0x00A759E0

struct CHandleBasedMemoryPool {
    void* GetPointerFromHandle(long handle);                       // 0x00A026B0
};

struct CMovableResourceMemoryPool : CHandleBasedMemoryPool {
    bool AllocateResource(long* handle, void* owner, unsigned long size, unsigned long alignment);   // 0x00A62E40
};

static __forceinline bool IsValidSize(__int64 size) { return size >= 0 && size <= 0x7fffffff; }
static __forceinline bool AllocateChecked(CMovableResourceMemoryPool* pool, long* handle, unsigned long bytes, void* owner);

struct CContainmentVolume_Methods : CContainmentVolume {
    bool Initialise(const CPlane* planes, unsigned long count, CMovableResourceMemoryPool* pool);
};

static __forceinline bool AllocateChecked(CMovableResourceMemoryPool* pool, long* handle, unsigned long bytes, void* owner)
{
    if (!IsValidSize((__int64)bytes))
        return false;
    return pool->AllocateResource(handle, owner, bytes, 0);
}
bool CContainmentVolume_Methods::Initialise(const CPlane* planes, unsigned long count, CMovableResourceMemoryPool* pool)
{
    Size = count;
    if (count == 0)
        return true;
    MemoryPool = (::CMovableResourceMemoryPool*)pool;
    if (pool) {
        unsigned long bytes = count * sizeof(CPlane);
        long handle;
        if (!AllocateChecked(pool, &handle, bytes, this))
            return false;
        ContainmentPlanes = (::CPlane*)((CMovableResourceMemoryPool*)MemoryPool)->GetPointerFromHandle(handle);
    }
    else
        ContainmentPlanes = (::CPlane*)new CPlane[count];
    CopyPlanes(planes, planes + count, (CPlane*)ContainmentPlanes);
    return true;
}

// ---- stubs ----
static char g_log[2048];
static void logf(const char* fmt, long a = 0, long b = 0) { char t[256]; _snprintf(t, sizeof t, fmt, a, b); strcat(g_log, t); strcat(g_log, ";"); }
CPlane::CPlane() { logf("CPlane()"); }
static CPlane g_block[8];
static bool g_allocOk;
CPlane* __fastcall CopyPlanes(const CPlane* first, const CPlane* last, CPlane* dest)
{
    logf("CopyPlanes(%ld)", (long)(last - first));
    CPlane* d = dest;
    for (const CPlane* p = first; p != last; ++p, ++d) *d = *p;
    return d;
}
void* CHandleBasedMemoryPool::GetPointerFromHandle(long handle) { logf("GetPointerFromHandle(%ld)", handle); return g_block; }
bool CMovableResourceMemoryPool::AllocateResource(long* handle, void* owner, unsigned long size, unsigned long alignment)
{
    logf("AllocateResource(%ld,%ld)", (long)size, (long)alignment);
    *handle = 77;
    return g_allocOk;
}

int main()
{
    CPlane planes[3];
    for (int i = 0; i < 3; ++i) { planes[i].Normal.X = (float)i; planes[i].Normal.Y = 2.0f * i; planes[i].Normal.Z = 3.0f * i; planes[i].Distance = 10.0f + i; }
    CMovableResourceMemoryPool pool;
    CContainmentVolume_Methods* volume = (CContainmentVolume_Methods*)calloc(1, sizeof(CContainmentVolume_Methods));

    // 1. no planes: true, nothing allocated
    g_log[0] = 0;
    CHECK(volume->Initialise(planes, 0, &pool) == true);
    CHECK(volume->Size == 0 && g_log[0] == 0);

    // 2. pool allocation succeeds: block from the handle, planes copied
    g_log[0] = 0; g_allocOk = true;
    CHECK(volume->Initialise(planes, 3, &pool) == true);
    CHECK(volume->Size == 3 && volume->MemoryPool == (::CMovableResourceMemoryPool*)&pool);
    CHECK(strcmp(g_log, "AllocateResource(48,0);GetPointerFromHandle(77);CopyPlanes(3);") == 0);
    CHECK(volume->ContainmentPlanes == (::CPlane*)g_block && g_block[2].Distance == 12.0f && g_block[1].Normal.Z == 3.0f);

    // 3. pool allocation fails: false, no copy
    g_log[0] = 0; g_allocOk = false;
    CHECK(volume->Initialise(planes, 2, &pool) == false);
    CHECK(strcmp(g_log, "AllocateResource(32,0);") == 0);

    // 4. oversized request: rejected before the pool is asked
    g_log[0] = 0; g_allocOk = true;
    CHECK(volume->Initialise(planes, 0x08000000UL, &pool) == false);
    CHECK(g_log[0] == 0);

    // 5. no pool: new CPlane[count] (default ctor per plane), copied
    g_log[0] = 0;
    CHECK(volume->Initialise(planes, 2, 0) == true);
    CHECK(volume->MemoryPool == 0);
    CHECK(strcmp(g_log, "CPlane();CPlane();CopyPlanes(2);") == 0);
    CHECK(volume->ContainmentPlanes != 0 && ((CPlane*)volume->ContainmentPlanes)[1].Distance == 11.0f);

    if (fails == 0) printf("00a755c0_TEST PASS\n"); else printf("00a755c0_TEST FAIL\n");
    return fails != 0;
}