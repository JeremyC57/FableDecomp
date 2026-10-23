// Behaviour test for CEngineWaterRenderer::RenderIceBumpMap @ 0x00B6E3E0 (restated with fakes).
#include <cstdio>
#include <cstring>

struct C4DVector {
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
struct CTrackedRenderState { long id; unsigned long m_value; unsigned long m_captureMask; long applied; bool m_queued; };

struct Shader {
    C4DVector table[8];
    unsigned long UpdateFlags;
    int prepared;
    int vs, ps;
    C4DVector seen5[2], seen6[2];
    void SetPixelShaderConstant(unsigned long index, const C4DVector& value) { table[index] = value; UpdateFlags |= 2; }
    void PrepareForRender() { seen5[prepared] = table[5]; seen6[prepared] = table[6]; ++prepared; UpdateFlags = 0; }
};

struct Core {
    CTrackedRenderState s28b8, s291c, s2930;
    unsigned long mask;
    CTrackedRenderState* captured[8];
    unsigned long prior[8];
    long capturedCount;
    CTrackedRenderState* dirty[8];
    long dirtyCount;
    int draws;
    unsigned long states28b8AtDraw[2];
    void Draw(unsigned long, unsigned long n, unsigned long, unsigned long t) { if (n == 0xe && t == 2) states28b8AtDraw[draws] = s28b8.m_value; ++draws; }
};

static void SetState(CTrackedRenderState& state, unsigned long value, Core* core)
{
    if (state.m_value != value) {
        if ((core->mask & state.m_captureMask) == 0) {
            state.m_captureMask |= core->mask;
            core->captured[core->capturedCount] = &state;
            core->prior[core->capturedCount] = state.m_value;
            ++core->capturedCount;
        }
        state.m_value = value;
        if (!state.m_queued) {
            state.m_queued = true;
            core->dirty[core->dirtyCount] = &state;
            ++core->dirtyCount;
        }
    }
}

static unsigned long g_blend = 9;

static void RenderIceBumpMap(Shader& sh, Core& core)
{
    ++sh.vs; ++sh.ps;
    sh.SetPixelShaderConstant(5, C4DVector(1.0f, 0.0f, 0.0f, 0.0f));
    sh.SetPixelShaderConstant(6, C4DVector(0.0f, 1.0f, 0.0f, 0.0f));
    sh.PrepareForRender();
    core.Draw(0, 0xe, 0, 2);
    SetState(core.s28b8, 1, &core);
    SetState(core.s291c, g_blend, &core);
    SetState(core.s2930, g_blend, &core);
    sh.SetPixelShaderConstant(5, C4DVector(0.0f, 1.0f, 0.0f, 0.0f));
    sh.SetPixelShaderConstant(6, C4DVector(0.0f, 0.0f, 1.0f, 0.0f));
    sh.PrepareForRender();
    core.Draw(0, 0xe, 0, 2);
    SetState(core.s28b8, 0, &core);
}

int main()
{
    int fails = 0;
    Shader sh; std::memset(&sh, 0, sizeof sh);
    Core core; std::memset(&core, 0, sizeof core);
    core.mask = 2;
    core.s291c.m_captureMask = 2;   // already captured in this block
    RenderIceBumpMap(sh, core);
    if (sh.vs != 1 || sh.ps != 1 || sh.prepared != 2 || core.draws != 2) ++fails;
    if (sh.seen5[0].X != 1.0f || sh.seen6[0].Y != 1.0f || sh.seen5[1].Y != 1.0f || sh.seen6[1].Z != 1.0f) ++fails;
    if (core.states28b8AtDraw[0] != 0 || core.states28b8AtDraw[1] != 1 || core.s28b8.m_value != 0) ++fails;
    if (core.s291c.m_value != 9 || core.s2930.m_value != 9) ++fails;
    if (core.capturedCount != 2 || core.captured[0] != &core.s28b8 || core.captured[1] != &core.s2930) ++fails;
    if (core.dirtyCount != 3) ++fails;   // 28b8 queued once
    if (fails == 0) std::printf("00b6e3e0_TEST PASS\n"); else std::printf("00b6e3e0_TEST FAIL %d\n", fails);
    return fails;
}
