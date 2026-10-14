#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SChunk; struct SArea { unsigned char _pad[0x14]; SChunk* m_pChunk; }; static void* g_removed = 0; static SChunk* g_released = 0; static unsigned long g_counter = 0, g_usage = 0;
struct SChunk { unsigned char _pad[8]; long m_noAreas; unsigned char _p[0x10]; bool m_pinned; void RemoveAreaFromFreeList(SArea* a) { g_removed = a; } };
struct CAlloc { virtual void v000() { } unsigned char _pad[0xc14 - sizeof(void*)]; unsigned long m_usage; unsigned long m_shift; void ReleaseChunk(SChunk* c) { g_released = c; }
    virtual void Deallocate(void* block) { unsigned long shift = m_shift; g_counter = shift; unsigned long usage = m_usage; g_usage = usage; SArea* area = (SArea*)((char*)block - (((usage + 0x1f) >> shift) << shift)); SChunk* chunk = area->m_pChunk; chunk->RemoveAreaFromFreeList(area); if (chunk->m_noAreas == 0 && !chunk->m_pinned) ReleaseChunk(chunk); } };

int main() {
    CAlloc* al = new CAlloc; al->m_usage = 0x11; al->m_shift = 4; unsigned char buf[0x60]; SChunk ch; std::memset(&ch, 0, sizeof ch); ((SArea*)buf)->m_pChunk = &ch; ch.m_noAreas = 2;
    al->Deallocate(buf + 0x30); if (g_removed != buf || g_released != 0) { std::printf("00a65a50_TEST FAIL\n"); return 1; } ch.m_noAreas = 0; ch.m_pinned = true; al->Deallocate(buf + 0x30); if (g_released != 0) { std::printf("00a65a50_TEST FAIL\n"); return 1; } ch.m_pinned = false; al->Deallocate(buf + 0x30); if (g_released != &ch || g_counter != 4 || g_usage != 0x11) { std::printf("00a65a50_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a65a50_TEST PASS\n"); return 0;
}
