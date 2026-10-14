#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CCharString { void* m_p; CCharString() : m_p(0) { } ~CCharString() { std::strcat(g_seq, "s"); } };
struct CAnimFilterBase { virtual void v00() { } ~CAnimFilterBase() { std::strcat(g_seq, "B"); } }; struct CNamed : CAnimFilterBase { CCharString m_4, m_8, m_c; ~CNamed() { } };

int main() {
    CNamed* p = new CNamed; delete p; if (std::strcmp(g_seq, "sssB")) { std::printf("0073e190_TEST FAIL\n"); return 1; }
    std::printf("0073e190_TEST PASS\n"); return 0;
}
