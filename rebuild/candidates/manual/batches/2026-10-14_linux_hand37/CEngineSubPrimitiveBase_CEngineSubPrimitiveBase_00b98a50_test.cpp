#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct CEngineSubPrimitiveBase : CBase { unsigned char _pad04[8]; void* m_pPrimitive; long m_pNext; CEngineSubPrimitiveBase(void* p) : CBase(), m_pPrimitive(p), m_pNext(0) { } };

int main() {
    int p; CEngineSubPrimitiveBase b(&p); if (b.m_pPrimitive != &p || b.m_pNext || g_base != 1) { std::printf("00b98a50_TEST FAIL\n"); return 1; }
    std::printf("00b98a50_TEST PASS\n"); return 0;
}
