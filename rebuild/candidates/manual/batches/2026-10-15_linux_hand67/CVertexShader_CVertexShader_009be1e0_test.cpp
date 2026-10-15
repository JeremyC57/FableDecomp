#include <cstdio>
static int g_deleted = 0;
static int g_baseDestroyed = 0;
struct CVertexShaderData {
    virtual ~CVertexShaderData() { ++g_deleted; }
    void* m_pShader; void* m_pDeclaration; long m_stride;
    long m_refCount;
};
struct CShaderBase {
    virtual void v00() { }
    ~CShaderBase() { ++g_baseDestroyed; }
};
struct CVertexShader : CShaderBase {
    CVertexShaderData* m_pData;
    virtual void v00() { }
    ~CVertexShader()
    {
        CVertexShaderData* data = m_pData;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
    }
};
int main()
{
    CVertexShaderData* d = new CVertexShaderData; d->m_refCount = 2;
    { CVertexShader a; a.m_pData = d; }
    if (g_deleted != 0 || d->m_refCount != 1 || g_baseDestroyed != 1) { std::printf("009be1e0_TEST FAIL\n"); return 1; }
    { CVertexShader b; b.m_pData = d; }
    if (g_deleted != 1 || g_baseDestroyed != 2) { std::printf("009be1e0_TEST FAIL\n"); return 1; }
    { CVertexShader c; c.m_pData = 0; }
    if (g_deleted != 1 || g_baseDestroyed != 3) { std::printf("009be1e0_TEST FAIL\n"); return 1; }
    std::printf("009be1e0_TEST PASS\n");
    return 0;
}
