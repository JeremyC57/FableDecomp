#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SChunk { unsigned long m_size; unsigned char _pad[4]; long m_noAreas; unsigned char _p[0x14]; SChunk* m_pNext; unsigned char _q[0xc]; void* m_pFirstArea; };
static unsigned long g_counter = 0, g_usage = 0;
struct CAlloc { unsigned char _pad[0xc14]; unsigned long m_usage; unsigned long m_shift; SChunk* m_pFirstChunk;
    unsigned long GetHeaderOverhead() { unsigned long shift = m_shift; g_counter = shift; unsigned long usage = m_usage; g_usage = usage; unsigned long t = 0; for (SChunk* c = m_pFirstChunk; c; c = c->m_pNext) t += c->m_noAreas * (((usage + 0x1f) >> shift) << shift) + (((usage + 0x33) >> shift) << shift); return t; } };

int main() {
    SChunk a, b; std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b); a.m_pNext = &b; CAlloc* al = new CAlloc; std::memset(al, 0, sizeof *al); al->m_pFirstChunk = &a; al->m_usage = 0x11; al->m_shift = 4; a.m_noAreas = 2; b.m_noAreas = 3; /* areaHeader = (0x11+0x1f)>>4<<4 = 0x30, chunkHeader = (0x11+0x33)>>4<<4 = 0x40 */ if (al->GetHeaderOverhead() != 2 * 0x30 + 0x40 + 3 * 0x30 + 0x40 || g_counter != 4 || g_usage != 0x11) { std::printf("00a64e20_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a64e20_TEST PASS\n"); return 0;
}