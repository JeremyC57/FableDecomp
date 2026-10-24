// behaviour test for 00c27e60: CHandleBasedMemoryPool::ReleaseBuffer -- frees the unaligned buffer
// and clears buffer/size only when one exists; the statistics are always reset.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
static void PoolFree(void* block) { ++g_frees; std::free(block); }
struct CHandleBasedMemoryPool {
    void* Buffer;
    void* UnalignedBuffer;
    long PoolSize;
    long AllocatedSize;
    long NumAllocations;
    long LockCount;
    long LargestFreeArea;
    unsigned long MemoryFlags;
    void ReleaseBuffer()
    {
        if (this->UnalignedBuffer) {
            PoolFree(this->UnalignedBuffer);
            this->UnalignedBuffer = 0;
            this->Buffer = 0;
            this->PoolSize = 0;
        }
        this->AllocatedSize = 0;
        this->NumAllocations = 0;
        this->LockCount = 0;
        this->LargestFreeArea = 0;
    }
};
int main()
{
    int ok = 1;
    CHandleBasedMemoryPool pool;
    pool.UnalignedBuffer = std::malloc(64);
    pool.Buffer = (char*)pool.UnalignedBuffer + 8;
    pool.PoolSize = 48;
    pool.AllocatedSize = 16;
    pool.NumAllocations = 2;
    pool.LockCount = 1;
    pool.LargestFreeArea = 32;
    pool.MemoryFlags = 7;
    pool.ReleaseBuffer();
    if (pool.UnalignedBuffer || pool.Buffer || pool.PoolSize || pool.AllocatedSize || pool.NumAllocations || pool.LockCount || pool.LargestFreeArea || g_frees != 1 || pool.MemoryFlags != 7) ok = 0;
    char fake[4];
    pool.Buffer = fake;
    pool.PoolSize = 4;
    pool.NumAllocations = 3;
    pool.ReleaseBuffer();
    if (pool.Buffer != fake || pool.PoolSize != 4 || pool.NumAllocations != 0 || g_frees != 1) ok = 0;
    if (!ok) { std::printf("00c27e60_TEST FAIL\n"); return 1; }
    std::printf("00c27e60_TEST PASS\n");
    return 0;
}