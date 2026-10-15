#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct __declspec(novtable) CParticleUpdateParamDescBase : CBase { void* m_pName; CParticleUpdateParamDescBase(void* n) : CBase(), m_pName(n) { } };
struct CParticleUpdateParamDesc : CParticleUpdateParamDescBase {
    void* m_pParam; virtual void v00() { }
    CParticleUpdateParamDesc(void* n, void* p) : CParticleUpdateParamDescBase(n), m_pParam(p) { }
};
int main()
{
    int a, b; CParticleUpdateParamDesc v(&a, &b);
    if (g_base != 1 || v.m_pName != &a || v.m_pParam != &b) { std::printf("00b0f310_TEST FAIL\n"); return 1; }
    std::printf("00b0f310_TEST PASS\n"); return 0;
}