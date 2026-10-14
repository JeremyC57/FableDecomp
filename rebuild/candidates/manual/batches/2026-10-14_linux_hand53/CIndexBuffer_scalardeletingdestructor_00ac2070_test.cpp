#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CMovableResource { virtual void v00() { } ~CMovableResource() { std::strcat(g_seq, "M"); } }; struct CIndexBuffer : CMovableResource { void Release() { std::strcat(g_seq, "R"); } virtual ~CIndexBuffer() { Release(); } };

int main() {
    CIndexBuffer* p = new CIndexBuffer; delete p; if (std::strcmp(g_seq, "RM")) { std::printf("00ac2070_TEST FAIL\n"); return 1; }
    std::printf("00ac2070_TEST PASS\n"); return 0;
}
