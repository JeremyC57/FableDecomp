// Behaviour test for 0x0122A9A0 the atexit destructor of CMemoryAllocator::Deallocator: with a live allocator, flag
// shutdown, and destroy it (dtor, page release, pointer cleared) only when nothing is left allocated.
// NF145: on engine/CMemoryAllocator.h (the same opt-ins as the row); the row body is repeated below. The allocator storage is
// committed with VirtualAlloc so the header's operator delete (VirtualFree MEM_RELEASE) really releases it; the host build
// stubs VirtualFree.
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __declspec(x)
#define FABLE_RETAIL_CMEMORYALLOCATOR_SAVEBINARY_HOST_PASS23   // the 0x49c size check is for the 32-bit layout
#endif
#define FABLE_RETAIL_CMEMORYALLOCATOR_CTOR
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATOR
#define protected public
#include "engine/CMemoryAllocator.h"
#undef protected

static int gAllocatorDtors = 0;
CMemoryAllocator* CMemoryAllocator::PMemoryAllocator = 0;
bool CMemoryAllocator::DestroyWhenEmpty = false;
CMemoryAllocator::~CMemoryAllocator() { ++gAllocatorDtors; }

#ifdef _MSC_VER
struct TestMemoryInfo { void* BaseAddress; void* AllocationBase; unsigned long AllocationProtect; unsigned long RegionSize;
                        unsigned long State; unsigned long Protect; unsigned long Type; };   // MEMORY_BASIC_INFORMATION (x86)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void* address, unsigned long size, unsigned long type, unsigned long protect);
extern "C" __declspec(dllimport) unsigned long __stdcall VirtualQuery(const void* address, TestMemoryInfo* info, unsigned long length);
static void* NewAllocatorStorage() { return VirtualAlloc(0, sizeof(CMemoryAllocator), 0x3000 /* MEM_COMMIT|MEM_RESERVE */, 4 /* PAGE_READWRITE */); }
static bool Released(void* p) { TestMemoryInfo info; VirtualQuery(p, &info, sizeof(info)); return info.State == 0x10000; /* MEM_FREE */ }
#else
#include <stdlib.h>
static void* gReleased = 0;
extern "C" int VirtualFree(void* address, unsigned long, unsigned long) { gReleased = address; free(address); return 1; }
static void* NewAllocatorStorage() { return calloc(1, sizeof(CMemoryAllocator)); }
static bool Released(void* p) { return gReleased == p; }
#endif

// The row body (0x0122A9A0).
CMemoryAllocator::CDeallocator::~CDeallocator()
{
    if (PMemoryAllocator) {
        DestroyWhenEmpty = true;
        if (PMemoryAllocator->NoAllocations == 0)
            Destroy();
    }
}

int main()
{
    bool ok = true;
    { CMemoryAllocator::CDeallocator d; }                 // no allocator: nothing happens
    ok = ok && !CMemoryAllocator::DestroyWhenEmpty && gAllocatorDtors == 0;
    CMemoryAllocator* a = (CMemoryAllocator*)NewAllocatorStorage();
    ok = ok && a != 0;
    if (!ok) { printf("0122a9a0_TEST FAIL\n"); return 1; }
    a->NoAllocations = 2;
    CMemoryAllocator::PMemoryAllocator = a;
    { CMemoryAllocator::CDeallocator d; }                 // blocks still allocated: flag only
    ok = ok && CMemoryAllocator::DestroyWhenEmpty && gAllocatorDtors == 0;
    ok = ok && CMemoryAllocator::PMemoryAllocator == a && !Released(a);
    CMemoryAllocator::DestroyWhenEmpty = false;
    a->NoAllocations = 0;
    { CMemoryAllocator::CDeallocator d; }                 // last one: destroy
    ok = ok && CMemoryAllocator::DestroyWhenEmpty && gAllocatorDtors == 1;
    ok = ok && Released(a) && CMemoryAllocator::PMemoryAllocator == 0;
    if (ok) { printf("0122a9a0_TEST PASS\n"); return 0; }
    printf("0122a9a0_TEST FAIL\n");
    return 1;
}
