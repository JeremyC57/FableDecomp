// Behaviour test for 0x00989640 CShaderRenderManager::UpdateWorldSpaceFogTransform.
#include <cstdio>
static unsigned int g_reg = 0, g_rows = 0; static float g_seen[4];
struct Device { long SetVertexShaderConstantF(unsigned int reg, const float* d, unsigned int rows) { g_reg = reg; g_rows = rows; for (int i = 0; i < 4; ++i) g_seen[i] = d[i]; return 0; } };
struct Range { long Offset; unsigned long Count; };
struct Layout { Range FogTransform; };
struct C4DVector { float X, Y, Z, W; C4DVector() {} C4DVector(const C4DVector& o) : X(o.X), Y(o.Y), Z(o.Z), W(o.W) {} };
struct Mgr {
    Layout* CurrentVSConstantLayout; Device* D3DDevice; C4DVector FogTransform; unsigned int UpdateFlags;
    void UpdateWorldSpaceFogTransform()
    {
        C4DVector fog = FogTransform;
        D3DDevice->SetVertexShaderConstantF(CurrentVSConstantLayout->FogTransform.Offset, &fog.X, 1);
        UpdateFlags = (UpdateFlags & ~0x2000u) | 0x4000u;
    }
};
int main()
{
    Layout l; l.FogTransform.Offset = 33; Device d; Mgr m; m.CurrentVSConstantLayout = &l; m.D3DDevice = &d;
    m.FogTransform.X = 1; m.FogTransform.Y = 2; m.FogTransform.Z = 3; m.FogTransform.W = 4; m.UpdateFlags = 0x2001;
    m.UpdateWorldSpaceFogTransform();
    if (g_reg != 33 || g_rows != 1 || g_seen[0] != 1 || g_seen[3] != 4 || m.UpdateFlags != 0x4001) { std::printf("00989640_TEST FAIL\n"); return 1; }
    std::printf("00989640_TEST PASS\n");
    return 0;
}