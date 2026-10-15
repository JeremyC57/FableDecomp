// Behaviour test for 0x00A66160 CMemoryAllocatorFixedSize::DeleteChunk: unlink from a 3-chunk list (head, middle, tail) and free the block.
#include <cstdio>
static void* g_freed = 0;
static void HeapFreeStub(void* mem) { g_freed = mem; }
struct CMemoryAreaChunk { void* ThisChunkMem; long NoAllocatedAreas; CMemoryAreaChunk* PNextChunk; CMemoryAreaChunk* PPreviousChunk; };
struct CMemoryAllocatorFixedSize {
    CMemoryAreaChunk* PFirstChunk; CMemoryAreaChunk* PLastChunk; CMemoryAreaChunk* PCurrentAllocatorChunk;
    void DeleteChunk(CMemoryAreaChunk* chunk)
    {
        if (PCurrentAllocatorChunk == chunk) PCurrentAllocatorChunk = 0;
        if (chunk->PNextChunk) chunk->PNextChunk->PPreviousChunk = chunk->PPreviousChunk;
        if (chunk->PPreviousChunk) chunk->PPreviousChunk->PNextChunk = chunk->PNextChunk;
        if (PFirstChunk == chunk) PFirstChunk = chunk->PNextChunk;
        if (PLastChunk == chunk) PLastChunk = chunk->PPreviousChunk;
        HeapFreeStub(chunk->ThisChunkMem);
    }
};
int main()
{
    int m0, m1, m2;
    CMemoryAreaChunk a = { &m0, 0, 0, 0 }, b = { &m1, 0, 0, 0 }, c = { &m2, 0, 0, 0 };
    a.PNextChunk = &b; b.PPreviousChunk = &a; b.PNextChunk = &c; c.PPreviousChunk = &b;
    CMemoryAllocatorFixedSize al = { &a, &c, &b };
    al.DeleteChunk(&b);
    int bad = !(a.PNextChunk == &c && c.PPreviousChunk == &a && al.PCurrentAllocatorChunk == 0 && g_freed == &m1);
    al.DeleteChunk(&a);
    bad |= !(al.PFirstChunk == &c && c.PPreviousChunk == 0 && g_freed == &m0);
    al.DeleteChunk(&c);
    bad |= !(al.PFirstChunk == 0 && al.PLastChunk == 0 && g_freed == &m2);
    if (bad) { std::printf("00a66160_TEST FAIL\n"); return 1; }
    std::printf("00a66160_TEST PASS\n");
    return 0;
}