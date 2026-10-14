#include <cstdio>
#include <cstring>
static int g_n = 0; static const char* g_names[2]; static void* g_ptr[2];
struct CPersistContext { void TransferBool(const char* n, bool* v) { g_names[g_n] = n; g_ptr[g_n++] = v; } void TransferSignedChar(const char* n, signed char* v) { g_names[g_n] = n; g_ptr[g_n++] = v; } };
struct CHeroStatsBlock_00578797 { unsigned char _pad0[0x1a0]; bool m_openedExperienceSpending; unsigned char _pad1[0xa7]; signed char m_tutorialsPlayed;
    void Transfer(CPersistContext& c, long) { c.TransferBool("OpenedExperienceSpending", &m_openedExperienceSpending); c.TransferSignedChar("TutorialsPlayed", &m_tutorialsPlayed); } };
int main() {
    CHeroStatsBlock_00578797* b = (CHeroStatsBlock_00578797*)new char[sizeof(CHeroStatsBlock_00578797)]; CPersistContext c; b->Transfer(c, 1);
    if (g_n != 2 || std::strcmp(g_names[0], "OpenedExperienceSpending") || std::strcmp(g_names[1], "TutorialsPlayed") || g_ptr[0] != &b->m_openedExperienceSpending || g_ptr[1] != &b->m_tutorialsPlayed) { std::printf("00578797_TEST FAIL\n"); return 1; }
    delete[] (char*)b;
    std::printf("00578797_TEST PASS\n"); return 0;
}
