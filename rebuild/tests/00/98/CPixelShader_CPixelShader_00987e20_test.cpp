#include <cstdio>
static int g_deleted = 0;
static int g_baseDestroyed = 0;
struct CPixelShaderData {
    virtual ~CPixelShaderData() { ++g_deleted; }
    void* m_pShader; 
    long m_refCount;
};
struct CShaderBase {
    virtual void v00() { }
    ~CShaderBase() { ++g_baseDestroyed; }
};
struct CPixelShader : CShaderBase {
    CPixelShaderData* m_pData;
    virtual void v00() { }
    ~CPixelShader()
    {
        CPixelShaderData* data = m_pData;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
    }
};
int main()
{
    CPixelShaderData* d = new CPixelShaderData; d->m_refCount = 2;
    { CPixelShader a; a.m_pData = d; }
    if (g_deleted != 0 || d->m_refCount != 1 || g_baseDestroyed != 1) { std::printf("00987e20_TEST FAIL\n"); return 1; }
    { CPixelShader b; b.m_pData = d; }
    if (g_deleted != 1 || g_baseDestroyed != 2) { std::printf("00987e20_TEST FAIL\n"); return 1; }
    { CPixelShader c; c.m_pData = 0; }
    if (g_deleted != 1 || g_baseDestroyed != 3) { std::printf("00987e20_TEST FAIL\n"); return 1; }
    std::printf("00987e20_TEST PASS\n");
    return 0;
}