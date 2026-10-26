#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CMovableResource { public: virtual void v00() { } ~CMovableResource() { std::strcat(g_seq, "M"); } }; class CIndexBuffer : public CMovableResource { public: void Release() { std::strcat(g_seq, "R"); } virtual ~CIndexBuffer() { Release(); } };

int main() {
    CIndexBuffer* p = new CIndexBuffer; delete p; if (std::strcmp(g_seq, "RM")) { std::printf("00ac2070_TEST FAIL\n"); return 1; }
    std::printf("00ac2070_TEST PASS\n"); return 0;
}