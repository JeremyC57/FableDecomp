#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct CEngineSubPrimitiveBase : CBase { unsigned char _pad04[8]; void* m_pPrimitive; long m_pNext; unsigned char _pad14[4]; void* m_pMesh; void* m_pMaterial; bool m_flag;
    CEngineSubPrimitiveBase(void* p, void* m, void* t, bool f) : CBase(), m_pPrimitive(p), m_pNext(0), m_pMesh(m), m_pMaterial(t), m_flag(f) { } };
struct CEngineSubPrimitiveStaticMesh : CEngineSubPrimitiveBase { CEngineSubPrimitiveStaticMesh(void* p, void* m, void* t, bool f) : CEngineSubPrimitiveBase(p, m, t, f) { } };
int main() {
    int p, m, t; CEngineSubPrimitiveStaticMesh s(&p, &m, &t, true); if (s.m_pPrimitive != &p || s.m_pNext || s.m_pMesh != &m || s.m_pMaterial != &t || !s.m_flag || g_base != 1) { std::printf("00b9cb10_TEST FAIL\n"); return 1; }
    std::printf("00b9cb10_TEST PASS\n"); return 0;
}