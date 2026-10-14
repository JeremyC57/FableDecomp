#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static bool g_enabled = false; struct CIME { unsigned char _pad[8]; bool m_active; bool IsImeEnabled() { std::strcat(g_seq, "E"); return g_enabled; } bool IsIMEActive() { if (m_active && IsImeEnabled()) return true; return false; } };

int main() {
    CIME ime; ime.m_active = true; g_enabled = true; if (!ime.IsIMEActive() || std::strcmp(g_seq, "E")) { std::printf("00404c50_TEST FAIL\n"); return 1; } g_enabled = false; if (ime.IsIMEActive()) { std::printf("00404c50_TEST FAIL\n"); return 1; } g_seq[0] = 0; ime.m_active = false; g_enabled = true; if (ime.IsIMEActive() || g_seq[0]) { std::printf("00404c50_TEST FAIL\n"); return 1; }
    std::printf("00404c50_TEST PASS\n"); return 0;
}