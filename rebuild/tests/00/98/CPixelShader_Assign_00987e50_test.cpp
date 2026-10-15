// Behaviour test for CPixelShader::Assign (0x00987E50): old data released (deleted at zero), new data shared and counted.
#include <cstdio>
static int g_deleted = 0;
struct CPixelShaderData {
    unsigned long ReferenceCount;
    virtual ~CPixelShaderData() { ++g_deleted; }
};
struct CPixelShader {
    CPixelShaderData* Data;
    virtual ~CPixelShader() {}
    void Assign(const CPixelShader& other)
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
            Data = 0;
        }
        Data = other.Data;
        ++Data->ReferenceCount;
    }
};
int main()
{
    CPixelShaderData* oldData = new CPixelShaderData; oldData->ReferenceCount = 1;
    CPixelShaderData* newData = new CPixelShaderData; newData->ReferenceCount = 1;
    CPixelShader a, b;
    a.Data = oldData; b.Data = newData;
    a.Assign(b);
    if (a.Data != newData || newData->ReferenceCount != 2 || g_deleted != 1) { std::printf("00987e50_TEST FAIL\n"); return 1; }
    CPixelShader c; c.Data = 0;
    c.Assign(b);
    if (c.Data != newData || newData->ReferenceCount != 3 || g_deleted != 1) { std::printf("00987e50_TEST FAIL\n"); return 1; }
    a.Assign(b);   // same data: drop one reference, take it back
    if (a.Data != newData || newData->ReferenceCount != 3 || g_deleted != 1) { std::printf("00987e50_TEST FAIL\n"); return 1; }
    delete newData;
    std::printf("00987e50_TEST PASS\n");
    return 0;
}