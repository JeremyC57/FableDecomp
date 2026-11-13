// Behaviour test for 0x00BCFF10 CEnginePrimitiveManagerAnimatedMeshesBase ctor: a model of the shader fetch (bank 3 / bank 4),
// the refcounted CVertexShader assignment (operator= adds a ref, the temporary's dtor releases it), the >= 1400 pixel-shader guard,
// PointLightShaderIndex and the selector set-up.
#include <cstdio>
#include <cstring>
struct Data { int refs; const char* name; int bank; };
static Data pool[128]; static int npool = 0; static int live_temps = 0;
struct VS {
    Data* d;
    VS() : d(0) {}
    VS(const VS& o) : d(o.d) { if (d) ++d->refs; }
    VS& operator=(const VS& o) { if (d) --d->refs; d = o.d; if (d) ++d->refs; return *this; }
    ~VS() { if (d) --d->refs; }
};
struct Res { int bank; VS Get(const char* n) const { Data* p = &pool[npool++]; p->refs = 0; p->name = n; p->bank = bank; VS v; v.d = p; ++p->refs; return v; } };
struct Mgr {
    unsigned long PointLightShaderIndex[6];
    VS Pal[4], DepthShadow[4], DP3Depth[4], Spot[4], Col[4], Outline, Pal2D, Pal2DDP3;
    VS* ShadowSel; VS* DP3Sel; float Fade;
    Mgr(const Res* r3, const Res* r4, unsigned long psver) {
        static const unsigned long idx[6] = {0,1,1,2,2,3};
        for (int i = 0; i < 6; ++i) PointLightShaderIndex[i] = idx[i];
        const Res* shader_resource = r3; const Res* dp3_shader_resource = r4;
        Pal[0] = shader_resource->Get("VSHADER_PALSKIN_DIRLIGHT_FOG");
        Outline = shader_resource->Get("VSHADER_PALSKIN_OUTLINE");
        for (int i = 0; i < 4; ++i) DepthShadow[i] = shader_resource->Get("VSHADER_PALSKIN_DIRLIGHT_FOG_SHADOW");
        for (int i = 0; i < 4; ++i) DP3Depth[i] = dp3_shader_resource->Get("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_FOG_SHADOW");
        if (psver >= 1400) {
            for (int i = 0; i < 4; ++i) Spot[i] = shader_resource->Get("VSHADER_RIGIDSKIN_DIRLIGHT_SPOT_LIGHT");
            for (int i = 0; i < 4; ++i) Col[i] = shader_resource->Get("VSHADER_PALSKIN_DIRLIGHT_FOG_COLBUFF_SHADOW");
        }
        Pal2D = shader_resource->Get("VSHADER_PALSKIN_2D_DIRLIGHT");
        Pal2DDP3 = dp3_shader_resource->Get("VSHADER_PALSKIN_2D_DP3_BUMP_DIRLIGHT");
        ShadowSel = DepthShadow; DP3Sel = DP3Depth; Fade = 2.0f;
    }
};
int main()
{
    Res r3 = {3}, r4 = {4};
    Mgr a(&r3, &r4, 0x578);
    int fail = 0;
    if (a.PointLightShaderIndex[0] != 0 || a.PointLightShaderIndex[2] != 1 || a.PointLightShaderIndex[5] != 3) fail = 1;
    if (!a.Spot[3].d || std::strcmp(a.Col[0].d->name, "VSHADER_PALSKIN_DIRLIGHT_FOG_COLBUFF_SHADOW") != 0) fail = 1;
    if (a.DP3Depth[0].d->bank != 4 || a.Pal2DDP3.d->bank != 4 || a.Pal2D.d->bank != 3 || a.Outline.d->bank != 3) fail = 1;
    if (a.Pal[0].d->refs != 1 || a.Spot[0].d->refs != 1) fail = 1;            // temporary released, member holds one ref
    if (a.ShadowSel != a.DepthShadow || a.DP3Sel != a.DP3Depth || a.Fade != 2.0f) fail = 1;
    Mgr b(&r3, &r4, 0x577);                                                     // ps_1_3: the guarded set stays empty
    if (b.Spot[0].d || b.Col[3].d || !b.Pal2D.d) fail = 1;
    if (fail) { std::printf("00bcff10_TEST FAIL\n"); return 1; }
    std::printf("00bcff10_TEST PASS\n");
    return 0;
}