#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0; static void* g_a = 0; static void* g_b = 0;
struct CDefMember_004dfac5 { unsigned char _body[12]; CDefMember_004dfac5() { g_a = this; } }; struct CDefMember_004dfae3 { unsigned char _body[4]; CDefMember_004dfae3() { g_b = this; } };
struct CShoveHaPennyRegionDef_Base { virtual void v00() { } unsigned char _pad[0x24]; CShoveHaPennyRegionDef_Base() { ++g_base; } };
struct CRumbleDef : CShoveHaPennyRegionDef_Base { CDefMember_004dfac5 m_28; CDefMember_004dfae3 m_34; virtual void v00() { } CRumbleDef() : CShoveHaPennyRegionDef_Base() { } };
int main() {
    CRumbleDef* d = new CRumbleDef; if (g_base != 1 || g_a != &d->m_28 || g_b != &d->m_34) { std::printf("004e1722_TEST FAIL\n"); return 1; } delete d;
    std::printf("004e1722_TEST PASS\n"); return 0;
}
