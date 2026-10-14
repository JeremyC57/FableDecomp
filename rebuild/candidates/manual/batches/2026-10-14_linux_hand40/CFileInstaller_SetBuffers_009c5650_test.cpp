#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[32] = ""; static int g_locked = 0;
struct CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };
struct CCriticalSectionLock_Retail { CRITICAL_SECTION_Fable* m_pSection; bool m_locked; CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* s, bool) : m_pSection(s), m_locked(true) { ++g_locked; std::strcat(g_seq, "L"); } ~CCriticalSectionLock_Retail() { --g_locked; std::strcat(g_seq, "U"); } };
static void* g_buffers = 0; static long g_count = 0;
struct CFileInstaller { unsigned char _pad[4]; CRITICAL_SECTION_Fable m_section; void InitBuffers(void* b, long c) { g_buffers = b; g_count = c; std::strcat(g_seq, "I"); }
    void SetBuffers(void* b, long c) { CCriticalSectionLock_Retail lock(&m_section, false); InitBuffers(b, c); } };
int main() {
    CFileInstaller f; int b; f.SetBuffers(&b, 7); if (std::strcmp(g_seq, "LIU") || g_buffers != &b || g_count != 7 || g_locked) { std::printf("009c5650_TEST FAIL\n"); return 1; }
    std::printf("009c5650_TEST PASS\n"); return 0;
}
