// Behaviour test for CEnginePrimitiveRenderUtil::SetupOutlineSmoothAlphaStencilMaskStates 0x00BF01E0: user vertex constant 0 is
// zeroed and constant 1 set to white (1,1,1,1); the twelve states end at z writes off, colour writes 0, alpha test on with
// reference = TextureAlphaRef and func GREATER (D3D 5), alpha blending off, fog off, stencil on / ALWAYS (D3D 8) / pass REPLACE
// (D3D 3) / write mask 1 << (StencilBit + 2) / reference 0; only changed states are bookmarked and queued, each once.
#include <cstdio>
#include <cstring>

enum ALPHA_TEST_FUNC { ALPHA_TEST_GREATER = 4 };
enum STENCIL_FUNC { STENCIL_FUNC_ALWAYS = 7 };
enum STENCIL_OP_MODE { STENCIL_OP_MODE_REPLACE = 2 };
static const unsigned long D3DAlphaFuncModeTable[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
static const unsigned long D3DStencilFuncModeTable[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
static const unsigned long D3DStencilOperationModeTable[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

struct CStateInfo { unsigned long DesiredState, BookmarkMask; bool DirtyListFlag; };
struct CRenderManager {
    CStateInfo ZBufferWriteEnable, ColourWriteEnable, AlphaTestEnable, AlphaBlendEnable, AlphaRef, AlphaTestFunc, FogEnable,
               StencilEnable, StencilFunc, StencilOpPass, StencilWriteMask, StencilRef;
    unsigned long CurrentBookmarkMask; int StackSize, DirtySize;
    void SetRenderState(unsigned long value, CStateInfo& state)
    {
        if (state.DesiredState != value) {
            if ((state.BookmarkMask & CurrentBookmarkMask) == 0) { state.BookmarkMask |= CurrentBookmarkMask; ++StackSize; }
            state.DesiredState = value;
            if (!state.DirtyListFlag) { state.DirtyListFlag = true; ++DirtySize; }
        }
    }
    void SetZBufferWriteEnable(bool e) { SetRenderState(e ? 1 : 0, ZBufferWriteEnable); }
    void SetColourWriteEnable(bool e) { SetRenderState(e ? 15 : 0, ColourWriteEnable); }
    void SetAlphaTestEnable(bool e) { SetRenderState(e ? 1 : 0, AlphaTestEnable); }
    void SetAlphaBlendEnable(bool e) { SetRenderState(e ? 1 : 0, AlphaBlendEnable); }
    void SetAlphaRef(unsigned long r) { SetRenderState(r, AlphaRef); }
    void SetAlphaTestFunc(ALPHA_TEST_FUNC f) { SetRenderState(D3DAlphaFuncModeTable[f], AlphaTestFunc); }
    void SetFogEnable(bool e) { SetRenderState(e ? 1 : 0, FogEnable); }
    void SetStencilEnable(bool e) { SetRenderState(e ? 1 : 0, StencilEnable); }
    void SetStencilFunc(STENCIL_FUNC f) { SetRenderState(D3DStencilFuncModeTable[f], StencilFunc); }
    void SetStencilOpPass(STENCIL_OP_MODE m) { SetRenderState(D3DStencilOperationModeTable[m], StencilOpPass); }
    void SetStencilWriteMask(unsigned long m) { SetRenderState(m, StencilWriteMask); }
    void SetStencilRef(unsigned long r) { SetRenderState(r, StencilRef); }
};
struct CRGBFloatColour { float R, G, B, A; CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { } };
struct CShaderRenderManager {
    float Constants[4][4]; int Calls;
    void SetUserVertexShaderConstant(unsigned long i, float x, float y, float z, float w)
    { Constants[i][0] = x; Constants[i][1] = y; Constants[i][2] = z; Constants[i][3] = w; ++Calls; }
    void SetUserVertexShaderConstant(unsigned long i, const CRGBFloatColour& c)
    { Constants[i][0] = c.R; Constants[i][1] = c.G; Constants[i][2] = c.B; Constants[i][3] = c.A; ++Calls; }
};
namespace NEngineInternal { CRenderManager* GPRenderManager; CShaderRenderManager* GPShaderRenderManager; }
struct CEngineMeshEffectOutline { unsigned char StencilBit, TextureAlphaRef; };
struct CEnginePrimitiveRenderUtil {
    void SetupOutlineSmoothAlphaStencilMaskStates(const CEngineMeshEffectOutline& effect)
    {
        unsigned long alphaRef = effect.TextureAlphaRef;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f));
        NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
        NEngineInternal::GPRenderManager->SetColourWriteEnable(false);
        NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        NEngineInternal::GPRenderManager->SetAlphaRef(alphaRef);
        NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
        NEngineInternal::GPRenderManager->SetFogEnable(false);
        NEngineInternal::GPRenderManager->SetStencilEnable(true);
        NEngineInternal::GPRenderManager->SetStencilFunc(STENCIL_FUNC_ALWAYS);
        NEngineInternal::GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_REPLACE);
        NEngineInternal::GPRenderManager->SetStencilWriteMask(1 << (effect.StencilBit + 2));
        NEngineInternal::GPRenderManager->SetStencilRef(0);
    }
};

static int Run(unsigned char bit, unsigned char ref, bool preset)
{
    CRenderManager rm; CShaderRenderManager srm; std::memset(&rm, 0, sizeof rm); std::memset(&srm, 0, sizeof srm);
    rm.CurrentBookmarkMask = 2;
    rm.ZBufferWriteEnable.DesiredState = 1; rm.ColourWriteEnable.DesiredState = 15; rm.FogEnable.DesiredState = 1;
    rm.StencilRef.DesiredState = 9; rm.AlphaBlendEnable.DesiredState = 1;
    if (preset) { rm.AlphaRef.DesiredState = ref; rm.StencilWriteMask.DesiredState = 1UL << (bit + 2); }
    NEngineInternal::GPRenderManager = &rm; NEngineInternal::GPShaderRenderManager = &srm;
    CEngineMeshEffectOutline effect; effect.StencilBit = bit; effect.TextureAlphaRef = ref;
    CEnginePrimitiveRenderUtil util; util.SetupOutlineSmoothAlphaStencilMaskStates(effect);
    if (srm.Calls != 2 || srm.Constants[0][0] != 0.0f || srm.Constants[0][3] != 0.0f || srm.Constants[1][0] != 1.0f || srm.Constants[1][3] != 1.0f) return 1;
    if (rm.ZBufferWriteEnable.DesiredState != 0 || rm.ColourWriteEnable.DesiredState != 0 || rm.AlphaTestEnable.DesiredState != 1 ||
        rm.AlphaBlendEnable.DesiredState != 0 || rm.AlphaRef.DesiredState != ref || rm.AlphaTestFunc.DesiredState != 5 ||
        rm.FogEnable.DesiredState != 0 || rm.StencilEnable.DesiredState != 1 || rm.StencilFunc.DesiredState != 8 ||
        rm.StencilOpPass.DesiredState != 3 || rm.StencilWriteMask.DesiredState != (1UL << (bit + 2)) || rm.StencilRef.DesiredState != 0) return 2;
    int changed = 5 + 5 + (preset ? 0 : (ref != 0) + 1);   // 5 preset-differs + AlphaTestEnable/Func/StencilEnable/Func/OpPass + ref/mask
    if (rm.StackSize != changed || rm.DirtySize != changed || (rm.AlphaTestFunc.BookmarkMask & 2) == 0) return 3;
    return 0;
}

int main()
{
    int r;
    if ((r = Run(0, 0x80, false)) || (r = Run(3, 0x10, true)) || (r = Run(5, 0, false))) { std::printf("00bf01e0_TEST FAIL %d\n", r); return 1; }
    std::printf("00bf01e0_TEST PASS\n");
    return 0;
}