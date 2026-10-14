#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CCharString { void* m_p; CCharString() : m_p(0) { std::strcat(g_seq, "s"); } };
struct SRec { virtual void v00() { } long m_count; SRec() : m_count(1) { } }; struct CMapFilenames : SRec { CCharString m_diffuse, m_bump, m_specular, m_detail; CMapFilenames() : SRec(), m_diffuse(), m_bump(), m_specular(), m_detail() { } };

int main() {
    CMapFilenames* m = new CMapFilenames; if (m->m_count != 1 || std::strcmp(g_seq, "ssss") || m->m_diffuse.m_p || m->m_detail.m_p) { std::printf("00ac5190_TEST FAIL\n"); return 1; } delete m;
    std::printf("00ac5190_TEST PASS\n"); return 0;
}