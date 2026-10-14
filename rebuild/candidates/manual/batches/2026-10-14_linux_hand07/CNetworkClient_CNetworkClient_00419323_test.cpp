#include <cstdio>
#include <cstddef>
// Behaviour model of CNetworkClient::CNetworkClient @ 0x00419323: base, embedded server (base + vtable), package.
static int g_order[8]; static int g_n = 0;
struct CInitBaseClass { virtual void v00() {} CInitBaseClass() { g_order[g_n++] = 1; } };
struct CNetworkServer : CInitBaseClass { unsigned char _body[0x2004]; virtual void v00() {} CNetworkServer() : CInitBaseClass() { g_order[g_n++] = 2; } };
struct CGameEventPackage { CGameEventPackage() { g_order[g_n++] = 3; } };
struct CNetworkClient : CInitBaseClass { long m_connection; CNetworkServer m_server; CGameEventPackage m_package; virtual void v00() {}
    CNetworkClient() : CInitBaseClass(), m_server(), m_package() {} };
int main()
{
    CNetworkClient c;
    bool ok = g_n == 4 && g_order[0] == 1 && g_order[1] == 1 && g_order[2] == 2 && g_order[3] == 3;
    if (sizeof(void*) == 4) ok = ok && offsetof(CNetworkClient, m_server) == 0x8 && offsetof(CNetworkClient, m_package) == 0x2010;
    if (!ok) { std::printf("00419323_TEST FAIL\n"); return 1; }
    std::printf("00419323_TEST PASS\n");
    return 0;
}
