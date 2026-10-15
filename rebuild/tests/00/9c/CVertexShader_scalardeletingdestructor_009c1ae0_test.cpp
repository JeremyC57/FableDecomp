#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_dataDeleted = 0, g_baseDtor = 0;
struct CVertexShaderData {
    virtual ~CVertexShaderData() { ++g_dataDeleted; }
    void* m_pShader; void* m_pDeclaration; long m_handle; long m_refCount;
    CVertexShaderData(long n) : m_pShader(0), m_pDeclaration(0), m_handle(0), m_refCount(n) { }
};
struct CShaderBase { virtual ~CShaderBase() { ++g_baseDtor; } };
struct CVertexShader : CShaderBase {
    CVertexShaderData* Data;
    CVertexShader(CVertexShaderData* d) : Data(d) { }
    __forceinline virtual ~CVertexShader() { CVertexShaderData* data = Data; if (data) { if (--data->m_refCount == 0) delete data; } }
};
int main()
{
    CVertexShaderData* shared = new CVertexShaderData(2);
    CVertexShader* a = new CVertexShader(shared); CVertexShader* b = new CVertexShader(shared); CVertexShader* c = new CVertexShader(0);
    delete a;
    if (g_dataDeleted != 0 || shared->m_refCount != 1 || g_baseDtor != 1) { std::printf("009c1ae0_TEST FAIL\n"); return 1; }
    delete b; delete c;
    if (g_dataDeleted != 1 || g_baseDtor != 3) { std::printf("009c1ae0_TEST FAIL\n"); return 1; }
    std::printf("009c1ae0_TEST PASS\n"); return 0;
}