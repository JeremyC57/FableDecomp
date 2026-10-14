#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CCharString { void* m_p; CCharString() : m_p(0) { std::strcat(g_seq, "s"); } };
struct CMapInfo { unsigned char _pad[0x10]; long m_10, m_14; CCharString m_name, m_file; unsigned char _p[0xc]; CCharString m_region; void Clear() { std::strcat(g_seq, "C"); } CMapInfo() : m_10(0), m_14(0), m_name(), m_file(), m_region() { Clear(); } };

int main() {
    CMapInfo* m = new CMapInfo; if (m->m_10 || m->m_14 || std::strcmp(g_seq, "sssC")) { std::printf("004fde50_TEST FAIL\n"); return 1; } delete m;
    std::printf("004fde50_TEST PASS\n"); return 0;
}