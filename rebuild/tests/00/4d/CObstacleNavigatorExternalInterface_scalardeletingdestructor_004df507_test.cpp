#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CNav { virtual void v00() { } long m_4; ~CNav() { std::strcat(g_seq, "N"); } }; struct CMember { unsigned char _b[4]; ~CMember() { std::strcat(g_seq, "m"); } }; struct CObstacleNav : CNav { CMember m_member8; virtual ~CObstacleNav() { } };

int main() {
    CObstacleNav* p = new CObstacleNav; delete p; if (std::strcmp(g_seq, "mN")) { std::printf("004df507_TEST FAIL\n"); return 1; }
    std::printf("004df507_TEST PASS\n"); return 0;
}