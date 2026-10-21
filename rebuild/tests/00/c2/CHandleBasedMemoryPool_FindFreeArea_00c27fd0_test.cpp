// Behaviour test for 00c27fd0: src/00c27fd0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CHandleBasedMemoryPool::FindFreeArea(long size)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CHandleBasedMemoryPool::FindFreeArea(long size) -- best-fit search (second pool build): walk the free lists from the size's bucket (size >> 9, capped at 255) upwards and return the smallest free area that holds `size`, stopping early on an exact fit; null when none
// Leaf: FindFreeArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27FD0, length 116 bytes. No callees.
// This is the 0x00C27xxx-0x00C28xxx copy of the pool (FindLargestFreeArea 0x00C28050, InsertFreeArea 0x00C27EF0
// landed): its free lists sit at +0x3c (one word later than the 0x00A0xxxx build) and the area test has no
// flags byte. Shape: the bucket clamp is a MIN-style ternary that spells `size >> 9` twice (a macro expansion) as the
// loop index initialiser (out-of-line `mov ebx,0FFh ; jmp`); a named bucket local lets VC7.1 test the copy (edx) instead
// of the index (ebx) at the loop entry; best size 0x7fffffff in esi, result in eax re-tested at the outer loop head.
struct SFreeArea_00c27fd0 {                        // a pool area record (as landed with 0x00C28050)
    void*                m_pResource;              // +0x00
    unsigned char        _pad04[4];
    long                 m_size;                   // +0x08
    short                m_padding;                // +0x0c
    unsigned char        m_state;                  // +0x0e ('T' = taken, 'N' = free)
    unsigned char        _pad0f;
    SFreeArea_00c27fd0** m_pPrevLink;              // +0x10
    SFreeArea_00c27fd0*  m_pNext;                  // +0x14
};
struct CHandleBasedMemoryPool {
    unsigned char       _pad[0x3c];
    SFreeArea_00c27fd0* m_freeLists[256];          // +0x3c
    SFreeArea_00c27fd0* FindFreeArea(long size);
};

SFreeArea_00c27fd0* CHandleBasedMemoryPool::FindFreeArea(long size)
{
    SFreeArea_00c27fd0* best = 0;
    long bestSize = 0x7fffffff;
    for (unsigned long i = ((unsigned long)size >> 9) < 255 ? ((unsigned long)size >> 9) : 255; i < 256 && !best; ++i) {
        for (SFreeArea_00c27fd0* area = m_freeLists[i]; area; area = area->m_pNext) {
            if (size <= area->m_size && area->m_size < bestSize) {
                bestSize = area->m_size;
                best = area;
                if (bestSize == size)
                    break;
            }
        }
    }
    return best;
}

// ---- stubs and driver ----

static SFreeArea_00c27fd0 g_area[8];
static CHandleBasedMemoryPool g_pool;
static void Link(int bucket, int a, long size, int next)
{
    g_area[a].m_size = size; g_area[a].m_pNext = next >= 0 ? &g_area[next] : 0;
    if (bucket >= 0) g_pool.m_freeLists[bucket] = &g_area[a];
}
int main()
{
    memset(&g_pool, 0, sizeof g_pool);
    CHECK(g_pool.FindFreeArea(100) == 0);
    // bucket 0: sizes 300, 150, 120 -> best fit for 100 is 120
    Link(0, 0, 300, 1); Link(-1, 1, 150, 2); Link(-1, 2, 120, -1);
    CHECK(g_pool.FindFreeArea(100) == &g_area[2]);
    CHECK(g_pool.FindFreeArea(150) == &g_area[1]);      // exact fit stops the walk
    CHECK(g_pool.FindFreeArea(301) == 0);
    // bucket 3 (size 1536..2047) and a later bucket 5
    Link(3, 3, 1700, 4); Link(-1, 4, 1600, -1); Link(5, 5, 2600, -1);
    CHECK(g_pool.FindFreeArea(1550) == &g_area[4]);     // best in the first non-empty bucket wins
    CHECK(g_pool.FindFreeArea(1650) == &g_area[3]);
    CHECK(g_pool.FindFreeArea(1800) == &g_area[5]);     // nothing in bucket 3 fits: move on
    CHECK(g_pool.FindFreeArea(1000) == &g_area[4]);     // starts at bucket 1, finds bucket 3
    // huge sizes clamp to bucket 255
    Link(255, 6, 0x7ffffff0, -1);
    CHECK(g_pool.FindFreeArea(0x10000000) == &g_area[6]);
    CHECK(g_pool.FindFreeArea(0x7ffffff1) == 0);
    Link(255, 7, 0x7fffffff, -1);
    CHECK(g_pool.FindFreeArea(0x20000000) == 0);        // a size of 0x7fffffff is never better than the start value
    if (fails == 0) printf("00c27fd0_TEST PASS\n"); else printf("00c27fd0_TEST FAIL\n");
    return fails;
}