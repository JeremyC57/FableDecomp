#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SChunk { unsigned char _pad[8]; SChunk* m_pNext; unsigned char _p[8]; unsigned char* m_pFirstArea; };
static unsigned char g_areas[64]; /* stride 8: area i at 8*i, header byte0 = own index, byte1 = next index */
static void setup(SChunk& c) { std::memset(g_areas, 0, sizeof g_areas); g_areas[0] = 0; g_areas[1] = 2; g_areas[16] = 2; g_areas[17] = 5; g_areas[40] = 5; g_areas[41] = 0xff; std::memset(&c, 0, sizeof c); c.m_pFirstArea = g_areas; }
static long g_n = 0; static SChunk* g_last = 0;
struct CAlloc { virtual void v000() { } unsigned char _pad[0xc1c - sizeof(void*)]; SChunk* m_pFirstChunk; void Validate(SChunk* c) { ++g_n; g_last = c; } virtual void Validate() { for (SChunk* chunk = m_pFirstChunk; chunk; chunk = chunk->m_pNext) Validate(chunk); } };

int main() {
    SChunk a, b; std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b); a.m_pNext = &b; CAlloc* al = new CAlloc; al->m_pFirstChunk = &a; al->Validate(); if (g_n != 2 || g_last != &b) { std::printf("00a663a0_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a663a0_TEST PASS\n"); return 0;
}