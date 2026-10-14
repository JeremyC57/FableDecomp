#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0;
struct CParentDefClassBase { virtual void v00() { } unsigned char _pad[0x38]; CParentDefClassBase() { ++g_base; } };
struct CIndex_00456964 { long m_value; CIndex_00456964() : m_value(-1) { } };
struct CLink_00456964 { void* m_p; CLink_00456964() : m_p(0) { } };
struct CExpressionDef : CParentDefClassBase { CIndex_00456964 m_3c, m_40, m_44, m_48, m_4c, m_50, m_54, m_58; long _pad5c, _pad60; CIndex_00456964 m_64, m_68, m_6c; CLink_00456964 m_70; long _pad74; CIndex_00456964 m_78; virtual void v00() { } CExpressionDef() { } };
int main() {
    CExpressionDef* d = new CExpressionDef; if (g_base != 1 || d->m_3c.m_value != -1 || d->m_58.m_value != -1 || d->m_64.m_value != -1 || d->m_6c.m_value != -1 || d->m_70.m_p != 0 || d->m_78.m_value != -1) { std::printf("00456964_TEST FAIL\n"); return 1; } delete d;
    std::printf("00456964_TEST PASS\n"); return 0;
}
