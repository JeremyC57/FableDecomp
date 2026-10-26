// Behaviour test for 0x009894B0 CShaderRenderManager::UpdateWorldSpaceTextureTransform: ranges with count 0
// are skipped, counts above 4 are clamped, the flags move from 0x1000 to 0x800.
#include <cstdio>
static int g_calls = 0; static unsigned int g_regs[4], g_rows[4]; static const float* g_data[4];
struct Device { long SetVertexShaderConstantF(unsigned int reg, const float* d, unsigned int rows) { g_regs[g_calls] = reg; g_rows[g_calls] = rows; g_data[g_calls] = d; ++g_calls; return 0; } };
struct Range { long Offset; unsigned long Count; };
struct Layout { Range TextureTransform[4]; };
class CMatrix4x4 { public: float m[16]; };
struct Mgr {
    Layout* CurrentVSConstantLayout; Device* D3DDevice; CMatrix4x4 TextureTransforms[4]; unsigned long TextureTransformCount; unsigned int UpdateFlags;
    void UpdateWorldSpaceTextureTransform()
    {
        for (unsigned long i = 0; i < TextureTransformCount; ++i) {
            const Range& range = CurrentVSConstantLayout->TextureTransform[i];
            unsigned long count = range.Count;
            if (count) D3DDevice->SetVertexShaderConstantF(range.Offset, TextureTransforms[i].m, count < 4 ? count : 4);
        }
        UpdateFlags = (UpdateFlags & ~0x1000u) | 0x800u;
    }
};
int main()
{
    Layout l; l.TextureTransform[0].Offset = 10; l.TextureTransform[0].Count = 2; l.TextureTransform[1].Offset = 20; l.TextureTransform[1].Count = 0;
    l.TextureTransform[2].Offset = 30; l.TextureTransform[2].Count = 9;
    Device d; Mgr m; m.CurrentVSConstantLayout = &l; m.D3DDevice = &d; m.TextureTransformCount = 3; m.UpdateFlags = 0x1001;
    m.UpdateWorldSpaceTextureTransform();
    if (g_calls != 2 || g_regs[0] != 10 || g_rows[0] != 2 || g_regs[1] != 30 || g_rows[1] != 4 || g_data[1] != m.TextureTransforms[2].m || m.UpdateFlags != 0x801) {
        std::printf("009894b0_TEST FAIL\n"); return 1;
    }
    std::printf("009894b0_TEST PASS\n");
    return 0;
}