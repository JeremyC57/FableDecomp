// Behaviour test for 0x00988840 CShaderRenderManager::SetVSMatrixConstantTransposed (16-dword source): the
// transposed block handed to a host device stub, and no call for zero rows.
#include <cstdio>
static unsigned int g_seen[16]; static unsigned int g_reg = 0, g_rows = 0; static int g_calls = 0;
struct Device { long SetVertexShaderConstantF(unsigned int reg, const float* data, unsigned int rows)
    { ++g_calls; g_reg = reg; g_rows = rows; const unsigned int* d = (const unsigned int*)data; for (unsigned int i = 0; i < rows * 4 && i < 16; ++i) g_seen[i] = d[i]; return 0; } };
struct CMatrix { unsigned int m[16]; };
struct Mgr {
    Device* D3DDevice;
    void SetVSMatrixConstantTransposed(unsigned long reg, const CMatrix* matrix, unsigned long rows)
    {
        static const int order[16] = { 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15 };
        unsigned int t[16];
        for (int k = 0; k < 16; ++k) t[k] = matrix->m[order[k]];
        if (rows) D3DDevice->SetVertexShaderConstantF(reg, (const float*)t, rows);
    }
};
int main()
{
    CMatrix src; for (int i = 0; i < 16; ++i) src.m[i] = i; Device dev; Mgr m; m.D3DDevice = &dev;
    m.SetVSMatrixConstantTransposed(5, &src, 0);
    if (g_calls != 0) { std::printf("00988840_TEST FAIL\n"); return 1; }
    m.SetVSMatrixConstantTransposed(5, &src, 2);
    static const int order[16] = { 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15 };
    bool ok = g_calls == 1 && g_reg == 5 && g_rows == 2;
    for (int i = 0; i < 8; ++i) ok = ok && g_seen[i] == (unsigned int)order[i];
    if (!ok) { std::printf("00988840_TEST FAIL\n"); return 1; }
    std::printf("00988840_TEST PASS\n");
    return 0;
}