// behaviour test for 0x00AF8860 CDynamicObjectHeap<156,2100>::Release() (model copy of the loop over a 3-block chain)
#include <cstdio>
struct Block { Block* NextEntry; };
static int g_deleted = 0;
struct Heap {
    void* FreeList; Block Start;
    void Release()
    {
        while (Start.NextEntry) {
            Block* next = Start.NextEntry->NextEntry;
            delete Start.NextEntry; ++g_deleted;
            Start.NextEntry = next;
        }
        FreeList = 0;
    }
};
int main()
{
    Heap h; h.FreeList = &h; h.Start.NextEntry = 0;
    for (int i = 0; i < 3; ++i) { Block* b = new Block; b->NextEntry = h.Start.NextEntry; h.Start.NextEntry = b; }
    h.Release();
    int ok = g_deleted == 3 && h.Start.NextEntry == 0 && h.FreeList == 0;
    if (!ok) { std::printf("00af8860_TEST FAIL\n"); return 1; }
    std::printf("00af8860_TEST PASS\n");
    return 0;
}