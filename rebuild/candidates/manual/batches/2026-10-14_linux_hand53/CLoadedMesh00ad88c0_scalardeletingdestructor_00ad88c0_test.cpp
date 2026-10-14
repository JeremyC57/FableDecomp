#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CBaseNC { virtual void v00() { } ~CBaseNC() { std::strcat(g_seq, "N"); } }; struct CMeshBase : CBaseNC { virtual void v00() { } ~CMeshBase() { std::strcat(g_seq, "M"); } }; struct CMember { unsigned char _b[4]; ~CMember() { std::strcat(g_seq, "m"); } }; struct CLoaded { CMeshBase m_base; CMember m_member4; ~CLoaded() { } };

int main() {
    CLoaded* p = new CLoaded; delete p; if (std::strcmp(g_seq, "mMN")) { std::printf("00ad88c0_TEST FAIL\n"); return 1; }
    std::printf("00ad88c0_TEST PASS\n"); return 0;
}
