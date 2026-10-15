#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct __declspec(novtable) CEngineSubPrimitiveBase : CBase {
    unsigned char _pad04[8]; void* m_pMesh; long m_count; unsigned char _pad14[4]; void* m_pMaterial; void* m_pInstances; unsigned char m_flag;
    CEngineSubPrimitiveBase(void* m, void* mat, void* i) : CBase(), m_pMesh(m), m_count(0), m_pMaterial(mat), m_pInstances(i), m_flag(0) { }
};
struct CEngineSubPrimitiveRepeatedStaticMesh : CEngineSubPrimitiveBase {
    virtual void v00() { }
    CEngineSubPrimitiveRepeatedStaticMesh(void* m, void* mat, void* i) : CEngineSubPrimitiveBase(m, mat, i) { }
};
int main()
{
    int a, b, c; unsigned char raw[sizeof(CEngineSubPrimitiveRepeatedStaticMesh)]; for (unsigned k = 0; k < sizeof(raw); ++k) raw[k] = 0xAA;
    CEngineSubPrimitiveRepeatedStaticMesh* v = new (raw) CEngineSubPrimitiveRepeatedStaticMesh(&a, &b, &c);
    if (g_base != 1 || v->m_pMesh != &a || v->m_count != 0 || v->m_pMaterial != &b || v->m_pInstances != &c || v->m_flag != 0) { std::printf("00b98bc0_TEST FAIL\n"); return 1; }
    std::printf("00b98bc0_TEST PASS\n"); return 0;
}