#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SArea { SArea* m_pNext; };
static long g_n = 0; static void* g_last = 0; struct CGlobalAlloc { void ValidateBlock(void* b) { ++g_n; g_last = b; } }; static CGlobalAlloc g_ga; static CGlobalAlloc* g_pGlobalAllocator = &g_ga;
struct SChunk { unsigned long m_size; unsigned char _pad[4]; long m_noAreas; unsigned char _p[0x14]; SChunk* m_pNext; unsigned char _q[0xc]; void* m_pFirstArea; };
static unsigned long g_counter = 0, g_usage = 0;
struct CAlloc { unsigned char _pad[0xc14]; unsigned long m_usage; unsigned long m_shift; SChunk* m_pFirstChunk;
    void Validate() { unsigned long shift = m_shift; g_counter = shift; unsigned long usage = m_usage; g_usage = usage; for (SChunk* c = m_pFirstChunk; c; c = c->m_pNext) { for (SArea* area = (SArea*)c->m_pFirstArea; area; area = area->m_pNext) g_pGlobalAllocator->ValidateBlock((char*)area + (((g_usage + 0x1f) >> g_counter) << g_counter)); } } };

int main() {
    SChunk a, b; std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b); a.m_pNext = &b; CAlloc* al = new CAlloc; std::memset(al, 0, sizeof *al); al->m_pFirstChunk = &a; al->m_usage = 0x11; al->m_shift = 4; SArea x, y, z; x.m_pNext = &y; y.m_pNext = 0; z.m_pNext = 0; a.m_pFirstArea = &x; b.m_pFirstArea = &z; al->Validate(); if (g_n != 3 || g_last != (char*)&z + 0x30 || g_counter != 4 || g_usage != 0x11) { std::printf("00a65330_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a65330_TEST PASS\n"); return 0;
}