#include <cstdio>
// Behaviour test for CMemoryAllocatorVariableSize::GetAllocatedArea (0x00A652D0).
unsigned long g_shift, g_align;
struct Area { Area* PNextArea; };
struct Chunk { Chunk* PNextChunk; Area* PFirstAllocatedArea; };
struct Alloc {
    unsigned long Alignment, AlignmentShift; Chunk* PFirstChunk;
    void* GetAllocatedArea(long index)
    {
        g_shift = AlignmentShift; g_align = Alignment;
        long count = 0;
        for (Chunk* c = PFirstChunk; c; c = c->PNextChunk)
            for (Area* a = c->PFirstAllocatedArea; a; a = a->PNextArea) {
                if (count == index) return (char*)a + (((g_align + 0x1f) >> g_shift) << g_shift);
                ++count;
            }
        return 0;
    }
};
int main()
{
    Area a3 = { 0 }, a2 = { 0 }, a1 = { &a2 };
    Chunk c2 = { 0, &a3 }, cempty = { &c2, 0 }, c1 = { &cempty, &a1 };
    Alloc al = { 0x24, 4, &c1 };
    bool ok = al.GetAllocatedArea(0) == (char*)&a1 + 0x40 && al.GetAllocatedArea(1) == (char*)&a2 + 0x40
           && al.GetAllocatedArea(2) == (char*)&a3 + 0x40 && al.GetAllocatedArea(3) == 0 && g_shift == 4 && g_align == 0x24;
    al.AlignmentShift = 5; al.Alignment = 0x10;
    ok = ok && al.GetAllocatedArea(1) == (char*)&a2 + 0x20;
    std::printf(ok ? "00a652d0_TEST PASS\n" : "00a652d0_TEST FAIL\n");
    return ok ? 0 : 1;
}