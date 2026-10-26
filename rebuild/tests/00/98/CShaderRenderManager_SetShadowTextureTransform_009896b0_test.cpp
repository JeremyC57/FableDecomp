#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
class CMatrix4x4 { public: float m[16]; };
class CVertexShaderConstantLayout { public: unsigned long ShadowTextureTransformRegister; };
static int g_calls = 0; static unsigned long g_reg = 0, g_rows = 0; static const CMatrix4x4* g_matrix = 0; static const void* g_this = 0;
class CShaderRenderManager { public:
    CVertexShaderConstantLayout* CurrentVSConstantLayout;
    void SetVSMatrixConstantTransposed(unsigned long startRegister, const CMatrix4x4* matrix, unsigned long rows)
    { ++g_calls; g_this = this; g_reg = startRegister; g_matrix = matrix; g_rows = rows; }
    void SetShadowTextureTransform(const CMatrix4x4* matrix)
    { SetVSMatrixConstantTransposed(CurrentVSConstantLayout->ShadowTextureTransformRegister, matrix, 4); }
};
int main()
{
    CVertexShaderConstantLayout layout; layout.ShadowTextureTransformRegister = 40;
    CShaderRenderManager m; m.CurrentVSConstantLayout = &layout;
    CMatrix4x4 mat;
    m.SetShadowTextureTransform(&mat);
    if (g_calls != 1 || g_this != &m || g_reg != 40 || g_matrix != &mat || g_rows != 4) { std::printf("009896b0_TEST FAIL\n"); return 1; }
    std::printf("009896b0_TEST PASS\n");
    return 0;
}