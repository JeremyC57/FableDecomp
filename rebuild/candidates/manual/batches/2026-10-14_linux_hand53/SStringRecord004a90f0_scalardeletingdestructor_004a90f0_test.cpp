#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CCharString { void* m_p; CCharString() : m_p(0) { } ~CCharString() { std::strcat(g_seq, "s"); } };
struct SRec { CCharString m_0, m_4; long m_8; CCharString m_c, m_10, m_14, m_18; ~SRec() { } };

int main() {
    SRec* p = new SRec; delete p; if (std::strcmp(g_seq, "ssssss")) { std::printf("004a90f0_TEST FAIL\n"); return 1; }
    std::printf("004a90f0_TEST PASS\n"); return 0;
}
