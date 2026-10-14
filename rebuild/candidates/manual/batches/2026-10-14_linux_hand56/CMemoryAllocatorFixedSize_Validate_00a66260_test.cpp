#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SChunk { unsigned char _pad[8]; SChunk* m_pNext; unsigned char _p[8]; unsigned char* m_pFirstArea; };
static unsigned char g_areas[64]; /* stride 8: area i at 8*i, header byte0 = own index, byte1 = next index */
static void setup(SChunk& c) { std::memset(g_areas, 0, sizeof g_areas); g_areas[0] = 0; g_areas[1] = 2; g_areas[16] = 2; g_areas[17] = 5; g_areas[40] = 5; g_areas[41] = 0xff; std::memset(&c, 0, sizeof c); c.m_pFirstArea = g_areas; }
static long g_n = 0; static void* g_last = 0; struct CGlobalAlloc { void ValidateBlock(void* b) { ++g_n; g_last = b; } }; static CGlobalAlloc g_ga; static CGlobalAlloc* g_pGlobalAllocator = &g_ga;
struct CAlloc { unsigned char _pad[0xc2c]; long m_areaStride; void Validate(SChunk* chunk) { unsigned char* area = chunk->m_pFirstArea; while (area) { g_pGlobalAllocator->ValidateBlock(area + 4); unsigned char next = area[1]; if (next == 0xff) break; area += (next - area[0]) * m_areaStride; } } };

int main() {
    SChunk c; setup(c); CAlloc* al = new CAlloc; std::memset(al, 0, sizeof *al); al->m_areaStride = 8; al->Validate(&c); if (g_n != 3 || g_last != g_areas + 44) { std::printf("00a66260_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a66260_TEST PASS\n"); return 0;
}
