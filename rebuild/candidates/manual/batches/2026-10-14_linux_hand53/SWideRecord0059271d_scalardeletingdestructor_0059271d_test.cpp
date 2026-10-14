#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CWideString { void* m_p; CWideString() : m_p(0) { } ~CWideString() { std::strcat(g_seq, "w"); } };
struct SRec { unsigned char _pad[0x1c]; CWideString m_text; ~SRec() { } };

int main() {
    SRec* p = new SRec; delete p; if (std::strcmp(g_seq, "w")) { std::printf("0059271d_TEST FAIL\n"); return 1; }
    std::printf("0059271d_TEST PASS\n"); return 0;
}
