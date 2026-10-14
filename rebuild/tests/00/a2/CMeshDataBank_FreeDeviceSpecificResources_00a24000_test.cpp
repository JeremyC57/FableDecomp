#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_n = 0; static void* g_last = 0; struct CEntry { virtual void v000() { } virtual void Release() { ++g_n; g_last = this; } }; struct CVec { CEntry** m_first; CEntry** m_last; CEntry** m_end; unsigned long size() const { return m_last - m_first; } };
struct CBank { unsigned char _pad[0x380]; CVec m_entries; void FreeDeviceSpecificResources() { for (unsigned long i = 0; i < m_entries.size(); ++i) { CEntry* e = m_entries.m_first[i]; if (e) e->Release(); } } };

int main() {
    CEntry e[3]; CEntry* p[3] = { &e[0], 0, &e[2] }; CBank* b = new CBank; b->m_entries.m_first = p; b->m_entries.m_last = p + 3; b->FreeDeviceSpecificResources(); if (g_n != 2 || g_last != &e[2]) { std::printf("00a24000_TEST FAIL\n"); return 1; } delete b;
    std::printf("00a24000_TEST PASS\n"); return 0;
}