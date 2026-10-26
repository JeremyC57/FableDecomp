#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CCharString { public: void* m_p; CCharString() : m_p(0) { } ~CCharString() { std::strcat(g_seq, "s"); } };
class CPhysicsMeshInfo { public: virtual void v00() { } unsigned char _body[0xa4]; ~CPhysicsMeshInfo() { std::strcat(g_seq, "P"); } }; struct CIP { virtual void v00() { } void* m_p; ~CIP() { std::strcat(g_seq, "i"); } }; struct CNamed : CPhysicsMeshInfo { CIP m_a8; CCharString m_b0, m_b4; ~CNamed() { } };

int main() {
    CNamed* p = new CNamed; delete p; if (std::strcmp(g_seq, "ssiP")) { std::printf("0092e3e0_TEST FAIL\n"); return 1; }
    std::printf("0092e3e0_TEST PASS\n"); return 0;
}