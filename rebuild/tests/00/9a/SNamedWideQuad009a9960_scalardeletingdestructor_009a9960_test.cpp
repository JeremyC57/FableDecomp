#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CCharString { public: void* m_p; CCharString() : m_p(0) { } ~CCharString() { std::strcat(g_seq, "s"); } };
class CWideString { public: void* m_p; CWideString() : m_p(0) { } ~CWideString() { std::strcat(g_seq, "w"); } };
struct SQuad { CWideString m_0, m_4, m_8, m_c; }; struct SRec { CCharString m_name; SQuad m_texts; ~SRec() { } };

int main() {
    SRec* p = new SRec; delete p; if (std::strcmp(g_seq, "wwwws")) { std::printf("009a9960_TEST FAIL\n"); return 1; }
    std::printf("009a9960_TEST PASS\n"); return 0;
}