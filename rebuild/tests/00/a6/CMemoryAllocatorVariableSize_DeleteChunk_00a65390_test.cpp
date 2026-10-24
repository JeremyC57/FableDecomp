// Behaviour test for 0x00A65390 CMemoryAllocatorVariableSize::DeleteChunk(CMemoryAreaChunk*)
#include <cstdio>

struct MEMORYSTATUS {
    unsigned long dwLength, dwMemoryLoad, dwTotalPhys, dwAvailPhys, dwTotalPageFile, dwAvailPageFile, dwTotalVirtual,
        dwAvailVirtual;
};
static int g_statusCalls = 0;
static int g_freeCalls = 0;
static void* g_lastFreed = 0;
static void GlobalMemoryStatusStub(MEMORYSTATUS* status) { status->dwLength = sizeof(MEMORYSTATUS); ++g_statusCalls; }
static int VirtualFreeStub(void* address, unsigned long size, unsigned long freeType)
{
    if (size != 0 || freeType != 0x8000) return 0;
    g_lastFreed = address;
    ++g_freeCalls;
    return 1;
}

static int g_removed = 0;

class CMemoryAllocatorVariableSize {
public:
    class CMemoryAreaChunk;
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
    class CMemoryAreaChunk {
    public:
        long              TotalDataSize;
        long              NoFreeAreas;
        long              NoAllocatedAreas;
        long              FreeDataSize;
        long              LargestFreeAreaSize;
        long              SmallestFreeAreaSize;
        CMemoryAllocatorVariableSize* PParentAllocator;
        bool              Permanent;
        bool              DemandPaged;
        CMemoryAreaChunk* PNextChunk;
        CMemoryAreaChunk* PPreviousChunk;
        CMemoryArea*      PFirstFreeArea;
        CMemoryArea*      PLastFreeArea;
        CMemoryArea*      PFirstAllocatedArea;
        bool IsDemandPaged() const { return DemandPaged; }
    };

    CMemoryAreaChunk* PFirstChunk;

    void RemoveFromTree(CMemoryArea*) { ++g_removed; }     // 0x00A64EF0 stand-in
    void DeleteChunk(CMemoryAreaChunk* chunk)
    {
        for (CMemoryArea* area = chunk->PFirstFreeArea; area; area = area->PNextArea)
            RemoveFromTree(area);
        if (chunk->PNextChunk)
            chunk->PNextChunk->PPreviousChunk = chunk->PPreviousChunk;
        if (chunk->PPreviousChunk)
            chunk->PPreviousChunk->PNextChunk = chunk->PNextChunk;
        if (PFirstChunk == chunk)
            PFirstChunk = chunk->PNextChunk;
        MEMORYSTATUS status;
        GlobalMemoryStatusStub(&status);
        if (!chunk->IsDemandPaged())
            VirtualFreeStub(chunk, 0, 0x8000);
        GlobalMemoryStatusStub(&status);
    }
};
typedef CMemoryAllocatorVariableSize::CMemoryAreaChunk CChunk;
typedef CMemoryAllocatorVariableSize::CMemoryArea CArea;

int main()
{
    CMemoryAllocatorVariableSize a;
    CChunk c[3];
    CArea areas[3];
    for (int i = 0; i < 3; ++i) {
        c[i].DemandPaged = false;
        c[i].PFirstFreeArea = 0;
        c[i].PPreviousChunk = i > 0 ? &c[i - 1] : 0;
        c[i].PNextChunk = i < 2 ? &c[i + 1] : 0;
        areas[i].PNextArea = i < 2 ? &areas[i + 1] : 0;
    }
    a.PFirstChunk = &c[0];
    c[1].PFirstFreeArea = &areas[0];                    // three free areas leave the tree
    a.DeleteChunk(&c[1]);
    bool ok = g_removed == 3 && c[0].PNextChunk == &c[2] && c[2].PPreviousChunk == &c[0] && a.PFirstChunk == &c[0] &&
              g_freeCalls == 1 && g_lastFreed == &c[1] && g_statusCalls == 2;
    c[0].DemandPaged = true;                            // the head chunk, demand paged: unlinked but not freed
    a.DeleteChunk(&c[0]);
    ok = ok && a.PFirstChunk == &c[2] && c[2].PPreviousChunk == 0 && g_freeCalls == 1 && g_statusCalls == 4;
    if (!ok) { std::printf("00a65390_TEST FAIL\n"); return 1; }
    std::printf("00a65390_TEST PASS\n");
    return 0;
}