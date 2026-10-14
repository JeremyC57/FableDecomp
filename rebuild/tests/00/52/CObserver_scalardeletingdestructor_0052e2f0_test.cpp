#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CRoot { virtual void v00() { } }; struct CMember { unsigned char _b[4]; ~CMember() { std::strcat(g_seq, "m"); } }; struct CObserver : CRoot { CMember m_member4; virtual ~CObserver() { } };

int main() {
    CObserver* p = new CObserver; delete p; if (std::strcmp(g_seq, "m")) { std::printf("0052e2f0_TEST FAIL\n"); return 1; }
    std::printf("0052e2f0_TEST PASS\n"); return 0;
}