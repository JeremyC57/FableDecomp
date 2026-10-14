#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static long g_lock = 0; static void EnterCriticalSection(void*) { ++g_lock; std::strcat(g_seq, "E"); } static void LeaveCriticalSection(void*) { --g_lock; std::strcat(g_seq, "L"); } static unsigned char g_section[24];
struct IAlloc { virtual unsigned long GetHeaderOverhead() { std::strcat(g_seq, "h"); return 10; } virtual unsigned long GetWastedSpace() { std::strcat(g_seq, "w"); return 100; } virtual void ConsolidateFreeAreas() { std::strcat(g_seq, "c"); } };
struct SEntry { IAlloc* m_pAllocator; unsigned char _pad[0x18]; }; static SEntry g_table[3];
struct CMemoryAllocator { unsigned char _pad[0x410]; unsigned long m_no; unsigned long GetHeaderOverhead() { unsigned long t = 0; EnterCriticalSection(g_section); for (unsigned long i = 0; i < m_no; ++i) t += g_table[i].m_pAllocator->GetHeaderOverhead(); LeaveCriticalSection(g_section); return t; } };

int main() {
    IAlloc subs[3]; for (int i = 0; i < 3; ++i) g_table[i].m_pAllocator = &subs[i]; CMemoryAllocator m; m.m_no = 3; unsigned long r = m.GetHeaderOverhead(); if (r != 30 || g_lock || std::strcmp(g_seq, "EhhhL")) { std::printf("009f2450_TEST FAIL\n"); return 1; } g_seq[0] = 0; m.m_no = 0; if (m.GetHeaderOverhead() != 0 || std::strcmp(g_seq, "EL")) { std::printf("009f2450_TEST FAIL\n"); return 1; }
    std::printf("009f2450_TEST PASS\n"); return 0;
}
