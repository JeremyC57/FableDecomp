#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_released = 0, g_baseDtor = 0; static void* g_order[4];
struct IUnknownRetail {
    virtual long __stdcall QueryInterface(void*, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { g_order[g_released++] = this; return 0; }
};
namespace N_0099a300 { struct CBase { virtual void v00() { } ~CBase() { ++g_baseDtor; } }; } using N_0099a300::CBase;
struct CVertexShaderData : CBase {
    IUnknownRetail* m_pShader; IUnknownRetail* m_pDeclaration; long m_handle; long m_refCount;
    CVertexShaderData(IUnknownRetail* s, IUnknownRetail* d) : m_pShader(s), m_pDeclaration(d), m_handle(0), m_refCount(1) { }
    __forceinline virtual ~CVertexShaderData() { if (m_pDeclaration) m_pDeclaration->Release(); if (m_pShader) m_pShader->Release(); }
};
int main()
{
    IUnknownRetail s, d;
    CVertexShaderData* v = new CVertexShaderData(&s, &d);
    delete v;
    if (g_released != 2 || g_order[0] != &d || g_order[1] != &s || g_baseDtor != 1) { std::printf("00a630d0_TEST FAIL\n"); return 1; }
    v = new CVertexShaderData(0, &d); delete v;
    if (g_released != 3 || g_baseDtor != 2) { std::printf("00a630d0_TEST FAIL\n"); return 1; }
    std::printf("00a630d0_TEST PASS\n"); return 0;
}