// Behaviour test for 0x00A02D20 (see src/00a02d20.cpp): the pool constructor on a portable model.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00a02d20_TEST FAIL\n"); return 1; } } while (0)

static int g_baseCalls = 0, g_nameCalls = 0;
struct CBase { CBase() { ++g_baseCalls; } virtual ~CBase() { } };
class CCharString { public: void* m_pData; CCharString() : m_pData(0) { ++g_nameCalls; } };
struct CHandleInfoArray { void* m_pData; unsigned int m_size; unsigned int m_capacity; CHandleInfoArray() : m_pData(0), m_size(0), m_capacity(0) { } };
class CHandleBasedMemoryPool : public CBase { public:
    void* Buffer; void* UnalignedBuffer; int PoolSize; int AllocatedSize; int NumAllocations; int LockCount;
    int LargestFreeArea; unsigned int MemoryFlags; bool MultiThreaded; bool ShutdownFlag; bool EnableDefrag; bool EnableFullDefragIfPartialFails;
    unsigned int MaxFreeSizeBeforeDefrag; unsigned int MaxFreeSizeBeforeFailure; int MaxBlocksToTryOnPartialDefrag; void* FailureHandler;
    void* FreeLists[256]; CHandleInfoArray Handles; int FirstUnusedHandle; CCharString Name;
    CHandleBasedMemoryPool()
        : Buffer(0), UnalignedBuffer(0), PoolSize(0), AllocatedSize(0), NumAllocations(0), LockCount(0),
          MultiThreaded(false), ShutdownFlag(false), FailureHandler(0), FirstUnusedHandle(-1)
    {
        std::memset(FreeLists, 0, sizeof(FreeLists));
        EnableDefrag = true; EnableFullDefragIfPartialFails = true;
        MaxFreeSizeBeforeDefrag = 0x80000; MaxFreeSizeBeforeFailure = 0x100000; MaxBlocksToTryOnPartialDefrag = 16;
    }
};
int main()
{
    CHandleBasedMemoryPool* p = new CHandleBasedMemoryPool;
    CHECK(g_baseCalls == 1 && g_nameCalls == 1);
    CHECK(p->Buffer == 0 && p->UnalignedBuffer == 0 && p->PoolSize == 0 && p->AllocatedSize == 0 && p->NumAllocations == 0 && p->LockCount == 0);
    CHECK(!p->MultiThreaded && !p->ShutdownFlag && p->EnableDefrag && p->EnableFullDefragIfPartialFails);
    CHECK(p->MaxFreeSizeBeforeDefrag == 0x80000 && p->MaxFreeSizeBeforeFailure == 0x100000 && p->MaxBlocksToTryOnPartialDefrag == 16);
    CHECK(p->FailureHandler == 0 && p->Handles.m_pData == 0 && p->Handles.m_size == 0 && p->FirstUnusedHandle == -1);
    for (int i = 0; i < 256; ++i) CHECK(p->FreeLists[i] == 0);
    delete p;
    std::printf("00a02d20_TEST PASS\n");
    return 0;
}