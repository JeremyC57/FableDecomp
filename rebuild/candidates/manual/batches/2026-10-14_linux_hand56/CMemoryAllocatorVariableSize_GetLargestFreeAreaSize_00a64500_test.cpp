#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SChunk { long m_size; long m_noAreas; long m_8; long m_c; unsigned long m_largestFreeArea; unsigned char _pad[0xc]; SChunk* m_pNext; unsigned char _p[0xc]; void* m_pFirstFreeArea; };
static unsigned long g_counter = 0, g_usage = 0;
struct CAlloc { unsigned char _pad[0xc14]; unsigned long m_usage; unsigned long m_counter; SChunk* m_pFirstChunk;
    unsigned long GetLargestFreeAreaSize() { unsigned long l = 0; for (SChunk* c = m_pFirstChunk; c; c = c->m_pNext) { if (c->m_largestFreeArea > l) l = c->m_largestFreeArea; } return l; } };

int main() {
    SChunk a, b, c; std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b); std::memset(&c, 0, sizeof c); a.m_pNext = &b; b.m_pNext = &c; CAlloc* al = new CAlloc; std::memset(al, 0, sizeof *al); al->m_pFirstChunk = &a; al->m_usage = 0x11; al->m_counter = 4; a.m_largestFreeArea = 5; b.m_largestFreeArea = 50; c.m_largestFreeArea = 7; if (al->GetLargestFreeAreaSize() != 50) { std::printf("00a64500_TEST FAIL\n"); return 1; } al->m_pFirstChunk = 0; if (al->GetLargestFreeAreaSize() != 0) { std::printf("00a64500_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a64500_TEST PASS\n"); return 0;
}
