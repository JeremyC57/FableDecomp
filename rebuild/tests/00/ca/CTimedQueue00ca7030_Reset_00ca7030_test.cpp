#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static unsigned int g_killed = 0; static unsigned int timeKillEvent(unsigned int t) { std::strcat(g_seq, "K"); g_killed = t; return 0; } static void EnterCriticalSection(void*) { std::strcat(g_seq, "E"); } static void LeaveCriticalSection(void*) { std::strcat(g_seq, "L"); }
struct CQueue { unsigned char _pad[0x70]; long m_70, m_74; unsigned char _pad78[0x1c]; unsigned char m_section[24]; unsigned char _padac[0xc]; long m_b8, m_bc; unsigned int m_timer;
    long Reset() { if (m_timer) { timeKillEvent(m_timer); m_timer = 0; } EnterCriticalSection(m_section); m_70 = 0; m_74 = 0; m_b8 = 0; m_bc = 0; LeaveCriticalSection(m_section); return 0; } };

int main() {
    CQueue q; std::memset(&q, 0xff, sizeof q); q.m_timer = 5; if (q.Reset() != 0 || std::strcmp(g_seq, "KEL") || g_killed != 5 || q.m_timer || q.m_70 || q.m_74 || q.m_b8 || q.m_bc) { std::printf("00ca7030_TEST FAIL\n"); return 1; } g_seq[0] = 0; q.Reset(); if (std::strcmp(g_seq, "EL")) { std::printf("00ca7030_TEST FAIL\n"); return 1; }
    std::printf("00ca7030_TEST PASS\n"); return 0;
}