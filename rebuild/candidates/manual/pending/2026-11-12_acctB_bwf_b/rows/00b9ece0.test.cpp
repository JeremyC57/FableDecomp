// Behaviour test for 0x00B9ECE0 CEnginePrimitiveManagerStaticMeshesBase ctor: a copy of the ctor's logic over a small model --
// point-light index table, shader fetch by name from bank 1 / bank 2 with ref-counted assignment, the ps_1_4 gate on the
// colour-buffer shadow shaders, the pixel shader and the shadow alias pointers.
#include <cstdio>
#include <cstring>
struct Data { int ref; char name[96]; };
static Data pool[128]; static int npool = 0;
struct Shader {
    Data* D;
    Shader() : D(0) {}
    Shader& operator=(Data* d) { if (D) --D->ref; D = d; ++D->ref; return *this; }
};
struct Bank { int id; Data* Get(const char* n) const { Data* d = &pool[npool++]; d->ref = 0; std::sprintf(d->name, "%d:%s", id, n); return d; } };
struct Model {
    unsigned long PointLightShaderIndex[6];
    Shader Static[4], Spot[4], ColBuff[4], Bump[4], DepthShadow, Pixel;
    Shader* StaticShadow; Shader* ShadowBufferRender; long OverridePrimitive, RenderOnlyNMeshes;
    Model(const Bank* b1, const Bank* b2, unsigned long psver) {
        PointLightShaderIndex[0] = 0; PointLightShaderIndex[1] = 1; PointLightShaderIndex[2] = 1;
        PointLightShaderIndex[3] = 2; PointLightShaderIndex[4] = 2; PointLightShaderIndex[5] = 3;
        const Bank* s = b1; const Bank* b = b2;
        Static[0] = s->Get("VSHADER_STATIC_DIRLIGHT_FOG");
        Static[3] = s->Get("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG");
        DepthShadow = s->Get("VSHADER_STATIC_SHADOW_BUFFER_RENDER");
        Bump[0] = b->Get("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_FOG_SHADOW");
        Spot[0] = s->Get("VSHADER_STATIC_DIRLIGHT_SPOT_LIGHT");
        if (psver >= 1400) { ColBuff[0] = s->Get("VSHADER_STATIC_DIRLIGHT_FOG_COLBUFF_SHADOW"); }
        Pixel = s->Get("PSHADER_TEXTURE_DIFFUSE_FOG_STIPPLE_ALPHA");
        ShadowBufferRender = &DepthShadow; StaticShadow = Static; OverridePrimitive = 0; RenderOnlyNMeshes = -1;
    }
};
int main()
{
    Bank b1 = {1}, b2 = {2};
    Model hi(&b1, &b2, 1400), lo(&b1, &b2, 1399);
    unsigned long want[6] = {0, 1, 1, 2, 2, 3};
    bool ok = std::memcmp(hi.PointLightShaderIndex, want, sizeof want) == 0
        && std::strcmp(hi.Static[0].D->name, "1:VSHADER_STATIC_DIRLIGHT_FOG") == 0 && hi.Static[0].D->ref == 1
        && std::strcmp(hi.Bump[0].D->name, "2:VSHADER_STATIC_DP3_BUMP_DIRLIGHT_FOG_SHADOW") == 0
        && hi.ColBuff[0].D != 0 && lo.ColBuff[0].D == 0 && lo.Static[1].D == 0
        && hi.ShadowBufferRender == &hi.DepthShadow && hi.StaticShadow == hi.Static
        && hi.OverridePrimitive == 0 && hi.RenderOnlyNMeshes == -1 && hi.Pixel.D != 0;
    Data* old = hi.Static[0].D; hi.Static[0] = b1.Get("X");
    ok = ok && old->ref == 0 && hi.Static[0].D->ref == 1;
    if (!ok) { std::printf("00b9ece0_TEST FAIL\n"); return 1; }
    std::printf("00b9ece0_TEST PASS\n");
    return 0;
}
