// behaviour test for 0x00BA7DB0 CShaderRenderManager::DrawBuffersIndexed(textures, textureCount, vertexBuffer, ...)
#include <cstdio>

struct Device {
    int Log[32]; int N;
    void* Bound[4]; int SetTextureCalls; int DrawType;
    Device() : N(0), SetTextureCalls(0), DrawType(0) { for (int i = 0; i < 4; ++i) Bound[i] = 0; }
    void SetStreamSource(unsigned int, void*, unsigned int, unsigned int) { Log[N++] = 1; }
    void SetTexture(unsigned int stage, void* t) { Log[N++] = 5; Bound[stage] = t; ++SetTextureCalls; }
    void SetIndices(void*) { Log[N++] = 2; }
    void DrawIndexedPrimitive(int type, int, unsigned int, unsigned int, unsigned int, unsigned int) { Log[N++] = 4; DrawType = type; }
};
struct Texture { void* PD3DTexture; };
struct RenderManager {
    Device* PD3DDevice; void* CurrentTextures[4]; int NumTextureStagesUsed;
    void RealiseRenderState() { PD3DDevice->Log[PD3DDevice->N++] = 3; }
    void SetTexture(int stage, void* t)
    {
        if (CurrentTextures[stage] != t) { CurrentTextures[stage] = t; PD3DDevice->SetTexture(stage, t); }
        if (NumTextureStagesUsed < stage + 1) NumTextureStagesUsed = stage + 1;
    }
    void SetTexture(int stage, const Texture* t) { SetTexture(stage, t ? t->PD3DTexture : 0); }
};
struct VertexBuffer { void* PVertexBuffer; unsigned char Stride; };
struct IndexBuffer { void* PD3DIndexBuffer; int PrimitiveType; };

struct ShaderRenderManager {
    RenderManager* RM;
    Device* D3DDevice;
    void Draw(const Texture* textures[], unsigned int count, VertexBuffer& vb, unsigned int minV, unsigned int numV,
              IndexBuffer& ib, unsigned int start, unsigned int prims, int type)
    {
        D3DDevice->SetStreamSource(0, vb.PVertexBuffer, 0, vb.Stride);
        for (unsigned int i = 0; i < count; ++i)
            RM->SetTexture((int)i, textures[i]);
        D3DDevice->SetIndices(ib.PD3DIndexBuffer);
        if (type == -1) type = ib.PrimitiveType;
        RM->RealiseRenderState();
        D3DDevice->DrawIndexedPrimitive(type, 0, minV, numV, start, prims);
    }
};

int main()
{
    Device dev;
    RenderManager rm; rm.PD3DDevice = &dev; rm.NumTextureStagesUsed = 0;
    for (int i = 0; i < 4; ++i) rm.CurrentTextures[i] = 0;
    ShaderRenderManager m; m.RM = &rm; m.D3DDevice = &dev;
    int d0 = 0, d1 = 0;
    Texture t0; t0.PD3DTexture = &d0;
    Texture t1; t1.PD3DTexture = &d1;
    const Texture* textures[3] = { &t0, 0, &t1 };
    VertexBuffer vb; vb.PVertexBuffer = 0; vb.Stride = 16;
    IndexBuffer ib; ib.PD3DIndexBuffer = 0; ib.PrimitiveType = 4;
    m.Draw(textures, 3, vb, 0, 4, ib, 0, 2, -1);
    if (dev.Bound[0] != &d0 || dev.Bound[1] != 0 || dev.Bound[2] != &d1 || dev.SetTextureCalls != 2) { std::printf("00ba7db0_TEST FAIL\n"); return 1; }
    if (rm.NumTextureStagesUsed != 3 || dev.DrawType != 4) { std::printf("00ba7db0_TEST FAIL\n"); return 1; }
    if (dev.Log[0] != 1 || dev.Log[dev.N - 3] != 2 || dev.Log[dev.N - 2] != 3 || dev.Log[dev.N - 1] != 4) { std::printf("00ba7db0_TEST FAIL\n"); return 1; }
    m.Draw(textures, 3, vb, 0, 4, ib, 0, 2, 5);   // cached textures: no rebinding
    if (dev.SetTextureCalls != 2 || dev.DrawType != 5) { std::printf("00ba7db0_TEST FAIL\n"); return 1; }
    std::printf("00ba7db0_TEST PASS\n");
    return 0;
}