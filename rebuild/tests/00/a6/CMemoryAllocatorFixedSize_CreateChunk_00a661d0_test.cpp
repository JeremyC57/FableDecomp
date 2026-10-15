// Behaviour test for 0x00A661D0 CMemoryAllocatorFixedSize::CreateChunk: alignment of the header and the free chain threaded last-to-first.
#include <cstdio>
#include <cstddef>
struct CMemoryArea { unsigned char Index, NextIndex, PrevIndex, Allocated; };
struct CMemoryAreaChunk { void* ThisChunkMem; long NoAllocatedAreas; CMemoryAreaChunk* PNextChunk; CMemoryAreaChunk* PPreviousChunk; CMemoryArea* PFirstFreeArea; CMemoryArea* PFirstAllocatedArea; };
struct A {
    size_t Alignment, AlignmentShift, StatAreaSize, ChunkHeaderSize, NoAreasPerChunk;
    CMemoryAreaChunk* CreateChunk(void* mem)
    {
        CMemoryAreaChunk* chunk = (CMemoryAreaChunk*)((((size_t)mem + Alignment - 1) >> AlignmentShift) << AlignmentShift);
        chunk->ThisChunkMem = mem; chunk->PNextChunk = 0; chunk->PPreviousChunk = 0; chunk->PFirstFreeArea = 0; chunk->PFirstAllocatedArea = 0; chunk->NoAllocatedAreas = 0;
        CMemoryArea* area = (CMemoryArea*)((unsigned char*)chunk + ChunkHeaderSize);
        for (size_t i = 0; i < NoAreasPerChunk; ++i) {
            area->NextIndex = 0xff; area->PrevIndex = 0xff; area->Index = (unsigned char)i; area->Allocated = 0;
            if (chunk->PFirstFreeArea == 0) area->NextIndex = 0xff; else area->NextIndex = chunk->PFirstFreeArea->Index;
            if (chunk->PFirstFreeArea) chunk->PFirstFreeArea->PrevIndex = (unsigned char)i;
            chunk->PFirstFreeArea = area;
            area = (CMemoryArea*)((unsigned char*)area + StatAreaSize);
        }
        return chunk;
    }
};
static unsigned char g_mem[1024];
int main()
{
    A a; a.Alignment = 64; a.AlignmentShift = 6; a.StatAreaSize = 16; a.ChunkHeaderSize = 64; a.NoAreasPerChunk = 3;
    void* mem = g_mem + 3;
    CMemoryAreaChunk* c = a.CreateChunk(mem);
    size_t addr = (size_t)c;
    CMemoryArea* a0 = (CMemoryArea*)((unsigned char*)c + 64);
    CMemoryArea* a2 = (CMemoryArea*)((unsigned char*)c + 64 + 32);
    int bad = !(addr % 64 == 0 && addr >= (size_t)mem && addr - (size_t)mem < 64 && c->ThisChunkMem == mem);
    bad |= !(c->PFirstFreeArea == a2 && a2->Index == 2 && a2->NextIndex == 1 && a2->PrevIndex == 0xff);
    bad |= !(a0->Index == 0 && a0->NextIndex == 0xff && a0->PrevIndex == 1 && a0->Allocated == 0);
    if (bad) { std::printf("00a661d0_TEST FAIL\n"); return 1; }
    std::printf("00a661d0_TEST PASS\n");
    return 0;
}