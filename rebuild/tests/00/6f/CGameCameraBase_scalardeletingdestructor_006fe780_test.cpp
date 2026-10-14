#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CAICam { virtual void v00() { } unsigned char _body[0xa4]; ~CAICam() { std::strcat(g_seq, "A"); } }; struct CList { void* m_p; ~CList() { std::strcat(g_seq, "l"); } }; struct CGameCameraBase : CAICam { CList m_lista8, m_listac; virtual ~CGameCameraBase() { } };

int main() {
    CGameCameraBase* p = new CGameCameraBase; delete p; if (std::strcmp(g_seq, "llA")) { std::printf("006fe780_TEST FAIL\n"); return 1; }
    std::printf("006fe780_TEST PASS\n"); return 0;
}