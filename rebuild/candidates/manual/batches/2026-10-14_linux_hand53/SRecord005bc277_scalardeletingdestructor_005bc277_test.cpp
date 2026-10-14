#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CMember { unsigned char _b[4]; ~CMember() { std::strcat(g_seq, "m"); } }; struct SRec { unsigned char _pad[0xc]; CMember m_member; ~SRec() { } };

int main() {
    SRec* p = new SRec; delete p; if (std::strcmp(g_seq, "m")) { std::printf("005bc277_TEST FAIL\n"); return 1; }
    std::printf("005bc277_TEST PASS\n"); return 0;
}
