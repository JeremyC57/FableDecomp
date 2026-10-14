#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SChunk { unsigned long m_size; unsigned char _pad[4]; long m_noAreas; unsigned char _p[0x14]; SChunk* m_pNext; unsigned char _q[0xc]; void* m_pFirstArea; };
static unsigned long g_counter = 0, g_usage = 0;
struct CAlloc { unsigned char _pad[0xc14]; unsigned long m_usage; unsigned long m_shift; SChunk* m_pFirstChunk;
    long GetAllocationSize(void* block) { unsigned long shift = m_shift; g_counter = shift; unsigned long usage = m_usage; g_usage = usage; return *(long*)((char*)block - (((usage + 0x1f) >> shift) << shift) + 0x18); } };

int main() {
    CAlloc* al = new CAlloc; std::memset(al, 0, sizeof *al); al->m_usage = 0x11; al->m_shift = 4; unsigned char buf[0x60]; *(long*)(buf + 0x18) = 77; if (al->GetAllocationSize(buf + 0x30) != 77 || g_counter != 4 || g_usage != 0x11) { std::printf("00a651b0_TEST FAIL\n"); return 1; } delete al;
    std::printf("00a651b0_TEST PASS\n"); return 0;
}
