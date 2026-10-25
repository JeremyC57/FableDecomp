// Behaviour test for 00f38710: vector<CResourcePage>::_M_clear -- each page's ResourceList block freed (null skipped), then the page array freed
#include <cstdio>
#include <cstdlib>
static int g_freed = 0;
static void Free(void* p) { if (p) { ++g_freed; std::free(p); } }
struct LongVec { long* s; long* f; long* e; ~LongVec() { Free(s); } };
struct Page { long ScratchPool; LongVec ResourceList; unsigned char* Next; long FreeSpace; bool AutoRestore; };
struct PageVec {
    Page* start; Page* finish; Page* end;
    void Clear() { for (Page* p = start; p != finish; ++p) p->~Page(); Free(start); }
};
int main()
{
    PageVec v; v.start = (Page*)std::malloc(4 * sizeof(Page)); v.finish = v.start + 3; v.end = v.start + 4;
    for (int i = 0; i < 3; ++i) { v.start[i].ResourceList.s = i == 1 ? 0 : (long*)std::malloc(8); }
    v.Clear();
    int ok = sizeof(Page) == 0x1c && g_freed == 3;
    PageVec e; e.start = e.finish = e.end = 0; e.Clear(); ok = ok && g_freed == 3;
    std::printf(ok ? "00f38710_TEST PASS\n" : "00f38710_TEST FAIL\n");
    return ok ? 0 : 1;
}
