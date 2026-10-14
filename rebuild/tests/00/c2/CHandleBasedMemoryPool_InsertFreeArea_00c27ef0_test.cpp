#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SFreeArea { void* m_pResource; unsigned char _pad04[4]; long m_size; short m_padding; unsigned char m_state; unsigned char _pad0f; SFreeArea** m_pPrevLink; SFreeArea* m_pNext; };
struct CHandleBasedMemoryPool { unsigned char _pad[0x10]; long m_usedBytes; long m_freeCount; unsigned char _pad18[4]; long m_largestFreeArea; unsigned char _pad20[0x1c]; SFreeArea* m_freeLists[256];
    void InsertFreeArea(SFreeArea* area) { area->m_size += area->m_padding; long size = area->m_size; unsigned long bucket = (unsigned long)size >> 9; area->m_padding = 0; if (bucket >= 0xff) bucket = 0xff;
        SFreeArea** head = &m_freeLists[bucket]; area->m_pPrevLink = head; SFreeArea* old = *head; area->m_pNext = old; if (old) old->m_pPrevLink = &area->m_pNext; *head = area; if (m_largestFreeArea < area->m_size) m_largestFreeArea = area->m_size; }
    SFreeArea* FindLargestFreeArea() { long size = m_largestFreeArea; unsigned long bucket = (unsigned long)size >> 9; if (bucket >= 0xff) bucket = 0xff; for (SFreeArea* area = m_freeLists[bucket]; area; area = area->m_pNext) { if (area->m_size == size) return area; } return 0; }
    void UpdateLargestFreeArea() { unsigned long cap = (unsigned long)m_largestFreeArea >> 9; if (cap >= 0xff) cap = 0xff; for (long bucket = (long)cap; bucket >= 0; --bucket) { if (m_freeLists[bucket]) { long largest = 0; for (SFreeArea* area = m_freeLists[bucket]; area; area = area->m_pNext) { if (largest < area->m_size) largest = area->m_size; } m_largestFreeArea = largest; return; } } m_largestFreeArea = 0; } };
static CHandleBasedMemoryPool g_pool; static SFreeArea g_areas[4];
static void reset() { std::memset(&g_pool, 0, sizeof g_pool); std::memset(g_areas, 0, sizeof g_areas); }

int main() {
    reset(); g_areas[0].m_size = 1000; g_areas[0].m_padding = 24; g_pool.InsertFreeArea(&g_areas[0]);
    if (g_areas[0].m_size != 1024 || g_areas[0].m_padding != 0 || g_pool.m_freeLists[2] != &g_areas[0] || g_areas[0].m_pPrevLink != &g_pool.m_freeLists[2] || g_areas[0].m_pNext != 0 || g_pool.m_largestFreeArea != 1024) { std::printf("00c27ef0_TEST FAIL\n"); return 1; }
    g_areas[1].m_size = 1100; g_pool.InsertFreeArea(&g_areas[1]); if (g_pool.m_freeLists[2] != &g_areas[1] || g_areas[1].m_pNext != &g_areas[0] || g_areas[0].m_pPrevLink != &g_areas[1].m_pNext || g_pool.m_largestFreeArea != 1100) { std::printf("00c27ef0_TEST FAIL\n"); return 1; }
    g_areas[2].m_size = 0x100000; g_pool.InsertFreeArea(&g_areas[2]); if (g_pool.m_freeLists[255] != &g_areas[2] || g_pool.m_largestFreeArea != 0x100000) { std::printf("00c27ef0_TEST FAIL\n"); return 1; } g_areas[3].m_size = 5; g_pool.InsertFreeArea(&g_areas[3]); if (g_pool.m_freeLists[0] != &g_areas[3] || g_pool.m_largestFreeArea != 0x100000) { std::printf("00c27ef0_TEST FAIL\n"); return 1; }
    std::printf("00c27ef0_TEST PASS\n"); return 0;
}