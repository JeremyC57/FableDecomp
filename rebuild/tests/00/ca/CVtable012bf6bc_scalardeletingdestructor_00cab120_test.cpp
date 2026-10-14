#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct IReleasable { virtual void v000() { } virtual void v001() { } virtual void Release() { std::strcat(g_seq, "R"); } }; struct COwner { unsigned char _pad[0xc]; IReleasable m_interface; }; static COwner g_owner; struct CRoot { virtual void v00() { } }; struct CObj : CRoot { long m_4; COwner* m_pOwner; CObj() : m_pOwner(&g_owner) { } virtual ~CObj() { m_pOwner->m_interface.Release(); } };

int main() {
    CObj* p = new CObj; delete p; if (std::strcmp(g_seq, "R")) { std::printf("00cab120_TEST FAIL\n"); return 1; }
    std::printf("00cab120_TEST PASS\n"); return 0;
}