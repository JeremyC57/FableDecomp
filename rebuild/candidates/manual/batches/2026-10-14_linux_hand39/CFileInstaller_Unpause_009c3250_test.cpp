#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_set = 0; static void* g_events[4]; static char g_seq[16] = ""; static int g_locked = 0;
static int SetEvent(void* e) { g_events[g_set++] = e; return 1; }
struct CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };
struct CCriticalSectionLock_Retail { CRITICAL_SECTION_Fable* m_pSection; bool m_locked; CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* s, bool) : m_pSection(s), m_locked(true) { ++g_locked; std::strcat(g_seq, "L"); } ~CCriticalSectionLock_Retail() { --g_locked; std::strcat(g_seq, "U"); } };
struct CFileInstaller { unsigned char _pad[4]; CRITICAL_SECTION_Fable m_section; unsigned char _pad1c[0x5f]; volatile bool m_waiting; volatile unsigned long m_pauseCount; unsigned char _pad80[4]; void* m_hResumeEvent; unsigned char _pad88[8]; void* m_hWakeEvent;
    void Unpause() { CCriticalSectionLock_Retail lock(&m_section, false); if (m_pauseCount > 0) { --m_pauseCount; if (m_waiting) { if (!(m_pauseCount > 0)) { SetEvent(m_hWakeEvent); SetEvent(m_hResumeEvent); } } } } };
int main() {
    CFileInstaller f; std::memset(&f, 0, sizeof f); f.m_hResumeEvent = (void*)1; f.m_hWakeEvent = (void*)2;
    f.m_pauseCount = 2; f.m_waiting = true; f.Unpause(); if (f.m_pauseCount != 1 || g_set != 0 || std::strcmp(g_seq, "LU")) { std::printf("009c3250_TEST FAIL\n"); return 1; }
    f.Unpause(); if (f.m_pauseCount != 0 || g_set != 2 || g_events[0] != (void*)2 || g_events[1] != (void*)1 || std::strcmp(g_seq, "LULU")) { std::printf("009c3250_TEST FAIL\n"); return 1; }
    f.Unpause(); if (f.m_pauseCount != 0 || g_set != 2) { std::printf("009c3250_TEST FAIL\n"); return 1; }
    f.m_pauseCount = 1; f.m_waiting = false; f.Unpause(); if (f.m_pauseCount != 0 || g_set != 2 || g_locked != 0) { std::printf("009c3250_TEST FAIL\n"); return 1; }
    std::printf("009c3250_TEST PASS\n"); return 0;
}
