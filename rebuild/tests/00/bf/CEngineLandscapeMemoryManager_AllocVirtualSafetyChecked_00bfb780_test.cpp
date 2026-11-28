#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FABLE_PORTABLE
#ifndef _MSC_VER
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
#define __int64 long long
enum EIndexBufferFormat { TestIndexFormat };
enum EPrimitiveType { TestPrimitiveType };
#endif
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_REPEATED_MESH_ALLOCATIONS_PASS30
#define FABLE_RETAIL_PASS22_REPEATED_MESH_CLIP_SIMD
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/RepeatedStaticMeshSubsection.h"
#include "engine/C4DVector.h"

static void require(bool condition, const char* message)
{
    if (!condition) { fprintf(stderr, "%s\n", message); exit(1); }
}

CFailedAllocationHandler::~CFailedAllocationHandler() {}
bool CMovableResource::CanRelocateData(long) const { return true; }
unsigned long CMovableResource::Verify() const { return 123; }
class TestManager : public CEngineLandscapeMemoryManager {
public:
    bool HandleFailedAllocation(unsigned long) { return false; }
    void RelocateData(long, void*, void*, long) {}
};

struct Allocation {
    long size;
    long alignment;
    CMovableResource* owner;
    unsigned char bytes[1024];
};
static Allocation allocations[5];
static int callCount, failAt;
static void* record(long size, long alignment, CMovableResource* owner)
{
    require(callCount < 5, "unexpected extra allocation");
    Allocation& a = allocations[callCount++];
    a.size = size; a.alignment = alignment; a.owner = owner;
    memset(a.bytes, 0xcd, sizeof(a.bytes));
    return callCount == failAt ? 0 : a.bytes;
}
void* CEngineLandscapeMemoryManager::AllocVirtual(long size, CMovableResource* owner)
{
    return record(size, 0, owner);
}
void* CEngineLandscapeMemoryManager::AllocVirtualAligned(long size, long alignment, CMovableResource* owner)
{
    return record(size, alignment, owner);
}

typedef unsigned char Element;
int main()
{
    TestManager manager, owner;
    const long limit = 0x7fffffffL / sizeof(Element);
    const long samples[] = { -1, 0, 1, 9, limit, limit + (sizeof(Element) > 1 ? 1 : 0), 0x7fffffffL };
    for (unsigned i = 0; i < sizeof(samples)/sizeof(samples[0]); ++i) {
        const long count = samples[i];
        for (failAt = 0; failAt <= 1; ++failAt) {
            callCount = 0;
            Element* result = manager.AllocArrayOfObjectsNoConstuctors<Element>(count, &owner);
            if (count < 0 || count > limit) {
                require(result == 0 && callCount == 0, "rejected count reached the allocator");
            } else {
                require(callCount == 1 && allocations[0].owner == &owner, "allocation owner or count");
                require(allocations[0].size == count * (long)sizeof(Element), "allocation byte count");
                require(allocations[0].alignment == 0, "allocation alignment");
                require((void*)result == (failAt ? 0 : allocations[0].bytes), "allocator return");
            }
        }
    }
    puts("00bfb780_TEST PASS");
    return 0;
}
