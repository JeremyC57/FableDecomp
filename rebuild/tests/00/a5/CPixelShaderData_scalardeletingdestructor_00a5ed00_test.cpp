#include <cstdio>
static int g_released = 0;
static int g_baseDestroyed = 0;
struct IDirect3DPixelShader9 {
    virtual long QueryInterface(const void*, void**) { return 0; }
    virtual unsigned long AddRef() { return 1; }
    virtual unsigned long Release() { ++g_released; return 0; }
};
struct CBaseClass { CBaseClass() { } virtual ~CBaseClass() { ++g_baseDestroyed; } };
struct CPixelShaderData : CBaseClass {
    IDirect3DPixelShader9* PixelShader; unsigned long ReferenceCount; unsigned long ConstCount; long* ConstIndices;
    CPixelShaderData() : PixelShader(0), ReferenceCount(1), ConstCount(0), ConstIndices(0) { }
    virtual ~CPixelShaderData() { if (PixelShader) PixelShader->Release(); delete[] ConstIndices; }
};
int main()
{
    IDirect3DPixelShader9 shader;
    CPixelShaderData* a = new CPixelShaderData; a->PixelShader = &shader; a->ConstIndices = new long[4]; a->ConstCount = 4;
    CBaseClass* base = a;
    delete base;
    if (g_released != 1 || g_baseDestroyed != 1) { std::printf("00a5ed00_TEST FAIL\n"); return 1; }
    CPixelShaderData* b = new CPixelShaderData;
    delete b;
    if (g_released != 1 || g_baseDestroyed != 2) { std::printf("00a5ed00_TEST FAIL\n"); return 1; }
    std::printf("00a5ed00_TEST PASS\n");
    return 0;
}