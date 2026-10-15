#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct __declspec(novtable) CVertexShaderDataBase : CBase { void* m_pShader; CVertexShaderDataBase(void* s) : CBase(), m_pShader(s) { } };
struct CVertexShaderData : CVertexShaderDataBase {
    void* m_pDeclaration; long m_handle; long m_refCount; virtual void v00() { }
    CVertexShaderData(void* s, void* d, long h, long, long) : CVertexShaderDataBase(s), m_pDeclaration(d), m_handle(h), m_refCount(1) { }
};
int main()
{
    int a, b; CVertexShaderData v(&a, &b, 42, 7, 9);
    if (g_base != 1 || v.m_pShader != &a || v.m_pDeclaration != &b || v.m_handle != 42 || v.m_refCount != 1) { std::printf("00a62fc0_TEST FAIL\n"); return 1; }
    std::printf("00a62fc0_TEST PASS\n"); return 0;
}
