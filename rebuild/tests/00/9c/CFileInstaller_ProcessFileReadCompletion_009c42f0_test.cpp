#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[32] = ""; static int g_locked = 0;
struct CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };
struct CCriticalSectionLock_Retail { CRITICAL_SECTION_Fable* m_pSection; bool m_locked; CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* s, bool) : m_pSection(s), m_locked(true) { ++g_locked; std::strcat(g_seq, "L"); } ~CCriticalSectionLock_Retail() { --g_locked; std::strcat(g_seq, "U"); } };
static void* g_record = 0;
class CFileInstaller { public: unsigned char _pad[4]; CRITICAL_SECTION_Fable m_section; unsigned char _pad1c[0x58]; void* volatile m_pReadCompletion; void ProcessReadCompletionData(void* r) { g_record = r; std::strcat(g_seq, "P"); }
    void ProcessFileReadCompletion() { CCriticalSectionLock_Retail lock(&m_section, false); if (m_pReadCompletion) { ProcessReadCompletionData(m_pReadCompletion); m_pReadCompletion = 0; } } };
int main() {
    CFileInstaller f; std::memset(&f, 0, sizeof f); int r; f.m_pReadCompletion = &r; f.ProcessFileReadCompletion(); if (std::strcmp(g_seq, "LPU") || g_record != &r || f.m_pReadCompletion != 0 || g_locked) { std::printf("009c42f0_TEST FAIL\n"); return 1; }
    f.ProcessFileReadCompletion(); if (std::strcmp(g_seq, "LPULU")) { std::printf("009c42f0_TEST FAIL\n"); return 1; }
    std::printf("009c42f0_TEST PASS\n"); return 0;
}