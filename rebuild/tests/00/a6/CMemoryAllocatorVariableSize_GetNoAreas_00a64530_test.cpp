#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SChunk { long m_size; long m_noAreas; long m_8; long m_c; unsigned long m_largestFreeArea; unsigned char _pad[0xc]; SChunk* m_pNext; unsigned char _p[0xc]; void* m_pFirstFreeArea; };
static unsigned long g_counter = 0, g_usage = 0;
struct CAlloc { unsigned char _pad[0xc14]; unsigned long m_usage; unsigned long m_counter; SChunk* m_pFirstChunk;
    long GetNoAreas() { g_counter = m_counter; g_usage = m_usage; long t = 0; for (SChunk* c = m_pFirstChunk; c; c = c->m_pNext) t += c->m_noAreas; return t; } };

int main() {
    SChunk a, b, c; std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b); std::memset(&c, 0, sizeof c); a.m_pNext = &b; b.m_pNext = &c; CAlloc* al = new CAlloc; std::memset(al, 0, sizeof *al); al->m_pFirstChunk = &a; al->m_usage = 0x11; al->m_counter = 4; a.m_noAreas = 1; b.m_noAreas = 2; c.m_noAreas = 3; if (al->GetNoAreas() != 6 || g_counter != 4 || g_usage != 0x11) { std::printf("00a64530_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a64530_TEST PASS\n"); return 0;
}