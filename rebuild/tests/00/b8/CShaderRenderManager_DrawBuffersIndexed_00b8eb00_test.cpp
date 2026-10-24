// behaviour test for 0x00B8EB00 CShaderRenderManager::DrawBuffersIndexed(vertexBuffer, min, num, indexBuffer, start, count, type)
#include <cstdio>

struct Device {
    int Log[16]; int N;
    void* Stream; unsigned int Stride; void* Indices;
    int DrawArgs[6];
    Device() : N(0), Stream(0), Stride(0), Indices(0) { }
    void SetStreamSource(unsigned int stream, void* data, unsigned int offset, unsigned int stride) { Log[N++] = 1; Stream = data; Stride = stride; (void)stream; (void)offset; }
    void SetIndices(void* data) { Log[N++] = 2; Indices = data; }
    void DrawIndexedPrimitive(int type, int base, unsigned int minV, unsigned int numV, unsigned int start, unsigned int count)
    { Log[N++] = 4; DrawArgs[0] = type; DrawArgs[1] = base; DrawArgs[2] = (int)minV; DrawArgs[3] = (int)numV; DrawArgs[4] = (int)start; DrawArgs[5] = (int)count; }
};
struct RenderStateManager { Device* Dev; void RealiseRenderState() { Dev->Log[Dev->N++] = 3; } };
struct VertexBuffer { void* PVertexBuffer; unsigned char Stride; };
struct IndexBuffer { void* PD3DIndexBuffer; int PrimitiveType; };

struct ShaderRenderManager {
    RenderStateManager* RenderManager;
    Device* D3DDevice;
    void DrawBuffersIndexed(VertexBuffer& vb, unsigned int minV, unsigned int numV, IndexBuffer& ib, unsigned int start, unsigned int count, int type)
    {
        D3DDevice->SetStreamSource(0, vb.PVertexBuffer, 0, vb.Stride);
        D3DDevice->SetIndices(ib.PD3DIndexBuffer);
        if (type == -1) type = ib.PrimitiveType;
        RenderManager->RealiseRenderState();
        D3DDevice->DrawIndexedPrimitive(type, 0, minV, numV, start, count);
    }
};

int main()
{
    Device dev; RenderStateManager rsm; rsm.Dev = &dev;
    ShaderRenderManager m; m.RenderManager = &rsm; m.D3DDevice = &dev;
    int vbData = 0, ibData = 0;
    VertexBuffer vb; vb.PVertexBuffer = &vbData; vb.Stride = 32;
    IndexBuffer ib; ib.PD3DIndexBuffer = &ibData; ib.PrimitiveType = 5;
    m.DrawBuffersIndexed(vb, 2, 30, ib, 6, 10, -1);
    if (dev.N != 4 || dev.Log[0] != 1 || dev.Log[1] != 2 || dev.Log[2] != 3 || dev.Log[3] != 4) { std::printf("00b8eb00_TEST FAIL\n"); return 1; }
    if (dev.Stream != &vbData || dev.Stride != 32 || dev.Indices != &ibData) { std::printf("00b8eb00_TEST FAIL\n"); return 1; }
    if (dev.DrawArgs[0] != 5 || dev.DrawArgs[1] != 0 || dev.DrawArgs[2] != 2 || dev.DrawArgs[3] != 30 || dev.DrawArgs[4] != 6 || dev.DrawArgs[5] != 10) { std::printf("00b8eb00_TEST FAIL\n"); return 1; }
    dev.N = 0;
    m.DrawBuffersIndexed(vb, 0, 3, ib, 0, 1, 4);
    if (dev.DrawArgs[0] != 4) { std::printf("00b8eb00_TEST FAIL\n"); return 1; }
    std::printf("00b8eb00_TEST PASS\n");
    return 0;
}