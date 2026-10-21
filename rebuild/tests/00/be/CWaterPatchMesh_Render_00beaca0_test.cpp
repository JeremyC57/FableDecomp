#include <cstdio>
#include <string>
// behaviour test: CWaterPatchMesh::Render does nothing without a vertex buffer; otherwise it sets an identity world
// transform translated to the camera, binds the buffer (stride from the buffer) as stream 0, flushes the dirty shader
// state for flags 5 and draws 0x121 vertices indexed with the given triangle count
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 {
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
};
static std::string g_log;
struct CShaderRenderManager {
    unsigned long UpdateFlags;
    CMatrix3x4 World;
    void SetWorldTransform(const CMatrix3x4& m) { World = m; g_log += "world;"; }
    void PrepareForRender(unsigned long flags)
    {
        char b[64];
        std::sprintf(b, "prepare %lu %lx;", flags, UpdateFlags);
        g_log += b;
        UpdateFlags = 0;
    }
};
struct IDirect3DVertexBuffer9 { int Id; };
struct IDirect3DDevice9 {
    virtual long SetStreamSource(unsigned int stream, IDirect3DVertexBuffer9* data, unsigned int offset, unsigned int stride)
    {
        char b[64];
        std::sprintf(b, "stream %u %d %u %u;", stream, data ? data->Id : 0, offset, stride);
        g_log += b;
        return 0;
    }
    virtual ~IDirect3DDevice9() { }
};
struct CRenderManagerCore {
    IDirect3DDevice9* PD3DDevice;
    void DrawCurrentBuffersIndexed(unsigned long minIndex, unsigned long count, unsigned long start, unsigned long prims)
    {
        char b[64];
        std::sprintf(b, "draw %lu %lu %lu %lu;", minIndex, count, start, prims);
        g_log += b;
    }
};
struct CEngineCamera { C3DVector WorldPosition; };
struct CVertexBufferWin32 { IDirect3DVertexBuffer9* PVertexBuffer; unsigned char Stride; };
static CShaderRenderManager g_shader;
static CRenderManagerCore g_core;
static CEngineCamera g_camera;
static CShaderRenderManager* g_pShaderRenderManager = &g_shader;
static CRenderManagerCore* g_pRenderManagerCore = &g_core;
static CEngineCamera* g_pEngineCamera = &g_camera;
struct CWaterPatchMesh {
    CVertexBufferWin32* VertexBuffer;
    void Render(long triangleCount) const;
};
void CWaterPatchMesh::Render(long triangleCount) const
{
    if (!VertexBuffer)
        return;
    CMatrix3x4 transform;
    const CEngineCamera* camera = g_pEngineCamera;
    transform.Position.X = camera->WorldPosition.X;
    transform.Position.Y = camera->WorldPosition.Y;
    transform.Position.Z = camera->WorldPosition.Z;
    g_pShaderRenderManager->SetWorldTransform(transform);
    CVertexBufferWin32* vertexBuffer = VertexBuffer;
    if (vertexBuffer)
        g_pRenderManagerCore->PD3DDevice->SetStreamSource(0, vertexBuffer->PVertexBuffer, 0, vertexBuffer->Stride);
    else
        g_pRenderManagerCore->PD3DDevice->SetStreamSource(0, 0, 0, 0);
    g_pShaderRenderManager->PrepareForRender(5);
    g_pRenderManagerCore->DrawCurrentBuffersIndexed(0, 0x121, 0, triangleCount);
}
int main()
{
    IDirect3DDevice9 device;
    g_core.PD3DDevice = &device;
    g_camera.WorldPosition.X = 10.0f; g_camera.WorldPosition.Y = -4.0f; g_camera.WorldPosition.Z = 2.5f;
    g_shader.UpdateFlags = 0x85;
    IDirect3DVertexBuffer9 d3dBuffer = { 77 };
    CVertexBufferWin32 buffer = { &d3dBuffer, 24 };
    CWaterPatchMesh mesh;
    mesh.VertexBuffer = 0;
    mesh.Render(9);
    bool ok = g_log.empty();
    mesh.VertexBuffer = &buffer;
    mesh.Render(512);
    const CMatrix3x4& w = g_shader.World;
    ok = ok && g_log == "world;stream 0 77 0 24;prepare 5 85;draw 0 289 0 512;"
        && w.XAxis.X == 1.0f && w.YAxis.Y == 1.0f && w.ZAxis.Z == 1.0f && w.XAxis.Y == 0.0f && w.ZAxis.X == 0.0f
        && w.Position.X == 10.0f && w.Position.Y == -4.0f && w.Position.Z == 2.5f;
    if (!ok) { std::printf("00beaca0_TEST FAIL %s\n", g_log.c_str()); return 1; }
    std::printf("00beaca0_TEST PASS\n");
    return 0;
}