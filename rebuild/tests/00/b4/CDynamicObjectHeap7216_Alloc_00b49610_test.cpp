// behaviour test for 0x00B49610 CDynamicObjectHeap<72,16>::Alloc(long) (model copy; CreateMoreBlocks pushes one entry)
#include <cstdio>
struct Entry { Entry* NextEntry; };
static Entry g_more;
struct Heap {
    long AllocCount; Entry* FreeList; bool AllowHeapToGrow;
    void CreateMoreBlocks() { g_more.NextEntry = 0; FreeList = &g_more; }
    void* Alloc(long)
    {
        if (FreeList == 0) {
            if (AllowHeapToGrow)
                CreateMoreBlocks();
            else
                return 0;
        }
        ++AllocCount;
        Entry* entry = FreeList;
        FreeList = entry->NextEntry;
        return entry;
    }
};
int main()
{
    Entry e2 = { 0 }, e1 = { &e2 };
    Heap h = { 0, &e1, false };
    int ok = h.Alloc(72) == &e1 && h.Alloc(72) == &e2 && h.Alloc(72) == 0 && h.AllocCount == 2;
    h.AllowHeapToGrow = true;
    ok = ok && h.Alloc(72) == &g_more && h.AllocCount == 3 && h.FreeList == 0;
    if (!ok) { std::printf("00b49610_TEST FAIL\n"); return 1; }
    std::printf("00b49610_TEST PASS\n");
    return 0;
}