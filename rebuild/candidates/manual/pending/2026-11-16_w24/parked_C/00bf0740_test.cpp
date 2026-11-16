// Behaviour test for CEnginePrimitiveRenderUtil::SetupOutlineStencilStates (0x00BF0740): self-contained doubles. The outline widths
// go to vertex constant 0 scaled by the glow factor / mesh scale / 100, white to constant 1, then the stencil-write state list:
// Z write, colour write and alpha test off; stencil on, ALWAYS, pass = REPLACE; write mask and reference = 1 << (StencilBit + 2).
#include <stdio.h>
#include <math.h>
struct CRGBFloatColour { float R, G, B, A; CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {} };
enum STENCIL_FUNC { STENCIL_FUNC_NEVER, STENCIL_FUNC_LESS, STENCIL_FUNC_EQUAL, STENCIL_FUNC_LESS_EQUAL, STENCIL_FUNC_GREATER, STENCIL_FUNC_NOT_EQUAL, STENCIL_FUNC_GREATER_EQUAL, STENCIL_FUNC_ALWAYS };
enum STENCIL_OP_MODE { STENCIL_OP_MODE_KEEP, STENCIL_OP_MODE_ZERO, STENCIL_OP_MODE_REPLACE };
struct CShaderRenderManager {
    float Constants[2][4]; int Uploads;
    void SetUserVertexShaderConstant(unsigned long reg, float x, float y, float z, float w) { Constants[reg][0] = x; Constants[reg][1] = y; Constants[reg][2] = z; Constants[reg][3] = w; ++Uploads; }
    void SetUserVertexShaderConstant(unsigned long reg, const CRGBFloatColour& c) { SetUserVertexShaderConstant(reg, c.R, c.G, c.B, c.A); }
};
struct CRenderManager {
    unsigned long ZWrite, ColourWrite, AlphaTest, StencilEnable, StencilFunc, StencilOpPass, StencilWriteMask, StencilRef, AlphaBlend, Fog; int Sets;
    void Set(unsigned long& state, unsigned long value) { state = value; ++Sets; }
    void SetZBufferWriteEnable(bool e) { Set(ZWrite, e ? 1 : 0); }
    void SetColourWriteEnable(bool e) { Set(ColourWrite, e ? 15 : 0); }
    void SetAlphaTestEnable(bool e) { Set(AlphaTest, e ? 1 : 0); }
    void SetStencilEnable(bool e) { Set(StencilEnable, e ? 1 : 0); }
    void SetStencilFunc(STENCIL_FUNC f) { Set(StencilFunc, (unsigned long)f + 1); }           // D3DCMP_* = index + 1
    void SetStencilOpPass(STENCIL_OP_MODE m) { Set(StencilOpPass, (unsigned long)m + 1); }    // D3DSTENCILOP_* = index + 1
    void SetStencilWriteMask(unsigned long m) { Set(StencilWriteMask, m); }
    void SetStencilRef(unsigned long r) { Set(StencilRef, r); }
};
struct CEngineScreenEffectOutlineGlow { float OutlineGlowScaleFactor; float PeekOutlineGlowScaleFactor() const { return OutlineGlowScaleFactor; } };
struct CEngineMeshEffectOutline { float WidthWorldSpace, WidthScreenSpace; unsigned long Colour; unsigned char StencilBit; };
static CRenderManager* GPRenderManager; static CShaderRenderManager* GPShaderRenderManager; static CEngineScreenEffectOutlineGlow* GPScreenEffectOutlineGlow;
static void SetupOutlineStencilStates(const CEngineMeshEffectOutline& effect, float mesh_scale)
{
    float width_world_space = effect.WidthWorldSpace * GPScreenEffectOutlineGlow->PeekOutlineGlowScaleFactor();
    float width_screen_space = effect.WidthScreenSpace * GPScreenEffectOutlineGlow->PeekOutlineGlowScaleFactor();
    GPShaderRenderManager->SetUserVertexShaderConstant(0, width_world_space * (1.0f / mesh_scale) * 0.01f, width_screen_space * (1.0f / mesh_scale) * 0.01f, 0.0f, 0.0f);
    GPShaderRenderManager->SetUserVertexShaderConstant(1, CRGBFloatColour(1, 1, 1, 1));
    GPRenderManager->SetZBufferWriteEnable(false);
    GPRenderManager->SetColourWriteEnable(false);
    GPRenderManager->SetAlphaTestEnable(false);
    GPRenderManager->SetStencilEnable(true);
    GPRenderManager->SetStencilFunc(STENCIL_FUNC_ALWAYS);
    GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_REPLACE);
    unsigned long stencil_mask = 1 << (effect.StencilBit + 2);
    GPRenderManager->SetStencilWriteMask(stencil_mask);
    GPRenderManager->SetStencilRef(stencil_mask);
}
static bool Near(float a, float b) { return fabs(a - b) <= 1e-6f * (1.0f + (float)fabs(b)); }
int main()
{
    static const float scales[] = { 0.25f, 1.0f, 3.0f }, glows[] = { 0.5f, 1.0f, 2.5f };
    int ok = 1, cases = 0;
    for (int s = 0; s < 3; ++s) for (int g = 0; g < 3; ++g) for (unsigned char bit = 0; bit < 6; ++bit) {
        CRenderManager rm; CShaderRenderManager sm; CEngineScreenEffectOutlineGlow glow;
        rm.ZWrite = rm.ColourWrite = rm.AlphaTest = rm.StencilEnable = rm.StencilFunc = rm.StencilOpPass = rm.StencilWriteMask = rm.StencilRef = 0x77; rm.AlphaBlend = rm.Fog = 0x55; rm.Sets = 0;
        sm.Uploads = 0; for (int r = 0; r < 2; ++r) for (int c = 0; c < 4; ++c) sm.Constants[r][c] = -9.0f;
        glow.OutlineGlowScaleFactor = glows[g];
        GPRenderManager = &rm; GPShaderRenderManager = &sm; GPScreenEffectOutlineGlow = &glow;
        CEngineMeshEffectOutline effect; effect.WidthWorldSpace = 2.0f + bit; effect.WidthScreenSpace = 7.0f - bit; effect.Colour = 0xff102030; effect.StencilBit = bit;
        SetupOutlineStencilStates(effect, scales[s]);
        ok = ok && sm.Uploads == 2 && rm.Sets == 8;
        ok = ok && Near(sm.Constants[0][0], effect.WidthWorldSpace * glows[g] / scales[s] / 100.0f) && Near(sm.Constants[0][1], effect.WidthScreenSpace * glows[g] / scales[s] / 100.0f);
        ok = ok && sm.Constants[0][2] == 0.0f && sm.Constants[0][3] == 0.0f;
        for (int c = 0; c < 4; ++c) ok = ok && sm.Constants[1][c] == 1.0f;
        ok = ok && rm.ZWrite == 0 && rm.ColourWrite == 0 && rm.AlphaTest == 0 && rm.StencilEnable == 1 && rm.StencilFunc == 8 && rm.StencilOpPass == 3;
        ok = ok && rm.StencilWriteMask == (4ul << bit) && rm.StencilRef == (4ul << bit) && rm.AlphaBlend == 0x55 && rm.Fog == 0x55;
        ++cases;
    }
    ok = ok && cases == 54;
    printf(ok ? "00bf0740_TEST PASS\n" : "00bf0740_TEST FAIL\n");
    return ok ? 0 : 1;
}
