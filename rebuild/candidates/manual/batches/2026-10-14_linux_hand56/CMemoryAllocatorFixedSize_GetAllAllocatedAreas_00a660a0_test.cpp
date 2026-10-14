#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SChunk { unsigned char _pad[8]; SChunk* m_pNext; unsigned char _p[8]; unsigned char* m_pFirstArea; };
static unsigned char g_areas[64]; /* stride 8: area i at 8*i, header byte0 = own index, byte1 = next index */
static void setup(SChunk& c) { std::memset(g_areas, 0, sizeof g_areas); g_areas[0] = 0; g_areas[1] = 2; g_areas[16] = 2; g_areas[17] = 5; g_areas[40] = 5; g_areas[41] = 0xff; std::memset(&c, 0, sizeof c); c.m_pFirstArea = g_areas; }
static bool g_consolidated = false;
struct CAlloc { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { } virtual void v006() { } virtual void v007() { } virtual void v008() { } virtual void ConsolidateFreeAreas() { g_consolidated = true; }
    unsigned char _pad[0xc1c - sizeof(void*)]; SChunk* m_pFirstChunk; unsigned char _p[0xc]; long m_areaStride;
    unsigned long GetAllAllocatedAreas(void** areas, unsigned long) { unsigned long count = 0; ConsolidateFreeAreas(); for (SChunk* chunk = m_pFirstChunk; chunk; chunk = chunk->m_pNext) { unsigned char* area = chunk->m_pFirstArea; while (area) { areas[count++] = area + 4; unsigned char next = area[1]; if (next == 0xff) break; area += (next - area[0]) * m_areaStride; } } return count; } };

int main() {
    SChunk c; setup(c); CAlloc* al = new CAlloc; al->m_pFirstChunk = &c; al->m_areaStride = 8; void* out[8]; unsigned long n = al->GetAllAllocatedAreas(out, 8);
    if (n != 3 || !g_consolidated || out[0] != g_areas + 4 || out[1] != g_areas + 20 || out[2] != g_areas + 44) { std::printf("00a660a0_TEST FAIL\n"); return 1; } al->m_pFirstChunk = 0; if (al->GetAllAllocatedAreas(out, 8) != 0) { std::printf("00a660a0_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a660a0_TEST PASS\n"); return 0;
}
