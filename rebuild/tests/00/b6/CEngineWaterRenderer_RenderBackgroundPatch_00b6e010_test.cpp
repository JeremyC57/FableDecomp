// Behaviour test for CEngineWaterRenderer::RenderBackgroundPatch (0x00B6E010): world transform at the camera, the main draw,
// then one draw per present bridge strip with vertex count = length / stride.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 { float M[9]; C3DVector Pos; };
struct CVertexBufferWin32 { int PVertexBuffer; int Length; unsigned char Stride; };
struct CIndexBuffer { int PD3DIndexBuffer; };
struct Draw { int vb; int stride; int ib; unsigned long verts; unsigned long tris; };
static Draw g_draws[8]; static int g_nDraws; static int g_curVB, g_curStride, g_curIB; static CMatrix3x4 g_world; static unsigned long g_flags;
struct CRenderManagerCore {
    CIndexBuffer* PSelectedIndexBuffer;
    void SetVertexBuffer(CVertexBufferWin32* b) { if (b) { g_curVB = b->PVertexBuffer; g_curStride = b->Stride; } else { g_curVB = 0; g_curStride = 0; } }
    void SetIndexBuffer(CIndexBuffer* b) { PSelectedIndexBuffer = b; g_curIB = b ? b->PD3DIndexBuffer : 0; }
    void DrawCurrentBuffersAsIndexedTris(unsigned long, unsigned long verts, unsigned long, unsigned long tris)
    { Draw d = { g_curVB, g_curStride, g_curIB, verts, tris }; g_draws[g_nDraws++] = d; }
};
struct CShaderRenderManager {
    void SetWorldTransform(const CMatrix3x4& m) { g_world = m; }
    void PrepareForRender(unsigned long f) { g_flags = f; }
};
static CRenderManagerCore g_core; static CShaderRenderManager g_srm; static C3DVector g_cam;
struct CEngineWaterBackgroundSubPatch {
    CIndexBuffer* IndexBuffer; CVertexBufferWin32* VertexBuffer;
    CIndexBuffer* BridgeIndexBuffers[4]; CVertexBufferWin32* BridgeVertexBuffers[4];
    unsigned short PolyCount, VertexCount, BridgePolyCount[4];
};
static void RenderBackgroundPatch(CEngineWaterBackgroundSubPatch* patch)
{
    CMatrix3x4 world;
    for (int i = 0; i < 9; ++i) world.M[i] = (i % 4 == 0) ? 1.0f : 0.0f;
    world.Pos = g_cam;
    g_srm.SetWorldTransform(world);
    g_srm.PrepareForRender(1);
    g_core.SetVertexBuffer(patch->VertexBuffer);
    g_core.SetIndexBuffer(patch->IndexBuffer);
    g_core.DrawCurrentBuffersAsIndexedTris(0, patch->VertexCount, 0, patch->PolyCount);
    for (int i = 0; i < 4; ++i) {
        if (patch->BridgeVertexBuffers[i]) {
            g_core.SetVertexBuffer(patch->BridgeVertexBuffers[i]);
            g_core.SetIndexBuffer(patch->BridgeIndexBuffers[i]);
            g_core.DrawCurrentBuffersAsIndexedTris(0, patch->BridgeVertexBuffers[i]->Length / patch->BridgeVertexBuffers[i]->Stride, 0, patch->BridgePolyCount[i]);
        }
    }
}
int main()
{
    CVertexBufferWin32 vb = { 11, 0, 24 }, b1 = { 21, 240, 24 }, b3 = { 23, 96, 32 };
    CIndexBuffer ib = { 12 }, i1 = { 31 }, i3 = { 33 };
    CEngineWaterBackgroundSubPatch p;
    p.IndexBuffer = &ib; p.VertexBuffer = &vb; p.PolyCount = 50; p.VertexCount = 40;
    for (int i = 0; i < 4; ++i) { p.BridgeIndexBuffers[i] = 0; p.BridgeVertexBuffers[i] = 0; p.BridgePolyCount[i] = (unsigned short)(i + 1); }
    p.BridgeVertexBuffers[1] = &b1; p.BridgeIndexBuffers[1] = &i1; p.BridgeVertexBuffers[3] = &b3; p.BridgeIndexBuffers[3] = &i3;
    g_cam.X = 1; g_cam.Y = 2; g_cam.Z = 3;
    RenderBackgroundPatch(&p);
    int ok = g_nDraws == 3 && g_flags == 1 && g_world.Pos.Y == 2 && g_world.M[4] == 1 && g_world.M[3] == 0
        && g_draws[0].vb == 11 && g_draws[0].ib == 12 && g_draws[0].verts == 40 && g_draws[0].tris == 50
        && g_draws[1].vb == 21 && g_draws[1].ib == 31 && g_draws[1].verts == 10 && g_draws[1].tris == 2
        && g_draws[2].vb == 23 && g_draws[2].stride == 32 && g_draws[2].verts == 3 && g_draws[2].tris == 4
        && g_core.PSelectedIndexBuffer == &i3;
    if (!ok) { std::printf("00b6e010_TEST FAIL\n"); return 1; }
    std::printf("00b6e010_TEST PASS\n");
    return 0;
}