#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SChunk { unsigned char _pad[8]; SChunk* m_pNext; unsigned char _p[8]; unsigned char* m_pFirstArea; };
static unsigned char g_areas[64]; /* stride 8: area i at 8*i, header byte0 = own index, byte1 = next index */
static void setup(SChunk& c) { std::memset(g_areas, 0, sizeof g_areas); g_areas[0] = 0; g_areas[1] = 2; g_areas[16] = 2; g_areas[17] = 5; g_areas[40] = 5; g_areas[41] = 0xff; std::memset(&c, 0, sizeof c); c.m_pFirstArea = g_areas; }
struct CAlloc { unsigned char _pad[0xc1c]; SChunk* m_pFirstChunk; unsigned char _p[0xc]; long m_areaStride;
    void* GetAllocatedArea(long index) { long count = 0; for (SChunk* chunk = m_pFirstChunk; chunk; chunk = chunk->m_pNext) { unsigned char* area = chunk->m_pFirstArea; while (area) { if (count == index) return area + 4; unsigned char next = area[1]; ++count; if (next == 0xff) break; area += (next - area[0]) * m_areaStride; } } return 0; } };

int main() {
    SChunk c; setup(c); CAlloc* al = new CAlloc; std::memset(al, 0, sizeof *al); al->m_pFirstChunk = &c; al->m_areaStride = 8; if (al->GetAllocatedArea(0) != g_areas + 4 || al->GetAllocatedArea(2) != g_areas + 44 || al->GetAllocatedArea(3) != 0) { std::printf("00a66100_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a66100_TEST PASS\n"); return 0;
}