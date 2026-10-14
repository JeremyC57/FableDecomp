#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct CEngineSubPrimitiveBase : CBase { unsigned char _pad04[8]; void* m_pPrimitive; long m_pNext; CEngineSubPrimitiveBase(void* p) : CBase(), m_pPrimitive(p), m_pNext(0) { } };
static int g_str = 0;
struct CCharString { void* m_pData; CCharString() : m_pData(0) { ++g_str; } };
struct CEngineSubPrimitiveText : CEngineSubPrimitiveBase { unsigned char _pad14[0x18]; CCharString m_text; CEngineSubPrimitiveText(void* p) : CEngineSubPrimitiveBase(p), m_text() { } };
int main() {
    int prim; CEngineSubPrimitiveText t(&prim); if (t.m_pPrimitive != &prim || t.m_pNext || g_base != 1 || g_str != 1 || t.m_text.m_pData) { std::printf("00bc7a50_TEST FAIL\n"); return 1; }
    std::printf("00bc7a50_TEST PASS\n"); return 0;
}
