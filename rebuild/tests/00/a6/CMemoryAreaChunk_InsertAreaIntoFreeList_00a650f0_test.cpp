#include <cstdio>
struct CMemoryAreaChunk;
struct CMemoryArea {
    CMemoryArea*      PNextArea;                // +0x00
    CMemoryArea*      PRightChild;              // +0x04
    CMemoryArea*      PPreviousArea;            // +0x08
    CMemoryArea*      PParentArea;              // +0x0c
    CMemoryArea*      PLeftChild;               // +0x10
    CMemoryAreaChunk* PParentChunk;             // +0x14
    long              DataSize;                 // +0x18
    unsigned char     Colour;                   // +0x1c
    unsigned char     ID;                       // +0x1d
};
class CMemoryAllocatorVariableSize { public:
    CMemoryArea*  PTreeRoot;                    // +0xc34
    int calls; CMemoryArea* lastRoot; CMemoryArea* lastArea;
    void InsertIntoTree(CMemoryArea* root, CMemoryArea* area) { ++calls; lastRoot = root; lastArea = area; }
};
struct CMemoryAreaChunk {
    long                          TotalDataSize;        // +0x00
    long                          NoFreeAreas;          // +0x04
    long                          NoAllocatedAreas;     // +0x08
    long                          FreeDataSize;         // +0x0c
    long                          LargestFreeAreaSize;  // +0x10
    long                          SmallestFreeAreaSize; // +0x14
    CMemoryAllocatorVariableSize* PParentAllocator;     // +0x18
    bool                          Permanent;            // +0x1c
    bool                          DemandPaged;          // +0x1d
    CMemoryAreaChunk*             PNextChunk;           // +0x20
    CMemoryAreaChunk*             PPreviousChunk;       // +0x24
    CMemoryArea*                  PFirstFreeArea;       // +0x28
    CMemoryArea*                  PLastFreeArea;        // +0x2c
    CMemoryArea*                  PFirstAllocatedArea;  // +0x30
    void InsertAreaIntoFreeList(CMemoryArea* area);
};

void CMemoryAreaChunk::InsertAreaIntoFreeList(CMemoryArea* area)
{
    CMemoryArea* p = PFirstFreeArea;
    if (!p) {
        area->PNextArea = 0;
        area->PPreviousArea = 0;
        PFirstFreeArea = area;
        PLastFreeArea = area;
    }
    else {
        while (p) {
            if (p->DataSize > area->DataSize) {
                area->PNextArea = p;
                area->PPreviousArea = p->PPreviousArea;
                if (p->PPreviousArea) {
                    p->PPreviousArea->PNextArea = area;
                    p->PPreviousArea = area;
                }
                else {
                    PFirstFreeArea = area;
                    p->PPreviousArea = area;
                }
                break;
            }
            if (!p->PNextArea) {
                area->PNextArea = 0;
                area->PPreviousArea = p;
                p->PNextArea = area;
                PLastFreeArea = area;
                break;
            }
            p = p->PNextArea;
        }
    }
    PParentAllocator->InsertIntoTree(PParentAllocator->PTreeRoot, area);
    FreeDataSize += area->DataSize;
    if (PLastFreeArea)
        LargestFreeAreaSize = PLastFreeArea->DataSize;
    else
        LargestFreeAreaSize = 0;
    if (PFirstFreeArea)
        SmallestFreeAreaSize = PFirstFreeArea->DataSize;
    else
        SmallestFreeAreaSize = 0;
    ++NoFreeAreas;
}

static int fail() { std::printf("00a650f0_TEST FAIL\n"); return 1; }
int main()
{
    CMemoryAllocatorVariableSize alloc; alloc.calls = 0; CMemoryArea rootArea; alloc.PTreeRoot = &rootArea;
    CMemoryAreaChunk c; c.PFirstFreeArea = 0; c.PLastFreeArea = 0; c.FreeDataSize = 100; c.NoFreeAreas = 0; c.PParentAllocator = &alloc;
    c.LargestFreeAreaSize = c.SmallestFreeAreaSize = -1;
    CMemoryArea a[4]; long sizes[4] = { 50, 20, 80, 50 };
    for (int i = 0; i < 4; ++i) { a[i].DataSize = sizes[i]; a[i].PNextArea = (CMemoryArea*)1; a[i].PPreviousArea = (CMemoryArea*)1; }
    c.InsertAreaIntoFreeList(&a[0]);
    if (c.PFirstFreeArea != &a[0] || c.PLastFreeArea != &a[0] || a[0].PNextArea || a[0].PPreviousArea || c.LargestFreeAreaSize != 50 || c.SmallestFreeAreaSize != 50 || c.FreeDataSize != 150 || c.NoFreeAreas != 1) return fail();
    c.InsertAreaIntoFreeList(&a[1]);   // 20: new first
    c.InsertAreaIntoFreeList(&a[2]);   // 80: new last
    c.InsertAreaIntoFreeList(&a[3]);   // 50: after a[0] (equal sizes go after), before a[2]
    CMemoryArea* order[4] = { &a[1], &a[0], &a[3], &a[2] };
    CMemoryArea* p = c.PFirstFreeArea;
    for (int i = 0; i < 4; ++i, p = p->PNextArea) { if (p != order[i]) return fail(); if (i && p->PPreviousArea != order[i - 1]) return fail(); }
    if (p != 0 || c.PLastFreeArea != &a[2] || c.SmallestFreeAreaSize != 20 || c.LargestFreeAreaSize != 80 || c.NoFreeAreas != 4 || c.FreeDataSize != 300) return fail();
    if (alloc.calls != 4 || alloc.lastRoot != &rootArea || alloc.lastArea != &a[3]) return fail();
    std::printf("00a650f0_TEST PASS\n");
    return 0;
}