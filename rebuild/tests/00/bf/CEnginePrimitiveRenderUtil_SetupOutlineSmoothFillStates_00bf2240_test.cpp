#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CRGBFLOATCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_C4DVECTOR
#include "engine/CRGBFloatColour.h"
#include "engine/RenderStateEnums.h"
#include <cstdio>
#include <cmath>

static unsigned failures;
#define CHECK(x) do { if (!(x)) { if (failures < 20) std::printf("line %d: %s\n", __LINE__, #x); ++failures; } } while (0)

static bool Near(float actual, double expected)
{
    return std::fabs(actual - expected) <= 0.000002 * (1.0 + std::fabs(expected));
}

struct Outline {
    float WidthWorldSpace, WidthScreenSpace;
    CRGBColour Colour;
    unsigned char TextureAlphaRef, TextureAlphaBias;
};

struct Glow {
    float scale;
    unsigned reads;
    float PeekOutlineGlowScaleFactor() { ++reads; return scale; }
};

struct State {
    unsigned long desired, old;
    bool captured, dirty;
    State() : desired(0), old(0), captured(false), dirty(false) {}
};
enum StateId { ZBUFFER, ZWRITE, FOG, STENCIL, ALPHATEST, ALPHAREF, ALPHAFUNC, BLEND, SRC, DST, OP, COUNT };
struct Render {
    State states[COUNT];
    unsigned writes, order[COUNT], captures, queues;
    Render() : writes(0), captures(0), queues(0) {}
    void Set(StateId which, unsigned long desired)
    {
        CHECK(writes < COUNT);
        order[writes++] = which;
        State& state = states[which];
        if (state.desired == desired)
            return;
        if (!state.captured) {
            state.old = state.desired;
            state.captured = true;
            ++captures;
        }
        state.desired = desired;
        if (!state.dirty) {
            state.dirty = true;
            ++queues;
        }
    }
    void SetZBufferEnable(bool on) { Set(ZBUFFER,on); }
    void SetZBufferWriteEnable(bool on) { Set(ZWRITE,on); }
    void SetFogEnable(bool on) { Set(FOG,on); }
    void SetStencilEnable(bool on) { Set(STENCIL,on); }
    void SetAlphaTestEnable(bool on) { Set(ALPHATEST,on); }
    void SetAlphaRef(unsigned long ref) { Set(ALPHAREF,ref); }
    void SetAlphaTestFunc(ALPHA_TEST_FUNC mode) { Set(ALPHAFUNC,mode); }
    void SetAlphaBlendEnable(bool on) { Set(BLEND,on); }
    void SetSourceAlphaBlendMode(ALPHA_BLEND_MODE mode) { Set(SRC,mode); }
    void SetDestAlphaBlendMode(ALPHA_BLEND_MODE mode) { Set(DST,mode); }
    void SetBlendOp(BLEND_OP mode) { Set(OP,mode); }
};
struct Shader {
    unsigned vertices, pixels, updateFlags;
    C4DVector width, pixel;
    CRGBFloatColour colour;
    Shader() : vertices(0), pixels(0), updateFlags(0x20), pixel(4,3,2,1) {}
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w)
    {
        CHECK(index == 0 && vertices == 0);
        width = C4DVector(x,y,z,w);
        ++vertices;
    }
    void SetUserVertexShaderConstant(unsigned long index, const CRGBFloatColour& value)
    {
        CHECK(index == 1 && vertices == 1);
        colour = value;
        ++vertices;
    }
    void SetPixelShaderConstant(unsigned long index, const C4DVector& value)
    {
        CHECK(index == 0 && vertices == 2);
        pixel = value;
        ++pixels;
        updateFlags |= 2;
    }
};

namespace NEngineInternal {
    Render* GPRenderManager;
    Shader* GPShaderRenderManager;
    Glow* GPScreenEffectOutlineGlow;
}

struct Primitive {
    void SetupOutlineSmoothFillStates(const Outline&, float, bool, long);
};
#define CEnginePrimitiveRenderUtil Primitive
#define CEngineMeshEffectOutline Outline

void CEnginePrimitiveRenderUtil::SetupOutlineSmoothFillStates(const CEngineMeshEffectOutline& effect,
        float mesh_scale, bool enable_alpha, long depth_texture_stage)
{
    float width_world_space = effect.WidthWorldSpace * NEngineInternal::GPScreenEffectOutlineGlow->PeekOutlineGlowScaleFactor();
    float width_screen_space = effect.WidthScreenSpace * NEngineInternal::GPScreenEffectOutlineGlow->PeekOutlineGlowScaleFactor();
    CRGBColour colour = effect.Colour;
    long alpha_ref = effect.TextureAlphaRef;
    long alpha_bias = effect.TextureAlphaBias;
    width_world_space *= 0.01f / mesh_scale;
    width_screen_space *= 0.01f / mesh_scale;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,
        width_world_space, width_screen_space, 0.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, CRGBFloatColour(colour));

    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetFogEnable(false);
    NEngineInternal::GPRenderManager->SetStencilEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaRef(1);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);

    if (enable_alpha) {
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_MAX);
        float ref = alpha_ref * (1.0f / 255.0f);
        float bias = alpha_bias * (1.0f / 255.0f);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(bias, bias, bias, ref));
    }
    else {
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    }
}

int main()
{
    const float scale[4] = { 0.125f, 1.0f, -1.5f, 16.0f };
    const float strength[4] = { 0.0f, 0.5f, 1.0f, 2.0f };
    const unsigned char alpha[4] = { 0, 1, 115, 255 };
    for (unsigned scenario = 0; scenario != 512; ++scenario) {
        const bool enabled = (scenario & 1) != 0;
        const float meshScale = scale[(scenario >> 1) & 3];
        const float glowScale = strength[(scenario >> 3) & 3];
        const unsigned int ref = alpha[(scenario >> 5) & 3];
        const unsigned int bias = alpha[(scenario >> 7) & 3];
        Outline outline;
        outline.WidthWorldSpace = 5.25f;
        outline.WidthScreenSpace = -17.5f;
        outline.Colour.Initialise(64, 2, 255, 0);
        outline.TextureAlphaRef = (unsigned char)ref;
        outline.TextureAlphaBias = (unsigned char)bias;
        Glow glow;
        glow.scale = glowScale;
        glow.reads = 0;
        Shader shader;
        Render render;
        Primitive primitive;
        NEngineInternal::GPRenderManager = &render;
        NEngineInternal::GPShaderRenderManager = &shader;
        NEngineInternal::GPScreenEffectOutlineGlow = &glow;

        const unsigned long wanted[COUNT] = { 0, 0, 0, 0, 1, 1, ALPHA_TEST_GREATER,
            enabled ? 1UL : 0UL, ALPHA_BLEND_ONE, ALPHA_BLEND_ONE, BLEND_OP_MAX };
        unsigned needsCapture = 0, needsQueue = 0;
        for (unsigned i = 0; i != COUNT; ++i) {
            State& state = render.states[i];
            const unsigned statePattern = (scenario + i) & 7;
            state.desired = wanted[i] + ((statePattern & 1) ? 1 : 0);
            state.old = 100;
            state.captured = (statePattern & 2) != 0;
            state.dirty = (statePattern & 4) != 0;
            if (i < (enabled ? COUNT : SRC) && state.desired != wanted[i]) {
                if (!state.captured)
                    ++needsCapture;
                if (!state.dirty)
                    ++needsQueue;
            }
        }

        primitive.SetupOutlineSmoothFillStates(outline, meshScale, enabled, (long)scenario - 256L);

        CHECK(glow.reads == 2 && shader.vertices == 2);
        CHECK(Near(shader.width.X, 5.25 * glowScale * 0.01 / meshScale));
        CHECK(Near(shader.width.Y, -17.5 * glowScale * 0.01 / meshScale));
        CHECK(shader.width.Z == 0.0f && shader.width.W == 0.0f);
        CHECK(Near(shader.colour.R, 64.0 / 255.0) && Near(shader.colour.G, 2.0 / 255.0));
        CHECK(shader.colour.B == 1.0f && shader.colour.A == 0.0f);
        CHECK(shader.pixels == (enabled ? 1U : 0U) && shader.updateFlags == (enabled ? 0x22U : 0x20U));
        if (enabled) {
            CHECK(Near(shader.pixel.X, bias / 255.0) && Near(shader.pixel.Y, bias / 255.0));
            CHECK(Near(shader.pixel.Z, bias / 255.0) && Near(shader.pixel.W, ref / 255.0));
        }
        else {
            CHECK(shader.pixel.X == 4 && shader.pixel.Y == 3 && shader.pixel.Z == 2 && shader.pixel.W == 1);
        }
        const unsigned states = enabled ? COUNT : SRC;
        CHECK(render.writes == states && render.captures == needsCapture && render.queues == needsQueue);
        for (unsigned i = 0; i != COUNT; ++i) {
            const unsigned statePattern = (scenario + i) & 7;
            const unsigned long initial = wanted[i] + ((statePattern & 1) ? 1 : 0);
            const bool changedHere = i < states && initial != wanted[i];
            if (i < states)
                CHECK(render.order[i] == i);
            CHECK(render.states[i].desired == (i < states ? wanted[i] : initial));
            CHECK(render.states[i].captured == (changedHere || (statePattern & 2) != 0));
            CHECK(render.states[i].dirty == (changedHere || (statePattern & 4) != 0));
            CHECK(render.states[i].old == (changedHere && !(statePattern & 2) ? initial : 100));
        }
    }
    std::puts(failures ? "00bf2240_TEST FAIL" : "00bf2240_TEST PASS");
    return failures ? 1 : 0;
}