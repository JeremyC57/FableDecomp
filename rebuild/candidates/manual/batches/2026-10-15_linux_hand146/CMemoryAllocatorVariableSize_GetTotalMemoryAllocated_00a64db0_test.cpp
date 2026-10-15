// Behaviour test for 0x00A64DB0 CMemoryAllocatorVariableSize::GetTotalMemoryAllocated over two chunks.
#include <cstdio>
static unsigned int g_align = 0, g_shift = 0;
struct Area { Area* PNext; unsigned int Size; };
struct Chunk { int NoAllocatedAreas; Chunk* PNextChunk; Area* PFirstAllocatedArea; };
static unsigned int AlignSize(unsigned int size) { return ((size + g_align - 1) >> g_shift) << g_shift; }
struct V {
    unsigned int Alignment, AlignmentShift; Chunk* PFirstChunk;
    unsigned int GetTotalMemoryAllocated()
    {
        g_shift = AlignmentShift; g_align = Alignment;
        unsigned int total = 0;
        for (Chunk* c = PFirstChunk; c; c = c->PNextChunk) {
            unsigned int data = 0;
            for (Area* a = c->PFirstAllocatedArea; a; a = a->PNext) data += a->Size;
            total += c->NoAllocatedAreas * AlignSize(0x20) + AlignSize(0x34) + data;
        }
        return total;
    }
};
int main()
{
    Area a2 = { 0, 100 }, a1 = { &a2, 50 }, b1 = { 0, 7 };
    Chunk c2 = { 1, 0, &b1 }, c1 = { 2, &c2, &a1 };
    V v; v.Alignment = 16; v.AlignmentShift = 4; v.PFirstChunk = &c1;
    // chunk1: 2*32 + 64 + 150 = 278; chunk2: 32 + 64 + 7 = 103
    if (v.GetTotalMemoryAllocated() != 381 || g_align != 16) { std::printf("00a64db0_TEST FAIL\n"); return 1; }
    v.PFirstChunk = 0;
    if (v.GetTotalMemoryAllocated() != 0) { std::printf("00a64db0_TEST FAIL\n"); return 1; }
    std::printf("00a64db0_TEST PASS\n");
    return 0;
}
