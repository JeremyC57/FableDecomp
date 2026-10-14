#include <cstdio>
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct CEnginePrimitiveManager : CBase { long m_type; bool m_enabled; CEnginePrimitiveManager(long type) : CBase(), m_type(type), m_enabled(true) { } };
int main() {
    CEnginePrimitiveManager m(0x28); if (m.m_type != 0x28 || !m.m_enabled || g_base != 1) { std::printf("00bacdb0_TEST FAIL\n"); return 1; }
    std::printf("00bacdb0_TEST PASS\n"); return 0;
}