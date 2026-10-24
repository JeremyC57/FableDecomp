// Behaviour test for 0x00A656E0 CMemoryAllocatorVariableSize::AllocateMemoryFromExistingChunks(unsigned long)
#include <cstdio>

static long g_requested = -1;
static long g_allocated = -1;

class CMemoryAllocatorVariableSize {
public:
    class CMemoryArea;
    class CMemoryAreaChunk {
    public:
        CMemoryArea* AllocAreaFromArea(CMemoryArea* area, long size) { g_allocated = size; return area; }   // 0x00A65480 stand-in
    };
    class CMemoryArea {
    public:
        CMemoryArea*      PNextArea;
        CMemoryArea*      PRightChild;
        CMemoryArea*      PPreviousArea;
        CMemoryArea*      PParentArea;
        CMemoryArea*      PLeftChild;
        CMemoryAreaChunk* PParentChunk;
        long              DataSize;
        unsigned char     Colour;
        unsigned char     ID;
    };

    unsigned long Alignment;
    unsigned long AlignmentShift;
    CMemoryArea*  PTreeRoot;                            // the one free area the stand-in tree holds

    unsigned long Align(unsigned long size) const { return (size + Alignment - 1) >> AlignmentShift << AlignmentShift; }
    CMemoryArea* GetTreeAreaWithSizeGreaterThanOrEqualTo(long size) const   // 0x00A64C10 stand-in
    {
        g_requested = size;
        return (PTreeRoot && PTreeRoot->DataSize >= size) ? PTreeRoot : 0;
    }
    CMemoryArea* AllocateMemoryFromExistingChunks(unsigned long size)
    {
        size = Align(size);
        CMemoryArea* area = GetTreeAreaWithSizeGreaterThanOrEqualTo(size);
        if (area)
            return area->PParentChunk->AllocAreaFromArea(area, size);
        return 0;
    }
};
typedef CMemoryAllocatorVariableSize::CMemoryArea CArea;

int main()
{
    CMemoryAllocatorVariableSize::CMemoryAreaChunk chunk;
    CArea area;
    area.DataSize = 64;
    area.PParentChunk = &chunk;
    CMemoryAllocatorVariableSize a;
    a.Alignment = 16;
    a.AlignmentShift = 4;
    a.PTreeRoot = &area;
    CArea* r1 = a.AllocateMemoryFromExistingChunks(17);    // rounds up to 32
    bool ok = r1 == &area && g_requested == 32 && g_allocated == 32;
    g_allocated = -1;
    CArea* r2 = a.AllocateMemoryFromExistingChunks(65);    // 80 > 64: no area, no allocation
    ok = ok && r2 == 0 && g_requested == 80 && g_allocated == -1;
    CArea* r3 = a.AllocateMemoryFromExistingChunks(64);    // already aligned
    ok = ok && r3 == &area && g_requested == 64 && g_allocated == 64;
    if (!ok) { std::printf("00a656e0_TEST FAIL\n"); return 1; }
    std::printf("00a656e0_TEST PASS\n");
    return 0;
}